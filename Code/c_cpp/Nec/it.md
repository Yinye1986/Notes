在中断中

```c
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_0) {
        if (HAL_GPIO_ReadPin(IR_RX_GPIO_Port, IR_RX_Pin) == GPIO_PIN_SET) {
            // 上升沿：space 结束，读取宽度
            uint32_t now = __HAL_TIM_GET_COUNTER(&htim2);
            uint32_t width = now - last_edge_time;
            last_edge_time = now;

            // 处理这个 width（关键！）
            process_ir_pulse(width);
        }
    }
}
```
