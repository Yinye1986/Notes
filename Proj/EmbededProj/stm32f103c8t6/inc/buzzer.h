#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f10x.h"
#include <stddef.h>

typedef void (*BuzzerCallback)(void);

void Buzzer_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Buzzer_Start(uint32_t duration_ms);
void Buzzer_Stop(void);
void Buzzer_RegisterInterrupt(BuzzerCallback callback);

#endif
