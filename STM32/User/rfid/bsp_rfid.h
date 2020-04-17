#ifndef __BSP_RFID_H
#define __BSP_RFID_H

#include "stm32f10x.h"
#include "constants.h"
#include "debug.h"
#include "common.h"
// 定义stm32板子rfid使用的gpio和spi

#ifdef GZ_DEBUG
	#define Bsp_Rfid_TAG 			"bsp_rfid"
#else 
	#define Bsp_Rfid_TAG			""
#endif
/* rifd硬件io口*/
/*
				1. GND 电源地
				2. VCC 电源正（3～5.5V）
PA1				3. rifd的rst，复位管脚，低电平有效
PA4(SPI1_NSS)	4. rifd的sda，片选管脚，低电平有效
PA5(SPI1_SCK)	5. rfid的sck，时钟管脚
PA6(SPI1_MISO)	6. rfid到stm32，数据引脚
PA7(SPI1_MOSI)	7. stm32到rfid，数据引脚
*/

/*********************************** RC522 引脚定义 *********************************************/
//RC522模块有除了电源还有6个数据引脚，其中IRQ不需要使用，悬空即可，剩下的5个数据引脚连接如下：
//如果RC522需要修改与STM32的连接，则修改这些IO即可，但必须连接到STM32的SPI引脚

//片选:PA4，即RC522模块的SDA引脚，接STM32的SPI的NSS引脚
#define RFID_GPIO_CS_CLK_FUN      	RCC_APB2PeriphClockCmd
#define RFID_GPIO_CS_CLK        	RCC_APB2Periph_GPIOA
#define RFID_GPIO_CS_PORT    	    GPIOA			   
#define RFID_GPIO_CS_PIN		    GPIO_Pin_4
#define RFID_GPIO_CS_MODE		    GPIO_Mode_Out_PP

//时钟:PA5，即RC522模块的SCK引脚，接STM32的SPI的SCK引脚
#define RFID_GPIO_SCK_CLK_FUN       RCC_APB2PeriphClockCmd
#define RFID_GPIO_SCK_CLK           RCC_APB2Periph_GPIOA
#define RFID_GPIO_SCK_PORT    	    GPIOA			   
#define RFID_GPIO_SCK_PIN		    GPIO_Pin_5
#define RFID_GPIO_SCK_MODE		    GPIO_Mode_Out_PP

//数据输入:PA7，即RC522模块的MOSI引脚，接STM32的SPI的MOSI引脚
#define RFID_GPIO_MOSI_CLK_FUN      RCC_APB2PeriphClockCmd
#define RFID_GPIO_MOSI_CLK          RCC_APB2Periph_GPIOA
#define RFID_GPIO_MOSI_PORT    	   	GPIOA			   
#define RFID_GPIO_MOSI_PIN		    GPIO_Pin_7
#define RFID_GPIO_MOSI_MODE		    GPIO_Mode_Out_PP

//数据输出:PA6，即RC522模块的MISO引脚，接STM32的SPI的MISO引脚
#define RFID_GPIO_MISO_CLK_FUN      RCC_APB2PeriphClockCmd
#define RFID_GPIO_MISO_CLK          RCC_APB2Periph_GPIOA
#define RFID_GPIO_MISO_PORT    	    GPIOA			   
#define RFID_GPIO_MISO_PIN		    GPIO_Pin_6
#define RFID_GPIO_MISO_MODE		    GPIO_Mode_IN_FLOATING

//复位:PA1，即即RC522模块的RST引脚，接STM32的普通IO即可
#define RFID_GPIO_RST_CLK_FUN       RCC_APB2PeriphClockCmd
#define RFID_GPIO_RST_CLK           RCC_APB2Periph_GPIOA
#define RFID_GPIO_RST_PORT    	    GPIOA		   
#define RFID_GPIO_RST_PIN		    GPIO_Pin_1
#define RFID_GPIO_RST_MODE		    GPIO_Mode_Out_PP

/*********************************** RC522 函数宏定义*********************************************/
#define RFID_CS_Clr()         		GPIO_ResetBits(RFID_GPIO_CS_PORT, RFID_GPIO_CS_PIN)
#define RFID_CS_Set()        		GPIO_SetBits(RFID_GPIO_CS_PORT, RFID_GPIO_CS_PIN)
#define RFID_CS_On()				RFID_CS_Clr()
#define RFID_CS_Off()				RFID_CS_Set()

#define RFID_RST_Clr()      		GPIO_ResetBits(RFID_GPIO_RST_PORT, RFID_GPIO_RST_PIN)
#define RFID_RST_Set()     			GPIO_SetBits(RFID_GPIO_RST_PORT, RFID_GPIO_RST_PIN)
#define RFID_RST_On()				RFID_RST_Clr()
#define RFID_RST_Off()				RFID_RST_Set()

#define RFID_SCK_Clr()             	GPIO_ResetBits(RFID_GPIO_SCK_PORT, RFID_GPIO_SCK_PIN)
#define RFID_SCK_Set()             	GPIO_SetBits (RFID_GPIO_SCK_PORT, RFID_GPIO_SCK_PIN)

#define RFID_MOSI_Clr()            	GPIO_ResetBits(RFID_GPIO_MOSI_PORT, RFID_GPIO_MOSI_PIN)
#define RFID_MOSI_Set()            	GPIO_SetBits(RFID_GPIO_MOSI_PORT, RFID_GPIO_MOSI_PIN)

#define RFID_MISO_GET()          	GPIO_ReadInputDataBit(RFID_GPIO_MISO_PORT, RFID_GPIO_MISO_PIN)

#define Rfid_Mode 0

#if Rfid_Mode == 1
	#include "spi.h"
	#define RFID_SPI				SPI1
	#define DummyData				0xEF
	#define RFID_SPI_CLK_FUN    	RCC_APB2PeriphClockCmd
	#define RFID_SPI_CLK        	RCC_APB2Periph_SPI1
	#define RFID_SPI_DIR        	SPI_Direction_2Lines_FullDuplex
	#define RFID_SPI_MODE       	SPI_Mode_Master
	#define RFID_SPI_DATA_SIZE  	SPI_DataSize_8b
	#define RFID_SPI_CPOL       	SPI_CPOL_High
	#define RFID_SPI_CPHA       	SPI_CPHA_2Edge
	#define RFID_SPI_NSS        	SPI_NSS_Hard
	#define RFID_SPI_BAURATE    	SPI_BaudRatePrescaler_16
	#define RFID_SPI_FISRT_BIT  	SPI_FirstBit_MSB
	#define RFID_SPI_CRC        	16
#endif

void Rfid_Gpio_Init(void);
void Rfid_Spi_Init(void);
byte Rfid_Read_Byte(void);
void Rfid_Write_Byte(byte data);




#define          macDummy_Data              0x00

#endif
