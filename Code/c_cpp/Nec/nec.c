#include "nec.h"

volatile ir_state_t ir_state = IR_STATE_IDLE;
volatile uint8_t ir_bit_count = 0;
volatile uint32_t ir_data = 0; // 32位暂存 addr+cmd
volatile bool ir_frame_ready = false;

void process_ir_pulse(uint32_t width) {
// 容差：±200us
#define TOL 200
#define IS_HEADER_MARK(w) ((w) > (9000 - TOL) && (w) < (9000 + TOL))
#define IS_HEADER_SPACE(w) ((w) > (4500 - TOL) && (w) < (4500 + TOL))
#define IS_BIT_0(w) ((w) > (562 - TOL) && (w) < (562 + TOL))
#define IS_BIT_1(w) ((w) > (1688 - TOL) && (w) < (1688 + TOL))

  switch (ir_state) {
  case IR_STATE_IDLE:
    if (IS_HEADER_MARK(width)) {
      ir_state = IR_STATE_HEADER;
    }
    break;
  case IR_STATE_HEADER:
    if (IS_HEADER_SPACE(width)) {
      // 引导码正确，准备接收数据
      ir_bit_count = 0;
      ir_data = 0;
      ir_state = IR_STATE_DATA;
    } else {
      ir_state = IR_STATE_IDLE; // 失败，重置
    }
    break;
  case IR_STATE_DATA:
    uint8_t bit_val = 0;
    if (IS_BIT_1(width)) {
      bit_val = 1;
    } else if (IS_BIT_0(width)) {
      bit_val = 0;
    } else {
      // 无效脉冲（噪声？），丢弃整帧
      ir_state = IR_STATE_IDLE;
      return;
    }

    // 👇 关键：逐位重组（LSB first）
    if (ir_bit_count < 32) {
      ir_data |= ((uint32_t)bit_val << ir_bit_count);
    }
    ir_bit_count++;

    // 收满 32 位？
    if (ir_bit_count == 32) {
      ir_state = IR_STATE_DONE;
      ir_frame_ready = true; // 通知主循环
    }
    break;
  case IR_STATE_DONE:
    // 可选：检查重复码（110ms space）
    ir_state = IR_STATE_IDLE;
    break;
  }
}
