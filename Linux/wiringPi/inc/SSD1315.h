#ifndef __SSD1315_H__
#define __SSD1315_H__


#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdint.h>
#include "SSD1315_DATA.h"

extern int OLED_FD;

extern uint8_t OLED_BUFFER[8][128];

void iicInit(void);
void oledInit(void);
void oledCmd(uint8_t cmd);
void oledData(uint8_t *Data, uint8_t Count);
void oledSetcursor(uint8_t Page, uint8_t X);
void oledUpdate(void);
void oledUpdateArea(uint8_t y, uint8_t x, uint8_t width, uint8_t height);
void oledClear(void);
void oledClearArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);
void oledShowImage(uint8_t y, uint8_t x, uint8_t Width, uint8_t Height, const uint8_t *Image);
void oledShow16x8(uint8_t y, uint8_t x, char Char);
void oledShow8x6(uint8_t y, uint8_t x, char Char);

#endif
