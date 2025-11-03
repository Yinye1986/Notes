#include "nec.h"
#include "stm32f1xx_hal_tim.h"
#include "tim.h"
#include "usart.h"

// --- NEC 协议常量 (基于 1 us 精度) ---
#define NEC_LEADER_HIGH_US 9000U  // 引导码高电平标准值 (标准 9000us)
#define NEC_LEADER_LOW_US 4500U   // 引导码低电平标准值 (标准 4500us)
#define NEC_BIT_HIGH_US 562U      // 位高电平标准值 (标准 562.5us)
#define NEC_LOGIC_0_LOW_US 562U   // 逻辑 '0' 低电平标准值 (标准 562.5us)
#define NEC_LOGIC_1_LOW_US 1687U  // 逻辑 '1' 低电平标准值 (标准 1687.5us)

// 容差 (20%)
#define TOLERANCE 0.20
#define NEC_LEADER_HIGH_MAX ((uint32_t)(NEC_LEADER_HIGH_US * (1 + TOLERANCE)))  // 引导码高电平最大值
#define NEC_LEADER_HIGH_MIN ((uint32_t)(NEC_LEADER_HIGH_US * (1 - TOLERANCE)))  // 引导码高电平最小值
#define NEC_LEADER_LOW_MAX ((uint32_t)(NEC_LEADER_LOW_US * (1 + TOLERANCE)))    // 引导码低电平最大值
#define NEC_LEADER_LOW_MIN ((uint32_t)(NEC_LEADER_LOW_US * (1 - TOLERANCE)))    // 引导码低电平最小值
#define NEC_LOGIC_0_LOW_MAX ((uint32_t)(NEC_LOGIC_0_LOW_US * (1 + TOLERANCE)))  // 逻辑 '0' 低电平最大值
#define NEC_LOGIC_0_LOW_MIN ((uint32_t)(NEC_LOGIC_0_LOW_US * (1 - TOLERANCE)))  // 逻辑 '0' 低电平最小值
#define NEC_LOGIC_1_LOW_MAX ((uint32_t)(NEC_LOGIC_1_LOW_US * (1 + TOLERANCE)))  // 逻辑 '1' 低电平最大值
#define NEC_LOGIC_1_LOW_MIN ((uint32_t)(NEC_LOGIC_1_LOW_US * (1 - TOLERANCE)))  // 逻辑 '1' 低电平最小值

volatile NEC_StatusTypeDef status = NEC_STATE_IDLE;
volatile uint32_t lastCapture = 0;  // 上一次捕获时间戳
volatile uint32_t rawData = 0;      // 32 位数据接收缓冲区
volatile uint8_t bitIndex = 0;      // 当前接收的位数 (0-31)
volatile uint8_t edgeCount = 0;     // 边沿计数，用于辅助判断引导码

// NEC结果全局变量
NecResult_t NecResult = {0, 0, 0};

