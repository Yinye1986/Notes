#include "nec.h"
#include "stm32f1xx_hal_tim.h"
#include "tim.h"

#include "usart.h"

// --- NEC 协议常量 (基于 1 us 精度) ---
#define NEC_LEADER_HIGH_MAX 9500U // 引导码高电平最大值 (标准 9000us)
#define NEC_LEADER_HIGH_MIN 8500U // 引导码高电平最小值
#define NEC_LEADER_LOW_MAX 5000   // 引导码低电平最大值 4500us
#define NEC_LEADER_LOW_MIN 4000   // 引导码低电平最小值
#define NEC_LOGIC_0_LOW_MAX 750U  // 逻辑 '0' 低电平最大值 562.9us
#define NEC_LOGIC_0_LOW_MIN 400U  // 逻辑 '0' 低电平最小值
#define NEC_LOGIC_1_LOW_MAX 1900U // 逻辑 '1' 低电平最大值 1687.5us
#define NEC_LOGIC_1_LOW_MIN 1500U // 逻辑 '1' 低电平最小值

volatile NEC_StatusTypeDef status = NEC_STATE_IDLE;
volatile uint32_t lastCapture = 0; // 上一次捕获时间戳
volatile uint32_t NecDataRaw = 0;  // 32 位数据接收缓冲区
volatile uint8_t BitIndex = 0;     // 当前接收的位数 (0-31)
volatile uint8_t EdgeCount = 0;    // 边沿计数，用于辅助判断引导码

void NEC_Init(void) {
  // 启动定时器基准 (已经在 tim.c 的 MX_TIM2_Init 中完成)
  HAL_TIM_Base_Start(HANDLE);
  // 启动 TIM2 输入捕获, 并启用中断 假设红外信号接在 TIM2_CH1
  HAL_TIM_IC_Start_IT(HANDLE, TIM_CHANNEL_1);
  // 设置初始捕获极性为上升沿捕获 (等待信号起始)
  __HAL_TIM_SET_CAPTUREPOLARITY(HANDLE, TIM_CHANNEL_1,
                                TIM_INPUTCHANNELPOLARITY_RISING);
  status = NEC_STATE_IDLE;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
    uint32_t CurrentCapture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    uint32_t Duration = 0;
    // 计算持续时间 (关键在于处理溢出)
    if (CurrentCapture > lastCapture) {
      Duration = CurrentCapture - lastCapture;
    } else {
      // 溢出处理 我设置TIM的Counter为Up模式最大值2 ^ 16(0xFFFF)
      Duration = (CurrentCapture + (0xFFFF - lastCapture));
    }
    switch (status) {
    case NEC_STATE_IDLE:
      // 首次捕获到上升沿, 清零计数器, 改变极性为下降沿检测, 计时高电平
      __HAL_TIM_SET_COUNTER(htim, 0);
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,
                                    TIM_INPUTCHANNELPOLARITY_FALLING);
      status = NEC_STATE_LEADER_HIGH;
      break;
    case NEC_STATE_LEADER_HIGH:
      if (Duration > NEC_LEADER_HIGH_MIN && Duration < NEC_LEADER_HIGH_MAX) {
        // 清零计数器, 并改变为上升沿检测, 计时低电平时间
        __HAL_TIM_SET_COUNTER(htim, 0);
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,
                                      TIM_INPUTCHANNELPOLARITY_RISING);
      } else {
        // 不满足9ms则复位
        goto NEC_Reset;
      }
      break;
    case NEC_STATE_LEADER_LOW:
      // 4.5ms 低电平校验通过，引导码完整
      if (Duration > NEC_LEADER_LOW_MIN && Duration < NEC_LEADER_LOW_MAX) {

        // 清零计数器, 准备采集第一个数据位的高电平
        __HAL_TIM_SET_COUNTER(htim, 0);
        // 切换为下降沿捕获 (等待 562.5us 高电平结束)
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,
                                      TIM_INPUTCHANNELPOLARITY_FALLING);
        // 进入数据采集状态
        status = NEC_STATE_CAPTURING;
        BitIndex = 0; // 确保位索引从 0 开始
      } else {
        // 不满足4.5ms则复位
        goto NEC_Reset;
      }

      break;
    case NEC_STATE_CAPTURING:
      if (!IR_IN) {
        // 如果是下降沿, 开始计时低电平时间
        __HAL_TIM_SET_COUNTER(htim, 0);
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,
                                      TIM_INPUTCHANNELPOLARITY_RISING);
      } else {
        // 上升沿, 处理得到的低电平时间
        if (Duration > NEC_LOGIC_1_LOW_MIN && Duration < NEC_LOGIC_1_LOW_MAX) {

          NecDataRaw |= (1U << BitIndex);
          BitIndex++;
        } else if (Duration > NEC_LOGIC_0_LOW_MIN &&
                   Duration < NEC_LOGIC_0_LOW_MAX) {
          NecDataRaw |= (0U << BitIndex);
          BitIndex++;
        } else {
          goto NEC_Reset;
        }
        // 切换为上升沿捕获等待下一个高电平
        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,
                                      TIM_INPUTCHANNELPOLARITY_RISING);
      }
      if (BitIndex >= 32) {
        // 32 位采集完成，跳转到处理函数
        Nec_Protocol_Received();
        // 不需要 break，执行完成会继续更新 LastCapture
      }
      break;
    }
    lastCapture = CurrentCapture; // 更新时间戳
    return;
  NEC_Reset:
    // 错误
    status = NEC_STATE_IDLE;
    BitIndex = 0;
    NecDataRaw = 0;
    // 重新配置为捕获第一个上升沿
    __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,
                                  TIM_INPUTCHANNELPOLARITY_RISING);
    lastCapture = 0;
    return;
  }
}

void Nec_Protocol_Received(void) {
  uint8_t addr = (NecDataRaw >> 0) & 0xFF;     // 地址
  uint8_t addr_inv = (NecDataRaw >> 8) & 0xFF; // 地址反码
  uint8_t cmd = (NecDataRaw >> 16) & 0xFF;     // 命令
  uint8_t cmd_inv = (NecDataRaw >> 24) & 0xFF; // 命令反码

  // 校验：地址 + 地址反码 = 0xFF 且 命令 + 命令反码 = 0xFF
  if ((addr + addr_inv) == 0xFF && (cmd + cmd_inv) == 0xFF) {
    // 校验通过
    NecResult.address_ = addr;
    NecResult.command_ = cmd;
    NecResult.isValid_ = 1;

    // 串口输出调试信息
    printf("NEC OK: Addr=0x%02X, Cmd=0x%02X\r\n", addr, cmd);
  } else {
    NecResult.isValid_ = 0;
    printf("NEC Checksum Failed!\r\n");
  }

  // 复位状态机
  NecDataRaw = 0;
  BitIndex = 0;
  status = NEC_IDLE;
}
