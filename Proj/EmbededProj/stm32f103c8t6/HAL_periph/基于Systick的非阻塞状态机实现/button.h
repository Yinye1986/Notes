#ifndef BUTTON_H
#define BUTTON_H


#include "stm32f1xx.h"
#include <stdbool.h>

#define DEBOUNCE_TIME 20
#define RELEASE_DEBOUNCE_TIME 10

typedef enum {
  KEY_STATE_IDLE,
  KEY_STATE_DEBOUNCING,
  KEY_STATE_SINGLE_CLICK,
  KEY_STATE_PRESSED,
} KeyState_t;

typedef struct {
  GPIO_TypeDef *GPIOx;
  uint16_t GPIO_Pin;
  KeyState_t CURR_State;
  uint32_t time;
} Button_t;

extern Button_t BUTTON_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, bool isPullupButton);
extern void BUTTON_Sync(Button_t *handle);
extern bool BUTTON_Check(Button_t *handle, KeyState_t state);

#endif // BUTTON_H
