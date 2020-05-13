#ifndef __BSP_IIC_H
#define __BSP_IIC_H

#include "stm32f10x.h"
#include "constants.h"
#include "debug.h"

#ifdef GZ_DEBUG
	#define Bsp_IIC_TAG 			"bsp_iic"
#else 
	#define Bsp_IIC_TAG			""
#endif

#define I2C_GPIO_SDA_CLK_FUN      	RCC_APB2PeriphClockCmd
#define I2C_GPIO_SDA_CLK        	RCC_APB2Periph_GPIOB
#define I2C_GPIO_SDA_PORT    	    GPIOB			   
#define I2C_GPIO_SDA_PIN		    GPIO_Pin_7
#define I2C_GPIO_SDA_MODE		    GPIO_Mode_Out_PP

#define I2C_GPIO_SCL_CLK_FUN      	RCC_APB2PeriphClockCmd
#define I2C_GPIO_SCL_CLK        	RCC_APB2Periph_GPIOB
#define I2C_GPIO_SCL_PORT    	    GPIOB			   
#define I2C_GPIO_SCL_PIN		    GPIO_Pin_6
#define I2C_GPIO_SCL_MODE		    GPIO_Mode_Out_PP

 
#define I2C_SCL_Clr()         			GPIO_ResetBits(I2C_GPIO_SCL_PORT, I2C_GPIO_SCL_PIN)
#define I2C_SCL_Set()        			GPIO_SetBits(I2C_GPIO_SCL_PORT, I2C_GPIO_SCL_PIN)

#define I2C_SDA_Clr()         			GPIO_ResetBits(I2C_GPIO_SDA_PORT, I2C_GPIO_SDA_PIN)
#define I2C_SDA_Set()        			GPIO_SetBits(I2C_GPIO_SDA_PORT, I2C_GPIO_SDA_PIN)

#define I2C_SCL_Read()  				GPIO_ReadInputDataBit(I2C_GPIO_SCL_PORT, I2C_GPIO_SCL_PIN)
#define I2C_SDA_Read()  				GPIO_ReadInputDataBit(I2C_GPIO_SDA_PORT, I2C_GPIO_SDA_PIN)

void I2C_Gpio_Init(void);
void I2C_SDA_In();
void I2C_SDA_Out();
void I2C_delay();
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NAck(void);
uint8_t I2C_Wait_Ack(void);

uint8_t I2C_Read_Byte(uint8_t ack);
void I2C_Write_Byte(uint8_t IIC_Byte);

#endif