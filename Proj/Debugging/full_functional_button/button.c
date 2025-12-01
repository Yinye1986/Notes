#include "button.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal_gpio.h"

Button_t BUTTON_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,
                     bool isPullupButton) {
  Button_t btn = {0};
  btn.GPIOx = GPIOx;
  btn.GPIO_Pin = GPIO_Pin;
  btn.CURR_State = KEY_STATE_IDLE;
  btn.time = 0;
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = btn.GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  if (isPullupButton) {
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  } else {
    GPIO_InitStruct.Pull = GPIO_PULLUP;
  }
  HAL_GPIO_Init(btn.GPIOx, &GPIO_InitStruct);
  return btn;
}

void BUTTON_Sync(Button_t *btn) {
  uint32_t current_time = HAL_GetTick();
  bool isPressed = HAL_GPIO_ReadPin(btn->GPIOx, btn->GPIO_Pin);

  switch (btn->CURR_State) {
  case KEY_STATE_IDLE:
    if (isPressed) {
      btn->CURR_State = KEY_STATE_DEBOUNCING;
      btn->time = current_time;
    }
    break;
  case KEY_STATE_DEBOUNCING:
    if (!isPressed) {
      btn->CURR_State = KEY_STATE_IDLE;
    } else {
      if (current_time - btn->time >= DEBOUNCE_TIME) {
        btn->CURR_State = KEY_STATE_SINGLE_CLICK;
        btn->time = current_time;
      }
    }
    break;
  case KEY_STATE_SINGLE_CLICK:
    if (isPressed) {
      btn->CURR_State = KEY_STATE_PRESSED;
    } else {
      btn->CURR_State = KEY_STATE_IDLE;
    }
    break;

  case KEY_STATE_PRESSED:
    if (!isPressed) {
      btn->CURR_State = KEY_STATE_WAIT;
      btn->time = current_time;
    } else {
      if (current_time - btn->time >= LONG_PRESS_TIME) {
        btn->CURR_State = KEY_STATE_LONEPRESS;
        btn->time = current_time;
      }
    }
    break;
  case KEY_STATE_WAIT:
    if (isPressed) {
      if (current_time - btn->time <= DOUBLE_CLICK_THRESHOLD) {
        btn->CURR_State = KEY_STATE_DOUBLE_CLICK;
      }
    } else {
      btn->CURR_State = KEY_STATE_IDLE;
    }
    break;
  case KEY_STATE_DOUBLE_CLICK:
    btn->CURR_State = KEY_STATE_IDLE;
    break;
  case KEY_STATE_LONEPRESS:
    if (isPressed) {
      btn->CURR_State = KEY_STATE_LONEPRESS;
    } else {
      btn->CURR_State = KEY_STATE_IDLE;
    }
    break;
  }
}

bool BUTTON_Check(Button_t *handle, KeyState_t state) {
  if (handle->CURR_State == state) {
    return true;
  } else {
    return false;
  }
}
