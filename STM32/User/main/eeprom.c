#include "eeprom.h"

void E2prom_Init(void)
{
    I2C_Gpio_Init();
    Debug_Info(E2prom_TAG, "bsp eeprom running...");
}

void E2prom_Write_Byte(uint8_t deviceAddr, uint8_t addr, uint8_t data)
{
    I2C_Start();  
	I2C_Write_Byte(deviceAddr);   //发送器件地址0XA0,写数据
	Debug_Info(E2prom_TAG, "0x%02x ack:%d", deviceAddr,I2C_Wait_Ack()); 

    I2C_Write_Byte(addr);   //发送低地址
	Debug_Info(E2prom_TAG, "%d ack:%d", addr, I2C_Wait_Ack()); 

	I2C_Write_Byte(data);     //发送字节							   
	Debug_Info(E2prom_TAG, "data ack:%d",I2C_Wait_Ack()); 	

    I2C_Stop();
	delay_ms(10);
}

uint8_t E2prom_Read_Addr()
{
	uint8_t temp=0;		  	    																 
    I2C_Start();  
    
    I2C_Write_Byte(0xA1); 
	Debug_Info(E2prom_TAG, "0xA1 ack:%d",I2C_Wait_Ack()); 

    temp = I2C_Read_Byte(0);   
    I2C_Stop();  

	return temp;
}

uint8_t E2prom_Read_Byte(uint8_t deviceAddr, uint8_t addr)
{
    uint8_t temp=0;		  	    																 
    I2C_Start();  
    
    I2C_Write_Byte(deviceAddr); 
	Debug_Info(E2prom_TAG, "0x%02x ack:%d", deviceAddr,I2C_Wait_Ack()); 

    I2C_Write_Byte(addr);   
	Debug_Info(E2prom_TAG, "%d ack:%d", addr,I2C_Wait_Ack()); 
	
	I2C_Start();  	 	   
	I2C_Write_Byte(deviceAddr|0x01);  

	Debug_Info(E2prom_TAG, "0x%02x ack:%d", deviceAddr|0x01, I2C_Wait_Ack()); 
    temp = I2C_Read_Byte(0);   
    I2C_Stop();  

	return temp;
}

void E2prom_Read_Bytes(uint8_t deviceAddr, uint8_t addr, uint8_t *pData, uint8_t len)
{
    while(len)
	{
		*pData++=E2prom_Read_Byte(deviceAddr, addr++);	
		len--;
	}
}