#include "bsp_oled.h"

void Oled_Gpio_Init(void)
{
    GPIO_InitTypeDef oled_Init;
	 	GPIO_InitTypeDef  GPIO_InitStructure;


	/*!< Configure SPI_OLED_SPI pins: CS */
	// 片选
	OLED_GPIO_CS_CLK_FUN(OLED_GPIO_CS_CLK, ENABLE);
	oled_Init.GPIO_Pin = OLED_GPIO_CS_PIN;
	oled_Init.GPIO_Speed = OLED_GPIO_CS_SPEED;
	oled_Init.GPIO_Mode = OLED_GPIO_CS_MODE;
	GPIO_Init(OLED_GPIO_CS_PORT, &oled_Init);

	/*!< Configure SPI_OLED_SPI pins: SCK */
	// 时钟
	OLED_GPIO_SCK_CLK_FUN(OLED_GPIO_SCK_CLK, ENABLE);
	oled_Init.GPIO_Pin = OLED_GPIO_SCK_PIN;
	oled_Init.GPIO_Speed = OLED_GPIO_SCK_SPEED;
	oled_Init.GPIO_Mode = OLED_GPIO_SCK_MODE;
	GPIO_Init(OLED_GPIO_SCK_PORT, &oled_Init);

	/*!< Configure SPI_OLED_SPI pins: MOSI */
	// stm32->oled
	OLED_GPIO_MOSI_CLK_FUN(OLED_GPIO_MOSI_CLK, ENABLE);
	oled_Init.GPIO_Pin = OLED_GPIO_MOSI_PIN;
	oled_Init.GPIO_Speed = OLED_GPIO_MOSI_SPEED;
	oled_Init.GPIO_Mode = OLED_GPIO_MOSI_MODE;
	GPIO_Init(OLED_GPIO_MOSI_PORT, &oled_Init);

	/*!< Configure SPI_RC522_SPI pins: MISO */
	// oled->stm32

	/*!< Configure SPI_OLED_SPI pins: DC */
	// DC
	OLED_GPIO_DC_CLK_FUN(OLED_GPIO_DC_CLK, ENABLE);
	oled_Init.GPIO_Pin = OLED_GPIO_DC_PIN;
	oled_Init.GPIO_Speed = OLED_GPIO_DC_SPEED;
	oled_Init.GPIO_Mode = OLED_GPIO_DC_MODE;
	GPIO_Init(OLED_GPIO_DC_PORT, &oled_Init);
		
	/*!< Configure SPI_OLED_SPI pins: RST */
	// 复位
	OLED_GPIO_RST_CLK_FUN(OLED_GPIO_RST_CLK, ENABLE);
	oled_Init.GPIO_Pin = OLED_GPIO_RST_PIN;
	oled_Init.GPIO_Speed = OLED_GPIO_RST_SPEED;
	oled_Init.GPIO_Mode = OLED_GPIO_RST_MODE;
	GPIO_Init(OLED_GPIO_RST_PORT, &oled_Init);


 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能A端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOD3,6
 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_4);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能A端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOD3,6
 	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8);	





    Debug_Info(Bsp_Oled_TAG, "bsp oled gpio init");
}

void Oled_Spi_Init(void)
{
	SPI_InitTypeDef spi_Init;
	OLED_SPI_CLK_FUN(OLED_SPI_CLK, ENABLE); //①GPIO,SPI 时钟使能
	spi_Init.SPI_Direction = OLED_SPI_DIR; //设置 SPI 全双工
	spi_Init.SPI_Mode = OLED_SPI_MODE; //设置 SPI 工作模式:设置为主 SPI
	spi_Init.SPI_DataSize = OLED_SPI_DATA_SIZE; // 8 位帧结构
	spi_Init.SPI_CPOL = OLED_SPI_CPOL;//选择了串行时钟的稳态:时钟悬空高
	spi_Init.SPI_CPHA = OLED_SPI_CPHA; //数据捕获于第二个时钟沿
	spi_Init.SPI_NSS = OLED_SPI_NSS; //NSS 信号由硬件管理
	spi_Init.SPI_BaudRatePrescaler = OLED_SPI_BAURATE; //预分频 256
	spi_Init.SPI_FirstBit = OLED_SPI_FISRT_BIT; //数据传输从 MSB 位开始
	spi_Init.SPI_CRCPolynomial = OLED_SPI_CRC; //CRC 值计算的多项式
	SPI_Init(OLED_SPI, &spi_Init); //②根据指定的参数初始化外设 SPIx 寄存器
	SPI_Cmd(OLED_SPI, ENABLE); //③使能 SPI 外设
	//SPI1_ReadWriteByte(0xff);//④启动传输
    Debug_Info(Bsp_Oled_TAG, "bsp oled spi init");
}

void Oled_Spi_Write(byte data)
{
    /* Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(OLED_SPI, SPI_I2S_FLAG_TXE) == RESET);
    /* Send byte through the SPI2 peripheral */
    SPI_I2S_SendData(OLED_SPI, data);
}

//写入数据
void Oled_Write_Data(byte data)
{
	// Debug_Error(Bsp_Oled_TAG, "write data:");
	OLED_CS_On();
	OLED_DC_On();
	Oled_Write_Byte(data);
}

//写入命令
void Oled_Write_Cmd(byte cmd)
{
	// Debug_Error(Bsp_Oled_TAG, "write cmd:");
	OLED_CS_On();
	OLED_DC_Off();
	Oled_Write_Byte(cmd);
}

//硬件SPI发送数据
void Oled_Write_Byte(byte data)
{
	Spi_Read_Write_Byte(OLED_SPI, data);
}

#if OLED_MODE==1
//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void Oled_WR_Byte(byte data, uint8_t cmd)
{
	DATAOUT(dat);	    
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		   
	OLED_CS_Clr();
	OLED_WR_Clr();	 
	OLED_WR_Set();
	OLED_CS_Set();	  
	OLED_DC_Set();	 
} 	    	    
#else
//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void Oled_WR_Byte(byte data, uint8_t cmd)
{	
	uint8_t i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(data&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		data<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 
#endif
