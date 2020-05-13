#include "bsp_iic.h"

void I2C_Gpio_Init(void)
{
    GPIO_InitTypeDef gpio_Init;

	// SDA
	I2C_GPIO_SDA_CLK_FUN(I2C_GPIO_SDA_CLK, ENABLE);
	gpio_Init.GPIO_Pin = I2C_GPIO_SDA_PIN;
	gpio_Init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_Init.GPIO_Mode = I2C_GPIO_SDA_MODE;
	GPIO_Init(I2C_GPIO_SDA_PORT, &gpio_Init);

	// SCL
	I2C_GPIO_SCL_CLK_FUN(I2C_GPIO_SCL_CLK, ENABLE);
	gpio_Init.GPIO_Pin = I2C_GPIO_SCL_PIN;
	gpio_Init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_Init.GPIO_Mode = I2C_GPIO_SCL_MODE;
	GPIO_Init(I2C_GPIO_SCL_PORT, &gpio_Init);

	Debug_Info(Bsp_IIC_TAG, "bsp iic gpio init");
}

void I2C_delay()
{
	delay_us(50);
}

void I2C_SDA_In()
{
    GPIOB->CRL&=0X0FFFFFFF;
    GPIOB->CRL|=(u32)8<<28;
}

void I2C_SDA_Out()
{
    GPIOB->CRL&=0X0FFFFFFF;
    GPIOB->CRL|=(u32)3<<28;
}

void I2C_Start()
{
    I2C_SDA_Out();

    I2C_SDA_Set();
    I2C_delay();

    I2C_SCL_Set();
    I2C_delay();

    I2C_SDA_Clr();
    I2C_delay();

    Debug_Info(Bsp_IIC_TAG, "iic start");
}

void I2C_Stop()
{
    I2C_SDA_Out();

    I2C_SDA_Clr();
    I2C_delay();

    I2C_SCL_Set();   
    I2C_delay();

    I2C_SDA_Set();
    I2C_delay();

    Debug_Info(Bsp_IIC_TAG, "iic stop");
}

void I2C_Ack()
{
    I2C_SDA_Out();

    I2C_SCL_Clr();
    I2C_delay();

    I2C_SDA_Set();
    I2C_delay();

    I2C_SCL_Set();
    I2C_delay();

    I2C_SCL_Clr();
    I2C_delay(); 

    Debug_Info(Bsp_IIC_TAG, "iic ack");   
}

void I2C_NAck()
{
    I2C_SDA_Out();

    I2C_SCL_Clr();
    I2C_delay();

    I2C_SDA_Set();
    I2C_delay();

    I2C_SCL_Set();
    I2C_delay();

    I2C_SCL_Clr();
    I2C_delay();  

    Debug_Info(Bsp_IIC_TAG, "iic nack");  
}

uint8_t I2C_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	I2C_SDA_In();      

	// I2C_SDA_Clr();
    I2C_delay();

	I2C_SCL_Set();
    I2C_delay();
    	 
	while(I2C_SDA_Read())
	{
        // Debug_Info(Bsp_IIC_TAG, "iic read:0x%02x",I2C_SDA_Read());  
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop();
			return 1;
		}
	}

	I2C_SCL_Clr();
	return 0;  
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t I2C_Read_Byte(uint8_t ack)
{
	uint8_t i, data=0;
	I2C_SDA_In();

    for(i=0;i<8;i++ )
	{
        I2C_SCL_Clr(); 
        I2C_delay();

		I2C_SCL_Set();
        data<<=1;
        if(I2C_SDA_Read()==1) data |= 0x01;   
		I2C_delay(); 
        printf("0x%02x\t", data);
    }					 
    if (!ack)
        I2C_NAck();
    else
        I2C_Ack();

    return data;
}


void I2C_Write_Byte(uint8_t data)
{                        
    uint8_t i;   
	I2C_SDA_Out(); 

    for(i=0; i<8; i++)
    { 
        I2C_SCL_Clr();
        I2C_delay();     

		if((data&0x80)>>7) I2C_SDA_Set();
		else I2C_SDA_Clr();
		
        data<<=1;
        I2C_delay();
        		 	  
        I2C_SCL_Set();
		I2C_delay();
    }	 
} 