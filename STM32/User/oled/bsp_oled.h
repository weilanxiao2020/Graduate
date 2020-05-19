#ifndef __BSP_OLED_H
#define __BSP_OLED_H

#include "stm32f10x.h"
#include "debug.h"

#ifdef GZ_DEBUG
	#define	Bsp_Oled_TAG 				"bsp_oled"
#else 
	#define	Bsp_Oled_TAG 				""
#endif

/*
				1. GND 电源地
				2. VCC 电源正（3～5.5V）
PB13(SPI2_SCK)	3. D0 OLED 的 D0 脚，在 SPI 和 IIC 通信中为时钟管脚
PB15(SPI2_MOSI)	4. D1 OLED 的 D1 脚，在 SPI 和 IIC 通信中为数据管脚
PA8				5. RES OLED 的 RES#脚，用来复位（低电平复位）
PB14			6. DC OLED 的 D/C#E 脚，数据和命令控制管脚
PB12(SPI2_NSS)	7. CS OLED 的 CS#脚，也就是片选管脚
*/
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PB->13
//              D1   接PB->15
//              RES  接PA->8
//              DC   接PB->14 
//              CS   接PB->12 

/* Oled GPIO*/


//片选:PB12，即OLED模块的CS引脚，接STM32的SPI的NSS引脚
#define OLED_GPIO_CS_CLK_FUN      	RCC_APB2PeriphClockCmd
#define OLED_GPIO_CS_CLK        	RCC_APB2Periph_GPIOB
#define OLED_GPIO_CS_PORT    	    GPIOB			   
#define OLED_GPIO_CS_PIN		    GPIO_Pin_12
#define OLED_GPIO_CS_MODE		    GPIO_Mode_Out_PP
#define OLED_GPIO_CS_SPEED          GPIO_Speed_50MHz

//时钟:PB13，即OLED模块的D0引脚,接STM32的SPI的SCK引脚
#define OLED_GPIO_SCK_CLK_FUN       RCC_APB2PeriphClockCmd
#define OLED_GPIO_SCK_CLK           RCC_APB2Periph_GPIOB
#define OLED_GPIO_SCK_PORT    	    GPIOB			   
#define OLED_GPIO_SCK_PIN		    GPIO_Pin_13
#define OLED_GPIO_SCK_MODE		    GPIO_Mode_Out_PP
#define OLED_GPIO_SCK_SPEED         GPIO_Speed_50MHz

//数据输入:PB15，即OLED模块的D1引脚，接STM32的SPI的MOSI引脚
#define OLED_GPIO_MOSI_CLK_FUN      RCC_APB2PeriphClockCmd
#define OLED_GPIO_MOSI_CLK          RCC_APB2Periph_GPIOB
#define OLED_GPIO_MOSI_PORT    	   	GPIOB			   
#define OLED_GPIO_MOSI_PIN		    GPIO_Pin_15
#define OLED_GPIO_MOSI_MODE		    GPIO_Mode_Out_PP
#define OLED_GPIO_MOSI_SPEED        GPIO_Speed_50MHz

//数据输出:PB14，即OLED模块的MISO引脚，接STM32的SPI的MISO引脚
#define OLED_GPIO_MISO_CLK_FUN      RCC_APB2PeriphClockCmd
#define OLED_GPIO_MISO_CLK          RCC_APB2Periph_GPIOB
#define OLED_GPIO_MISO_PORT    	    GPIOB			   
#define OLED_GPIO_MISO_PIN		    GPIO_Pin_14
#define OLED_GPIO_MISO_MODE		    GPIO_Mode_IN_FLOATING
#define OLED_GPIO_MISO_SPEED        GPIO_Speed_50MHz

//数据:PB14，即OLED模块的DC引脚
#define OLED_GPIO_DC_CLK_FUN      	RCC_APB2PeriphClockCmd
#define OLED_GPIO_DC_CLK          	RCC_APB2Periph_GPIOB
#define OLED_GPIO_DC_PORT    	    GPIOB			   
#define OLED_GPIO_DC_PIN		    GPIO_Pin_14
#define OLED_GPIO_DC_MODE   	    GPIO_Mode_Out_PP
#define OLED_GPIO_DC_SPEED          GPIO_Speed_50MHz

//复位:PA8，即OLED模块的RES引脚
#define OLED_GPIO_RST_CLK_FUN      	RCC_APB2PeriphClockCmd
#define OLED_GPIO_RST_CLK          	RCC_APB2Periph_GPIOA
#define OLED_GPIO_RST_PORT    	    GPIOA			   
#define OLED_GPIO_RST_PIN		    GPIO_Pin_8
#define OLED_GPIO_RST_MODE		    GPIO_Mode_Out_PP
#define OLED_GPIO_RST_SPEED         GPIO_Speed_50MHz

#define OLED_CS_On()	    OLED_CS_Clr()					
#define OLED_CS_Off()       OLED_CS_Set()

#define OLED_DC_On()        OLED_DC_Set()
#define OLED_DC_Off()       OLED_DC_Clr()

#define OLED_RST_On()        OLED_RST_Set()
#define OLED_RST_Off()       OLED_RST_Clr()


#define OLED_SCLK_Clr() 	GPIO_ResetBits(OLED_GPIO_SCK_PORT,OLED_GPIO_SCK_PIN)//CLK
#define OLED_SCLK_Set() 	GPIO_SetBits(OLED_GPIO_SCK_PORT,OLED_GPIO_SCK_PIN)

#define OLED_SDIN_Clr() 	GPIO_ResetBits(OLED_GPIO_MOSI_PORT,OLED_GPIO_MOSI_PIN)//DIN
#define OLED_SDIN_Set() 	GPIO_SetBits(OLED_GPIO_MOSI_PORT,OLED_GPIO_MOSI_PIN)

#define OLED_RST_Clr() 		GPIO_ResetBits(OLED_GPIO_RST_PORT,OLED_GPIO_RST_PIN)//RES
#define OLED_RST_Set() 		GPIO_SetBits(OLED_GPIO_RST_PORT,OLED_GPIO_RST_PIN)

#define OLED_DC_Clr() 		GPIO_ResetBits(OLED_GPIO_DC_PORT,OLED_GPIO_DC_PIN)//DC
#define OLED_DC_Set() 		GPIO_SetBits(OLED_GPIO_DC_PORT,OLED_GPIO_DC_PIN)
 		     
#define OLED_CS_Clr()  		GPIO_ResetBits(OLED_GPIO_CS_PORT,OLED_GPIO_CS_PIN)//CS
#define OLED_CS_Set()  		GPIO_SetBits(OLED_GPIO_CS_PORT,OLED_GPIO_CS_PIN)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void Oled_Gpio_Init(void);
void Oled_Spi_Init(void);

void Oled_Write_Cmd(uint8_t cmd);
void Oled_Write_Data(uint8_t data);
void Oled_Write_Byte(uint8_t data);
void Oled_WR_Byte(byte data, uint8_t cmd);
#endif