void NEC_Init(void) {
  // 启动定时器基准 (已经在 tim.c 的 MX_TIM2_Init 中完成)
  HAL_TIM_Base_Start(HANDLE);
  // 启动 TIM2 输入捕获, 并启用中断 假设红外信号接在 TIM2_CH1
  HAL_TIM_IC_Start_IT(HANDLE, TIM_CHANNEL_1);
  // 设置初始捕获极性为上升沿捕获 (等待信号起始)
  __HAL_TIM_SET_CAPTUREPOLARITY(HANDLE, TIM_CHANNEL_1,
                                TIM_INPUTCHANNELPOLARITY_RISING);
  status = NEC_STATE_IDLE;
  lastCapture = 0;
  rawData = 0;
  bitIndex = 0;
  edgeCount = 0;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
    uint32_t currentCapture = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
    uint32_t duration = 0;

    // 计算持续时间 (关键在于处理溢出)
    // 假设定时器是16位，最大值为0xFFFF
    if (currentCapture >= lastCapture) {
      duration = currentCapture - lastCapture;
    } else {
      // 溢出处理：从lastCapture到0xFFFF再到currentCapture
      duration = (0xFFFF - lastCapture) + currentCapture + 1;
    }

    switch (status) {
      case NEC_STATE_IDLE:
        // 捕获到引导码的上升沿
        // 切换为下降沿捕获以捕获引导码的下降沿
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,
                                      TIM_INPUTCHANNELPOLARITY_FALLING);
        status = NEC_STATE_LEADER_HIGH;
        lastCapture = currentCapture;
        break;

      case NEC_STATE_LEADER_HIGH: // 捕获到引导码的下降沿
        // 检查是否是9ms的高电平
        if (duration >= NEC_LEADER_HIGH_MIN && duration <= NEC_LEADER_HIGH_MAX) {
          // 进入引导码低电平检测
          __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,
                                        TIM_INPUTCHANNELPOLARITY_RISING);
          status = NEC_STATE_LEADER_LOW;
          lastCapture = currentCapture;
        } else {
          // 不满足引导码高电平要求，复位
          goto nec_reset;
        }
        break;

      case NEC_STATE_LEADER_LOW: // 捕获到引导码后的上升沿
        // 检查是否是4.5ms的低电平
        if (duration >= NEC_LEADER_LOW_MIN && duration <= NEC_LEADER_LOW_MAX) {
          // 进入数据位捕获状态，准备捕获第一个数据位的下降沿
          __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,
                                        TIM_INPUTCHANNELPOLARITY_FALLING);
          status = NEC_STATE_CAPTURING_HIGH;
          bitIndex = 0; // 确保位索引从 0 开始
          lastCapture = currentCapture;
        } else {
          // 不满足引导码低电平要求，复位
          goto nec_reset;
        }
        break;

      case NEC_STATE_CAPTURING_HIGH: // 捕获到数据位的下降沿 (开始低电平)
        // 等待低电平结束，即捕获下一个上升沿
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,
                                      TIM_INPUTCHANNELPOLARITY_RISING);
        status = NEC_STATE_CAPTURING_LOW;
        lastCapture = currentCapture;
        break;

      case NEC_STATE_CAPTURING_LOW: // 捕获到数据位的上升沿 (低电平结束)
        // 分析低电平时间以确定是逻辑0还是逻辑1
        if (duration >= NEC_LOGIC_0_LOW_MIN && duration <= NEC_LOGIC_0_LOW_MAX) {
          // 逻辑'0' - 不需要设置位，因为rawData初始化为0
          rawData &= ~(1U << bitIndex); // 明确设置为0
          bitIndex++;
        } else if (duration >= NEC_LOGIC_1_LOW_MIN && duration <= NEC_LOGIC_1_LOW_MAX) {
          // 逻辑'1'
          rawData |= (1U << bitIndex);
          bitIndex++;
        } else {
          // 不是有效的逻辑电平，复位
          goto nec_reset;
        }

        if (bitIndex >= 32) {
          // 已接收到所有32位数据
          Nec_Protocol_Received();
          goto nec_reset;
        } else {
          // 继续捕获下一个数据位的下降沿
          __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,
                                        TIM_INPUTCHANNELPOLARITY_FALLING);
          status = NEC_STATE_CAPTURING_HIGH;
          lastCapture = currentCapture;
        }
        break;

      default:
        // 如果状态异常，复位
        goto nec_reset;
    }
  }
  return;

nec_reset:
  // 复位状态机
  status = NEC_STATE_IDLE;
  rawData = 0;
  bitIndex = 0;
  edgeCount = 0;
  // 重新配置为捕获第一个上升沿
  __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,
                                TIM_INPUTCHANNELPOLARITY_RISING);
  lastCapture = 0;
}

void Nec_Protocol_Received(void) {
  uint8_t addr = (rawData >> 0) & 0xFF;      // 地址
  uint8_t addr_inv = (rawData >> 8) & 0xFF;  // 地址反码
  uint8_t cmd = (rawData >> 16) & 0xFF;      // 命令
  uint8_t cmd_inv = (rawData >> 24) & 0xFF;  // 命令反码

  // 校验：地址 + 地址反码 = 0xFF 且 命令 + 命令反码 = 0xFF
  if ((addr + addr_inv) == 0xFF && (cmd + cmd_inv) == 0xFF) {
    // 校验通过
    NecResult.address_ = addr;
    NecResult.command_ = cmd;
    NecResult.isValid_ = 1;

    // 串口输出调试信息
    printf("NEC OK: Addr=0x%02X, Cmd=0x%02X\r\n", addr, cmd);
  } else {
    // 校验失败
    NecResult.isValid_ = 0;
    printf("NEC Checksum Failed! Addr=0x%02X, ~Addr=0x%02X, Cmd=0x%02X, ~Cmd=0x%02X\r\n",
           addr, addr_inv, cmd, cmd_inv);
  }

  // 复位状态机，准备接收下一个信号
  status = NEC_STATE_IDLE;
  rawData = 0;
  bitIndex = 0;
  __HAL_TIM_SET_CAPTUREPOLARITY(HANDLE, TIM_CHANNEL_1,
                                TIM_INPUTCHANNELPOLARITY_RISING);
  lastCapture = 0;
}
