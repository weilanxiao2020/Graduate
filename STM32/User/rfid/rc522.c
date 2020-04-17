#include "rc522.h"

/**
  * @brief  读RC522寄存器
  * @param  ucAddress，寄存器地址
  * @retval 寄存器的当前值
  */
byte RC_Raw_Read(uint8_t ucAddress)
{
	uint8_t ucAddr;
	byte ucReturn;
	
	ucAddr = ((ucAddress << 1) & 0x7E) | 0x80;	
	
	// 通过硬件spi从rc522读数据
	RFID_CS_On();
	Rfid_Write_Byte(ucAddr);
	ucReturn = Rfid_Read_Byte();
	RFID_CS_Off();
	
	return ucReturn;	
}

/**
  * @brief  写RC522寄存器
  * @param  ucAddress，寄存器地址
  * @param  ucValue，写入寄存器的值
  * @retval 无
  */
void RC_Raw_Write(uint8_t ucAddress, byte ucValue)
{  
	uint8_t ucAddr;

	ucAddr = (ucAddress << 1) & 0x7E;	
	
	// 通过硬件spi方式往rc522写数据
	RFID_CS_On();
	Rfid_Write_Byte(ucAddr);	
	Rfid_Write_Byte(ucValue);
	RFID_CS_Off();		
}

/**
  * @brief  对RC522寄存器置位
  * @param  ucReg，寄存器地址
  * @param  ucMask，置位值
  * @retval 无
  */
void RC_Raw_SetMask(uint8_t ucReg, byte ucMask)  
{
	uint8_t ucTemp;
	
	ucTemp = RC_Raw_Read(ucReg);
	RC_Raw_Write(ucReg, ucTemp | ucMask); // set bit mask
}


/**
  * @brief  对RC522寄存器清位
  * @param  ucReg，寄存器地址
  * @param  ucMask，清位值
  * @retval 无
  */
void RC_Raw_ClearMask(uint8_t ucReg, byte ucMask)  
{
	uint8_t ucTemp;

	ucTemp = RC_Raw_Read(ucReg);
	RC_Raw_Write(ucReg, ucTemp & (~ucMask)); // clear bit mask
}

/**
  * @brief  复位RC522 
  * @param  无
  * @retval 无
  */
void RC_Reset(void)
{
	RFID_RST_Off();
	delay_us(1);
	
	RFID_RST_On();
	delay_us(1);
	
	RFID_RST_Off();
	delay_us(1);
	
	RC_Raw_Write(CommandReg, 0x0f);
	
	while (RC_Raw_Read(CommandReg) & 0x10 );

	delay_us(1);
  
	//定义发送和接收常用模式 和Mifare卡通讯，CRC初始值0x6363
	RC_Raw_Write(ModeReg, 0x3D);        
	RC_Raw_Write(TReloadRegL, 30);      //16位定时器低位    
	RC_Raw_Write(TReloadRegH, 0);			 //16位定时器高位
	RC_Raw_Write(TModeReg, 0x8D);			 //定义内部定时器的设置
	RC_Raw_Write(TPrescalerReg, 0x3E);	 //设置定时器分频系数
	RC_Raw_Write(TxAutoReg, 0x40);			 //调制发送信号为100%ASK	
	
}

/**
  * @brief  开启RC522天线 
  * @param  无
  * @retval 无
  */
void RC_Antenna_On(void)
{
	uint8_t uc;

	uc = RC_Raw_Read(TxControlReg);
	if(!(uc & 0x03)) RC_Raw_SetMask(TxControlReg, 0x03);		
}


/**
  * @brief  关闭天线
  * @param  无
  * @retval 无
  */
void RC_Antenna_Off(void)
{
	RC_Raw_ClearMask(TxControlReg, 0x03);	
}
