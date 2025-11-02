# 配置定时器
使用内部时钟
分频器设为72-1, 得到1MHz的频率, 1微秒精度
开启定时器中断

主函数中启动 TIM2 定时器的时间基准
```c
HAL_TIM_Base_Start(&htim2);
```

紧接着启动 TIM2 定时器的输入捕获功能, 并启用中断
```c
HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
```

重写中断回调函数
