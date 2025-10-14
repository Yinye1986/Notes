/* buzzer.c */
#include "buzzer.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

static GPIO_TypeDef* BUZZER_GPIO;
static uint16_t BUZZER_PIN;
static volatile uint32_t alarm_duration = 0;
static BuzzerCallback user_callback = NULL;

void TIM_IRQHandler(void) {
    static uint32_t counter = 0;
    
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        
        if (++counter >= alarm_duration / 1000) {
            Buzzer_Stop();
            counter = 0;
            if(user_callback) user_callback();
        }
    }
}

void Buzzer_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    // GPIO初始化
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    BUZZER_GPIO = GPIOx;
    BUZZER_PIN = GPIO_Pin;
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx, &GPIO_InitStruct);

    // 定时器初始化
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    TIM_InitStruct.TIM_Prescaler = 7200 - 1;  // 72MHz/7200=10kHz
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStruct.TIM_Period = 10000 - 1;    // 10kHz/10000=1秒
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);

    // 中断配置
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, DISABLE);
}

void Buzzer_Start(uint32_t duration_ms) {
    alarm_duration = duration_ms;
    GPIO_SetBits(BUZZER_GPIO, BUZZER_PIN);
    TIM_Cmd(TIM2, ENABLE);
}

void Buzzer_Stop(void) {
    TIM_Cmd(TIM2, DISABLE);
    GPIO_ResetBits(BUZZER_GPIO, BUZZER_PIN);
    TIM_SetCounter(TIM2, 0);
}

void Buzzer_RegisterInterrupt(BuzzerCallback callback) {
    user_callback = callback;
}
