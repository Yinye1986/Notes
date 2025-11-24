# Modbus Slave 代码片段
```c
if (uart1RxBuf[0] == 0x01 && uart1RxBuf[1] == 0x05) {
  uint8_t address_low = uart1RxBuf[3];
  if (address_low >= 0 && address_low <= 7) {
    uint16_t target_pin = (uint16_t)(1 << address_low);
    GPIO_PinState pin_state;
    if (uart1RxBuf[4] == 0xFF && uart1RxBuf[5] == 0x00) {
      pin_state = GPIO_PIN_RESET;
    } else if (uart1RxBuf[4] == 0x00 && uart1RxBuf[5] == 0x00) {
      pin_state = GPIO_PIN_SET;
    }
    HAL_GPIO_WritePin(GPIOE, target_pin, pin_state);
    uint8_t return_cmd[6];
    for (int i = 0; i < 6; i++) {
      return_cmd[i] = uart1RxBuf[i];
    }
    HAL_UART_Transmit(&huart1, return_cmd, 6, 0xFFFF);
  }
} else if (uart1RxBuf[0] == 0x01 && uart1RxBuf[1] == 0x0F) {
  uint8_t tmp = uart1RxBuf[6];
  for (int i = 7; i > 0; i--) {
    if ((tmp >> i) & 0x01) {
      uint16_t target_pin = (uint16_t)(1 << i);
      HAL_GPIO_WritePin(GPIOE, target_pin, GPIO_PIN_RESET);
    }
  }
} else if (uart1RxBuf[0] == 0x01 && uart1RxBuf[1] == 0x03) {
  uint16_t address = (uart1RxBuf[2] << 8) | uart1RxBuf[3];
  uint8_t byteQuantity = (uart1RxBuf[4] << 8) | uart1RxBuf[5];
  uint8_t return_cmd[3 + byteQuantity];
  return_cmd[0] = uart1RxBuf[0];
  return_cmd[1] = uart1RxBuf[1];
  return_cmd[2] = byteQuantity;
  for (uint8_t i = 0; i < byteQuantity / 2; i++) {
    uint16_t addr = address + i;
    return_cmd[3 + i * 2] = regs[addr - 0xE001] >> 8;   // 高位字节
    return_cmd[4 + i * 2] = regs[addr - 0xE001] & 0xFF; // 低位字节
  }
  HAL_UART_Transmit(&huart1, return_cmd, 3 + byteQuantity, 0xFFFF);
} else if (uart1RxBuf[0] == 0x01 && uart1RxBuf[1] == 0x06) {
  uint15_t address = (uart1RxBuf[2] << 8) | uart1RxBuf[3];
  uint16_t data = (uart1RxBuf[4] << 8) | uart1RxBuf[5];
  regs[addr - 0xE001] = data;
  uint8_t return_cmd[6];
  memcpy(return_cmd, uart1RxBuf, 6);
  HAL_UART_Transmit(&huart1, return_cmd, 6, 0xFFFF);
} else if (uart1RxBuf[0] == 0x01 && uart1RxBuf[1] == 0x10) {
  uint16_t address = (uart1RxBuf[2] << 8) | uart1RxBuf[3];
  uint8_t regQuantity = (uart1RxBuf[4] << 8) | uart1RxBuf[5];
  uint8_t byteQuantity = regQuantity * 2;
  for (uint8_t i = 0; i < regQuantity; i++) {
    uint16_t addr = address + i;
    regs[addr - 0xE001] = (uart1RxBuf[7 + i * 2] << 8) | uart1RxBuf[8 + i * 2];
  }
  uint8_t return_cmd[6];
  memcpy(return_cmd, uart1RxBuf, 6);
  HAL_UART_Transmit(&huart1, return_cmd, 6, 0xFFFF);
} else if (uart1RxBuf[0] == 0x01 && uart1RxBuf[1] == 0x02) {
  uint8_t quantity = (uart1RxBuf[4] << 8) | uart1RxBuf[5];
  uint8_t return_cmd[3 + quantity / 8];
  return_cmd[0] = uart1RxBuf[0];
  return_cmd[1] = uart1RxBuf[1];
  return_cmd[2] = uart1RxBuf[2];
  return_cmd[3] = 0x00;
  // ...
} else if (uart1RxBuf[0] == 0x01 && uart1RxBuf[1] == 0x01) {
}
```
