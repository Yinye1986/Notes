#include "SSD1315.h"

int OLED_FD;
uint8_t OLED_BUFFER[8][128];

void iicInit(void) {
    OLED_FD = wiringPiI2CSetup(0x3c);
}

void oledInit(void) {
    oledCmd(0xAE);
    oledCmd(0x20);
    oledCmd(0x10);	
    oledCmd(0xb0);
    oledCmd(0xc8);	
    oledCmd(0x00);
    oledCmd(0x10);	
    oledCmd(0x40);
    oledCmd(0x81);
    oledCmd(0xff);
    oledCmd(0xa1);
    oledCmd(0xa6);
    oledCmd(0xa8);
    oledCmd(0x3f);	 
    oledCmd(0xa4);	
    oledCmd(0xd3);
    oledCmd(0x00);
    oledCmd(0xd5);
    oledCmd(0xf0);
    oledCmd(0xd9);
    oledCmd(0x22);
    oledCmd(0xda);	 
    oledCmd(0x12);
    oledCmd(0xdb);	
    oledCmd(0x20);	
    oledCmd(0x8d);	
    oledCmd(0x14);	
    oledCmd(0xaf);	
    oledClear();
    oledUpdate();
}

void oledCmd(uint8_t cmd) {
    wiringPiI2CWriteReg8(OLED_FD, 0x00, cmd);
}

void oledData(uint8_t *Data, uint8_t Count) {
	for (uint8_t i = 0; i < Count; i ++)
	{
        wiringPiI2CWriteReg8(OLED_FD, 0x40, Data[i]);
	}
}

void oledSetcursor(uint8_t Page, uint8_t X) {
	oledCmd(0xB0 | Page);
	oledCmd(0x10 | ((X & 0xF0) >> 4));
	oledCmd(0x00 | (X & 0x0F));
}

void oledUpdate(void) {
	for (uint8_t i=0; i<8; i++) {
		oledSetcursor(i, 0);
		oledData(OLED_BUFFER[i], 128);
	}
}

void oledUpdateArea(uint8_t y, uint8_t x, uint8_t width, uint8_t height) {
	if (x > 127) {return;}
	if (y > 63) {return;}
	if (x + width > 128) {width = 128 - x;}
	if (y + height > 64) {height = 64 - y;}
	for (uint8_t i=y/8; i<(y+height-1)/8+1; i++)
	{
		oledSetcursor(i, x);
		oledData(&OLED_BUFFER[i][x], width);
	}
}

void oledClear(void) {
	for (uint8_t i = 0; i < 8; i++) {
		for (uint8_t j = 0; j < 128; j++) {
			OLED_BUFFER[i][j] = 0x00;
		}
	}
}

void oledClearArea(uint8_t y, uint8_t x, uint8_t width, uint8_t height) {
	uint8_t i, j;
	if (x > 127) {return;}
	if (y > 63) {return;}
	if (x + width > 128) {width = 128 - x;}
	if (y + height > 64) {height = 64 - y;}
	
	for (j = y; j < y + height; j ++)		//遍历指定页
	{
		for (i = x; i < x + width; i ++)	//遍历指定列
		{
			OLED_BUFFER[j / 8][i] &= ~(0x01 << (j % 8));	//将显存数组指定数据清零
		}
	}
}




















void oledShowImage(uint8_t y, uint8_t x, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
	uint8_t i, j;
	if (y > 127) {return;}
	if (x > 63) {return;}
	oledClearArea(y, x, Width, Height);
	for (j = 0; j < (Height - 1) / 8 + 1; j ++)
	{
		for (i = 0; i < Width; i ++)
		{
			if (y + i > 127) {break;}
			if (x / 8 + j > 7) {return;}
			OLED_BUFFER[x / 8 + j][y + i] |= Image[j * Width + i] << (x % 8);
			if (x / 8 + j + 1 > 7) {continue;}
			OLED_BUFFER[x / 8 + j + 1][y + i] |= Image[j * Width + i] >> (8 - x % 8);
		}
	}
}

void oledShow16x8(uint8_t y, uint8_t x, char Char) {
    oledShowImage(y, x, 8, 16, OLED_F8x16[Char - ' ']);
    oledUpdateArea(y ,x ,8 ,16);
}

void oledShow8x6(uint8_t y, uint8_t x, char Char) {
    oledShowImage(y, x, 6, 8, OLED_F6x8[Char - ' ']);
    oledUpdateArea(y ,x ,6 ,8);
}
