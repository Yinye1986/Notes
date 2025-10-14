#include "ST7796U.h"

ST7796U_OPTION devOpt;
uint16_t POINT_COLOR = 0x0000, BACK_COLOR = 0xFFFF;
uint16_t DeviceCode;

void GPIO_Init(void) {
  pinMode(ST7796U_RST, OUTPUT);
  pinMode(ST7796U_CS, OUTPUT);
  pinMode(ST7796U_RS, OUTPUT);
  pinMode(ST7796U_BL, OUTPUT);
}
void SPI_Init(void) {
  if (wiringPiSPISetup(SPI_CHANNEL0, SPI_SPEED0) < 0) {
    printf("SPI Setup failed\n");
    return;
  }
  printf("SPI Setup successful\n");
}
void ST7796U_Reset(void) {
  pinMode(ST7796U_RST, OUTPUT);
  digitalWrite(ST7796U_RST, LOW);
  delay(100);
  digitalWrite(ST7796U_RST, HIGH);
  delay(50);
}
void ST7796U_Init(void) {
  ST7796U_WriteCmd(0x11);
  delay(120); // Delay 120ms
  ST7796U_WriteCmd(0x36); // Memory Data Access Control MY,MX~~
  ST7796U_WriteData(0x48);
  ST7796U_WriteCmd(0x3A);
  ST7796U_WriteData(0x55); // ST7796U_WriteData(0x66);
  ST7796U_WriteCmd(0xF0); // Command Set Control
  ST7796U_WriteData(0xC3);
  ST7796U_WriteCmd(0xF0);
  ST7796U_WriteData(0x96);
  ST7796U_WriteCmd(0xB4);
  ST7796U_WriteData(0x01);
  ST7796U_WriteCmd(0xB7);
  ST7796U_WriteData(0xC6);
  // ST7796U_WriteCmd(0xB9);
  // ST7796U_WriteData(0x02);
  // ST7796U_WriteData(0xE0);
  ST7796U_WriteCmd(0xC0);
  ST7796U_WriteData(0x80);
  ST7796U_WriteData(0x45);
  ST7796U_WriteCmd(0xC1);
  ST7796U_WriteData(0x13); // 18  //00
  ST7796U_WriteCmd(0xC2);
  ST7796U_WriteData(0xA7);
  ST7796U_WriteCmd(0xC5);
  ST7796U_WriteData(0x0A);
  ST7796U_WriteCmd(0xE8);
  ST7796U_WriteData(0x40);
  ST7796U_WriteData(0x8A);
  ST7796U_WriteData(0x00);
  ST7796U_WriteData(0x00);
  ST7796U_WriteData(0x29);
  ST7796U_WriteData(0x19);
  ST7796U_WriteData(0xA5);
  ST7796U_WriteData(0x33);
  ST7796U_WriteCmd(0xE0);
  ST7796U_WriteData(0xD0);
  ST7796U_WriteData(0x08);
  ST7796U_WriteData(0x0F);
  ST7796U_WriteData(0x06);
  ST7796U_WriteData(0x06);
  ST7796U_WriteData(0x33);
  ST7796U_WriteData(0x30);
  ST7796U_WriteData(0x33);
  ST7796U_WriteData(0x47);
  ST7796U_WriteData(0x17);
  ST7796U_WriteData(0x13);
  ST7796U_WriteData(0x13);
  ST7796U_WriteData(0x2B);
  ST7796U_WriteData(0x31);
  ST7796U_WriteCmd(0xE1);
  ST7796U_WriteData(0xD0);
  ST7796U_WriteData(0x0A);
  ST7796U_WriteData(0x11);
  ST7796U_WriteData(0x0B);
  ST7796U_WriteData(0x09);
  ST7796U_WriteData(0x07);
  ST7796U_WriteData(0x2F);
  ST7796U_WriteData(0x33);
  ST7796U_WriteData(0x47);
  ST7796U_WriteData(0x38);
  ST7796U_WriteData(0x15);
  ST7796U_WriteData(0x16);
  ST7796U_WriteData(0x2C);
  ST7796U_WriteData(0x32);
  ST7796U_WriteCmd(0xF0);
  ST7796U_WriteData(0x3C);
  ST7796U_WriteCmd(0xF0);
  ST7796U_WriteData(0x69);
  delay(120);
  ST7796U_WriteCmd(0x21);
  ST7796U_WriteCmd(0x29);
}
void LCD_Clear(uint16_t Color) {
  unsigned int i, m;
  LCD_SetWindows(0, 0, devOpt.width - 1, devOpt.height - 1);
  digitalWrite(ST7796U_CS, LOW);
  digitalWrite(ST7796U_RS, HIGH);
  for (i = 0; i < devOpt.height; i++) {
    for (m = 0; m < devOpt.width; m++) {
      SPI_SendByte(Color >> 8);
      SPI_SendByte(Color);
    }
  }
  digitalWrite(ST7796U_CS, HIGH);
}
void LCD_Init(void) {
  SPI_Init();      // 硬件SPI1初始化
  GPIO_Init();     // LCD GPIO初始化
  ST7796U_Reset(); // LCD 复位
  //*************3.5 ST7796S IPS初始化**********//
  ST7796U_Init();
  LCD_direction(USE_HORIZONTAL); // 设置LCD显示方向
  LCD_Clear(WHITE);              // 清全屏白色
}

