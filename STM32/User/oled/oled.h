#ifndef __OLED_H
#define __OLED_H			  	 

#include "stm32f10x.h"
#include "stdlib.h"	   
#include "debug.h"

#ifdef GZ_DEBUG
	#define	Bsp_Oled_TAG 				"bsp_oled"
#else 
	#define	Bsp_Oled_TAG 				""
#endif 	

#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    

//-----------------OLED----------------  					   


#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_0)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_0)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_1)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_1)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_4)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


void Oled_WR_Byte(uint8_t data, uint8_t cmd);	    
void Oled_Display_On(void);
void Oled_Display_Off(void);	   							   		    
void Oled_Init(void);
void Oled_Clear(void);
void Oled_Draw_Point(uint8_t x, uint8_t y, uint8_t t);
void Oled_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
void Oled_Show_Char(uint8_t x, uint8_t y, uint8_t chr);
void Oled_Show_Num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
void Oled_Show_String(uint8_t x, uint8_t y, uint8_t *p);	 
void Oled_Set_Pos(uint8_t x, uint8_t y);
void Oled_Show_Chinese(uint8_t x, uint8_t y, uint8_t no);
void Oled_Draw_Bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *ptrBmp);
#endif  
	 



