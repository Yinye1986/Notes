#ifndef BUTTON_H
#define BUTTON_H


#include "stm32f1xx.h"
#include <stdbool.h>

#define DEBOUNCE_TIME 19
#define LONG_PRESS_TIME 1500
#define DOUBLE_CLICK_THRESHOLD 100

typedef enum {
  KEY_STATE_IDLE,       // 闲置
  KEY_STATE_DEBOUNCING, // 消除抖动ing
  KEY_STATE_SINGLE_CLICK, // 单击达成
  KEY_STATE_PRESSED,
  KEY_STATE_WAIT,
  KEY_STATE_LONEPRESS,
  KEY_STATE_DOUBLE_CLICK,
} KeyState_t;

typedef struct {
  GPIO_TypeDef *GPIOx;
  uint16_t GPIO_Pin;
  KeyState_t CURR_State;
  uint32_t time; // 上一个状态达成的时间点
} Button_t;

extern Button_t BUTTON_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, bool isPullupButton);
extern void BUTTON_Sync(Button_t *handle);
extern bool BUTTON_Check(Button_t *handle, KeyState_t state);

#endif // BUTTON_H