void LCD_BLCTL(uint8_t cmd) {
  if (cmd == 1)
    digitalWrite(ST7796U_BL, HIGH);
  else
    digitalWrite(ST7796U_BL, LOW);
}

void SPI_SendByte(uint8_t data) {
  uint8_t buffer[1]= {data};
  wiringPiSPIDataRW(SPI_CHANNEL0, buffer, 1); // 发送一个字节并接收一个字节
}
uint8_t SPI_ReceiveByte(uint8_t data) {
  uint8_t buffer[1] = {data}; // 创建一个长度为2的缓冲区
  // 发送一个字节并接收一个字节
  wiringPiSPIDataRW(SPI_CHANNEL0, buffer, 1);
  // buffer[1] 现在包含从设备返回的数据
  return buffer[0];
}

void ST7796U_WriteCmd(uint8_t data) {
  digitalWrite(ST7796U_CS, LOW);
  digitalWrite(ST7796U_RS, LOW);
  SPI_SendByte(data);
  digitalWrite(ST7796U_CS, HIGH);
}
void ST7796U_WriteData(uint8_t data) {
  digitalWrite(ST7796U_CS, LOW);
  digitalWrite(ST7796U_RS, HIGH);
  SPI_SendByte(data);
  digitalWrite(ST7796U_CS, HIGH);
}
void ST7796U_WriteData16b(uint16_t data) {
    digitalWrite(ST7796U_CS, LOW);
    digitalWrite(ST7796U_RS, HIGH);
    SPI_SendByte(data >> 8);
    SPI_SendByte(data);
    digitalWrite(ST7796U_CS, HIGH);
}
uint8_t ST7796U_ReadData(void) {
  uint8_t data;
  digitalWrite(ST7796U_CS, LOW);
  digitalWrite(ST7796U_RS, HIGH);
  data = SPI_ReceiveByte(0xFF);
  digitalWrite(ST7796U_CS, HIGH);
  return data;
}
uint16_t ST7796U_ReadData16b(void) {
  uint16_t r, g;
  digitalWrite(ST7796U_CS, LOW);
  digitalWrite(ST7796U_RS, LOW);
  SPI_SendByte(devOpt.rramcmd);
  digitalWrite(ST7796U_RS, HIGH);
  SPI_SendByte(0xFF);
  r = SPI_ReceiveByte(0xFF);
  g = SPI_ReceiveByte(0xFF);
  digitalWrite(ST7796U_CS, HIGH);
  r <<= 8;
  r |= g;
  return r;
}





void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue) {
  ST7796U_WriteCmd(LCD_Reg);
  ST7796U_WriteData(LCD_RegValue);
}
uint8_t LCD_ReadReg(uint8_t LCD_Reg) {
  ST7796U_WriteCmd(LCD_Reg);
  return ST7796U_ReadData();
}

void LCD_WriteRAM_Prepare(void) { ST7796U_WriteCmd(devOpt.wramcmd); }

void LCD_ReadRAM_Prepare(void) { ST7796U_WriteCmd(devOpt.rramcmd); }


