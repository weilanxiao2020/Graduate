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
				1. GND ��Դ��
				2. VCC ��Դ����3��5.5V��
PB13(SPI2_SCK)	3. D0 OLED �� D0 �ţ��� SPI �� IIC ͨ����Ϊʱ�ӹܽ�
PB15(SPI2_MOSI)	4. D1 OLED �� D1 �ţ��� SPI �� IIC ͨ����Ϊ���ݹܽ�
PA8				5. RES OLED �� RES#�ţ�������λ���͵�ƽ��λ��
PB14			6. DC OLED �� D/C#E �ţ����ݺ�������ƹܽ�
PB12(SPI2_NSS)	7. CS OLED �� CS#�ţ�Ҳ����Ƭѡ�ܽ�
*/
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PB->13
//              D1   ��PB->15
//              RES  ��PA->8
//              DC   ��PB->14 
//              CS   ��PB->12 

/* Oled GPIO*/


//Ƭѡ:PB12����OLEDģ���CS���ţ���STM32��SPI��NSS����
#define OLED_GPIO_CS_CLK_FUN      	RCC_APB2PeriphClockCmd
#define OLED_GPIO_CS_CLK        	RCC_APB2Periph_GPIOB
#define OLED_GPIO_CS_PORT    	    GPIOB			   
#define OLED_GPIO_CS_PIN		    GPIO_Pin_12
#define OLED_GPIO_CS_MODE		    GPIO_Mode_Out_PP
#define OLED_GPIO_CS_SPEED          GPIO_Speed_50MHz

//ʱ��:PB13����OLEDģ���D0����,��STM32��SPI��SCK����
#define OLED_GPIO_SCK_CLK_FUN       RCC_APB2PeriphClockCmd
#define OLED_GPIO_SCK_CLK           RCC_APB2Periph_GPIOB
#define OLED_GPIO_SCK_PORT    	    GPIOB			   
#define OLED_GPIO_SCK_PIN		    GPIO_Pin_13
#define OLED_GPIO_SCK_MODE		    GPIO_Mode_Out_PP
#define OLED_GPIO_SCK_SPEED         GPIO_Speed_50MHz

//��������:PB15����OLEDģ���D1���ţ���STM32��SPI��MOSI����
#define OLED_GPIO_MOSI_CLK_FUN      RCC_APB2PeriphClockCmd
#define OLED_GPIO_MOSI_CLK          RCC_APB2Periph_GPIOB
#define OLED_GPIO_MOSI_PORT    	   	GPIOB			   
#define OLED_GPIO_MOSI_PIN		    GPIO_Pin_15
#define OLED_GPIO_MOSI_MODE		    GPIO_Mode_Out_PP
#define OLED_GPIO_MOSI_SPEED        GPIO_Speed_50MHz

//�������:PB14����OLEDģ���MISO���ţ���STM32��SPI��MISO����
#define OLED_GPIO_MISO_CLK_FUN      RCC_APB2PeriphClockCmd
#define OLED_GPIO_MISO_CLK          RCC_APB2Periph_GPIOB
#define OLED_GPIO_MISO_PORT    	    GPIOB			   
#define OLED_GPIO_MISO_PIN		    GPIO_Pin_14
#define OLED_GPIO_MISO_MODE		    GPIO_Mode_IN_FLOATING
#define OLED_GPIO_MISO_SPEED        GPIO_Speed_50MHz

//����:PB14����OLEDģ���DC����
#define OLED_GPIO_DC_CLK_FUN      	RCC_APB2PeriphClockCmd
#define OLED_GPIO_DC_CLK          	RCC_APB2Periph_GPIOB
#define OLED_GPIO_DC_PORT    	    GPIOB			   
#define OLED_GPIO_DC_PIN		    GPIO_Pin_14
#define OLED_GPIO_DC_MODE   	    GPIO_Mode_Out_PP
#define OLED_GPIO_DC_SPEED          GPIO_Speed_50MHz

//��λ:PA8����OLEDģ���RES����
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

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

void Oled_Gpio_Init(void);
void Oled_Spi_Init(void);

void Oled_Write_Cmd(uint8_t cmd);
void Oled_Write_Data(uint8_t data);
void Oled_Write_Byte(uint8_t data);
void Oled_WR_Byte(byte data, uint8_t cmd);
#endif
