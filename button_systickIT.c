#include "button_systickIT.h"
#include "stm32f1xx_hal_gpio.h"
#include <stdbool.h>

extern Button_t BUTTON_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,
                            bool is_pullup_button) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  if (is_pullup_button) {
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  } else {
    GPIO_InitStruct.Pull = GPIO_PULLUP;
  }
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
  Button_t btn = {
      .port_ = GPIOx,
      .pin_ = GPIO_Pin,
      .state_ = KEY_STATE_IDLE,
      .counter_ = 0,
      .is_pullup_button_ = is_pullup_button,
  };
  return btn;
}

extern void BUTTON_Scan() {
  extern Button_t *btn;
  bool level = HAL_GPIO_ReadPin(btn->port_, btn->pin_);
  if (level == btn->is_pullup_button_) {
    btn->counter_++;
    if (btn->counter_ >= BUTTON_LONGPRESS_THRESHOLD) {
      btn->state_ = KEY_STATE_LONGPRESS;
      btn->counter_ = 0;
    }
  } else if (btn->counter_ >= BUTTON_SINGLECLICK_THRESHOLD) {
    btn->state_ = KEY_STATE_SINGLE_CLICK;
    btn->counter_ = 0;
  } else {
    btn->counter_ = 0;
  }
}

// void HAL_SYSTICK_Callback(void) {
//   static uint8_t counter = 0;
//   counter++;
//   if (counter >= 500) {
//     counter = 0;
//       HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//   }
// }

extern bool BUTTON_Check(Button_t *btn, ButtonState_t state) {
  if (btn->state_ == state) {
    btn->state_ = KEY_STATE_IDLE;
    return 1;
  } else {
    return 0;
  }
}
