#ifndef NEC_H
#define NEC_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    IR_STATE_IDLE,      // 等待引导码
    IR_STATE_HEADER,    // 已收到 9ms mark，等待 4.5ms space
    IR_STATE_DATA,      // 正在接收 32 位数据
    IR_STATE_DONE       // 帧完成
} ir_state_t;

volatile ir_state_t ir_state;
volatile uint8_t ir_bit_count;
volatile uint32_t ir_data; // 32位暂存（addr+cmd）
volatile bool ir_frame_ready;
void process_ir_pulse(uint32_t width);

#endif // NEC_H
