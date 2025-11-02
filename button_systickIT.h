#ifndef BUTTON_H
#define BUTTON_H

#include "stm32f1xx.h"
#include <stdbool.h>
#include <stdint.h>

// 在定时器中断回调函数中, 完成按钮的检测并返回状态到按钮描述符中
// 通过BUTTON_Check()消费机制, 在执行按钮回调时, 置状态为IDLE, 释放按钮状态
// 在5ms一次的定时器中断中还需要三次消抖计数器的累加, 才判定按钮状态为单击

#define BUTTON_SCAN_CYCLE_MS 5
#define BUTTON_SINGLECLICK_THRESHOLD 3
#define BUTTON_LONGPRESS_THRESHOLD 100

typedef enum {
  KEY_STATE_IDLE,
  KEY_STATE_SINGLE_CLICK,
  KEY_STATE_WAIT,
  KEY_STATE_LONGPRESS,
} ButtonState_t;

typedef struct {
  GPIO_TypeDef *port_;
  uint16_t pin_;
  bool is_pullup_button_;
  uint8_t counter_;
  ButtonState_t state_;
} Button_t;

extern Button_t BUTTON_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, bool is_pullup_button);

extern void BUTTON_Scan();
extern bool BUTTON_Check(Button_t *btn, ButtonState_t state);
extern void HAL_SYSTICK_Callback(void);

#endif // BUTTON_H
