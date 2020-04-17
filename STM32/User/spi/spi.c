#include "spi.h"

// 硬件spi读写
byte Spi_Read_Write_Byte(SPI_TypeDef* SPIx, byte data)
{		
	byte retry=0;
	byte readData;
				 	
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPIx, data); //通过外设SPIx发送一个数据
	// printf("spi data 0x%x", data);
	retry=0;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	readData = SPI_I2S_ReceiveData(SPIx);
	return readData;    
}