#ifndef __OLED_H
#define __OLED_H

#include "bsp_oled.h"
#include "debug.h"
#include "stdlib.h"	 
#include "common.h"

#ifdef GZ_DEBUG
    #define Oled_TAG    "oled"
#else
    #define Oled_TAG    ""
#endif

//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
#define SIZE 16     //显示字体选择
#define XLevelL		0x00
#define XLevelH		0x10
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64
#define Max_Column	X_WIDTH
#define Max_Row		Y_WIDTH

void Oled_Init(void);
void Oled_Display_On(void);
void Oled_Display_Off(void);
void Oled_Set_Pos(uint8_t x, uint8_t y);
void Oled_Clear(void);
void Oled_Show_Char(uint8_t x, uint8_t y, uint8_t str);
void Oled_Show_Num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
void Oled_Show_String(uint8_t x, uint8_t y, uint8_t *str);	 
void Oled_Show_Chinese(uint8_t x, uint8_t y, uint8_t no);
void Oled_Show_Bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, byte prtBmp[]);

#endif