/*****************************************************************************
 * @name       :void LCD_DrawPoint(uint16_t x,uint16_t y)
 * @date       :2018-08-09
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/
void LCD_DrawPoint(uint16_t x, uint16_t y) {
  LCD_SetCursor(x, y); // 设置光标位置
  ST7796U_WriteData16b(POINT_COLOR);
}

uint16_t LCD_ReadPoint(uint16_t x, uint16_t y) {
  uint16_t color;
  LCD_SetCursor(x, y); // 设置光标位置
  color = ST7796U_ReadData16b();
  return color;
}
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd,
                    uint16_t yEnd) {
  ST7796U_WriteCmd(devOpt.setxcmd);
  ST7796U_WriteData(xStar >> 8);
  ST7796U_WriteData(0x00FF & xStar);
  ST7796U_WriteData(xEnd >> 8);
  ST7796U_WriteData(0x00FF & xEnd);

  ST7796U_WriteCmd(devOpt.setycmd);
  ST7796U_WriteData(yStar >> 8);
  ST7796U_WriteData(0x00FF & yStar);
  ST7796U_WriteData(yEnd >> 8);
  ST7796U_WriteData(0x00FF & yEnd);

  LCD_WriteRAM_Prepare(); // 开始写入GRAM
}

void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos) {
  LCD_SetWindows(Xpos, Ypos, Xpos, Ypos);
}

/*****************************************************************************
 * @name       :void LCD_direction(uint8_t direction)
 * @date       :2018-08-09
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
                                                                                                        2-180 degree
                                                                                                        3-270 degree
 * @retvalue   :None
******************************************************************************/
void LCD_direction(uint8_t direction) {
  devOpt.setxcmd = 0x2A;
  devOpt.setycmd = 0x2B;
  devOpt.wramcmd = 0x2C;
  devOpt.rramcmd = 0x2E;
  devOpt.dir = direction % 4;
  switch (devOpt.dir) {
  case 0:
    devOpt.width = LCD_WIDTH;
    devOpt.height = LCD_HEIGHT;
    LCD_WriteReg(0x36, (1 << 3) | (1 << 6));
    break;
  case 1:
    devOpt.width = LCD_HEIGHT;
    devOpt.height = LCD_WIDTH;
    LCD_WriteReg(0x36, (1 << 3) | (1 << 5));
    break;
  case 2:
    devOpt.width = LCD_WIDTH;
    devOpt.height = LCD_HEIGHT;
    LCD_WriteReg(0x36, (1 << 3) | (1 << 7));
    break;
  case 3:
    devOpt.width = LCD_HEIGHT;
    devOpt.height = LCD_WIDTH;
    LCD_WriteReg(0x36, (1 << 3) | (1 << 7) | (1 << 6) | (1 << 5));
    break;
  default:
    break;
  }
}

uint16_t LCD_Read_ID(void) {
  uint8_t i, val[3] = {0};
  ST7796U_WriteCmd(0xF0); // Command Set Control
  ST7796U_WriteData(0xC3);

  ST7796U_WriteCmd(0xF0);
  ST7796U_WriteData(0x96);
  digitalWrite(ST7796U_CS, LOW);
  for (i = 1; i < 4; i++) {
    digitalWrite(ST7796U_RS, LOW);
    SPI_SendByte(0xFB);
    digitalWrite(ST7796U_RS, HIGH);
    SPI_SendByte(0x10 + i);
    digitalWrite(ST7796U_RS, LOW);
    SPI_SendByte(0xD3);
    digitalWrite(ST7796U_RS, HIGH);
    val[i - 1] = SPI_ReceiveByte(0xFF);
    digitalWrite(ST7796U_RS, LOW);
    SPI_SendByte(0xFB);
    digitalWrite(ST7796U_RS, HIGH);
    SPI_SendByte(0x00);
  }
  digitalWrite(ST7796U_CS, HIGH);
  ST7796U_WriteCmd(0xF0); // Command Set Control
  ST7796U_WriteData(0x3C);
  ST7796U_WriteCmd(0xF0);
  ST7796U_WriteData(0x69);
  devOpt.id = val[1];
  devOpt.id <<= 8;
  devOpt.id |= val[2];
  return devOpt.id;
}
