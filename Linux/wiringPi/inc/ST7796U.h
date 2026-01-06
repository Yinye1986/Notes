#ifndef __ST7796U_H__
#define __ST7796U_H__

#include <stdint.h>
#include <stdio.h>
#include "wiringPi.h"
#include "wiringPiSPI.h"

#define SPI_CHANNEL0 0
#define SPI_CHANNEL1 1
#define SPI_SPEED0 16000000
#define SPI_SPEED1 4500000

#define ST7796U_CS 0
#define ST7796U_RS 0
#define ST7796U_BL 0
#define ST7796U_RST 0

#define LCD_HEIGHT 480
#define LCD_WIDTH 320

typedef struct {										    
    uint16_t width;			//LCD 宽度
	uint16_t height;		//LCD 高度
	uint16_t id;			//LCD ID
	uint8_t  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。
	uint16_t wramcmd;		//开始写gram指令
	uint16_t rramcmd;       //开始读gram指令
	uint16_t setxcmd;		//设置x坐标指令
	uint16_t setycmd;		//设置y坐标指令
} ST7796U_OPTION; 	

//LCD参数
extern ST7796U_OPTION devOpt;	//管理LCD重要参数
/////////////////////////////////////用户配置区///////////////////////////////////	 
#define USE_HORIZONTAL  	 0//定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转

//////////////////////////////////////////////////////////////////////////////////	  
//定义LCD的尺寸

//TFTLCD部分外要调用的函数		   
extern uint16_t  POINT_COLOR;//默认红色    
extern uint16_t  BACK_COLOR; //背景颜色.默认为白色

////////////////////////////////////////////////////////////////////

//画笔颜色
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 		0XFFE0
#define GBLUE	    0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 		0XBC40 //棕色
#define BRRED 		0XFC07 //棕红色
#define GRAY  		0X8430 //灰色

//GUI颜色
#define DARKBLUE    0X01CF	//深蓝色
#define LIGHTBLUE   0X7D7C	//浅蓝色  
#define GRAYBLUE    0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN  0X841F //浅绿色
#define LIGHTGRAY   0XEF5B //浅灰色(PANNEL)
#define LGRAY 		0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE   0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE      0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
void GPIO_Init(void);
void SPI_Init(void);
void ST7796U_Init(void);
void ST7796U_Reset(void);
void LCD_direction(uint8_t direction );
void LCD_Clear(uint16_t Color);	 
void LCD_Init(void);

void SPI_SendByte(uint8_t data);
uint8_t SPI_ReceiveByte(uint8_t data);

void ST7796U_WriteCmd(uint8_t data);
void ST7796U_WriteData(uint8_t data);   
void ST7796U_WriteData16b(uint16_t data);
uint8_t ST7796U_ReadData(void);
uint16_t ST7796U_ReadData16b(void);


void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
uint8_t LCD_ReadReg(uint8_t LCD_Reg);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_DrawPoint(uint16_t x,uint16_t y);//画点
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y); //读点
                                                //
void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);

uint16_t LCD_Read_ID(void);			  		 
void LCD_BLCLT(uint8_t cmd);

#endif  
