#ifndef __NEC_H__
#define __NEC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "tim.h"
#include <stdbool.h>

#define IR_IN HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)
#define HANDLE &htim2
#define CHANNEL TIM_CHANNEL_1

typedef enum {
    NEC_STATE_IDLE = 0,
    NEC_STATE_LEADER_HIGH,
    NEC_STATE_LEADER_LOW,
    NEC_STATE_CAPTURING_HIGH,
    NEC_STATE_CAPTURING_LOW,
} NEC_StatusTypeDef;

typedef struct {
    uint8_t address_;
    uint8_t command_;
    uint8_t isValid_;      // 1: 校验通过
} NecResult_t;

// 全局变量声明
extern NecResult_t NecResult;

void NEC_Init(void);
void Nec_Protocol_Received(void); // 数据处理函数

#ifdef __cplusplus
}
#endif

#endif