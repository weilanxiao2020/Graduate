#include "bsp_rfid.h"

void Rfid_Gpio_Init(void)
{
	GPIO_InitTypeDef gpio_Init;

	/*!< Configure SPI_RC522_SPI pins: CS */
	// 片选
	RFID_GPIO_CS_CLK_FUN(RFID_GPIO_CS_CLK, ENABLE);
	gpio_Init.GPIO_Pin = RFID_GPIO_CS_PIN;
	gpio_Init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_Init.GPIO_Mode = RFID_GPIO_CS_MODE;
	GPIO_Init(RFID_GPIO_CS_PORT, &gpio_Init);

	/*!< Configure SPI_RC522_SPI pins: SCK */
	// 时钟
	RFID_GPIO_SCK_CLK_FUN(RFID_GPIO_SCK_CLK, ENABLE);
	gpio_Init.GPIO_Pin = RFID_GPIO_SCK_PIN;
	gpio_Init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_Init.GPIO_Mode = RFID_GPIO_SCK_MODE;
	GPIO_Init(RFID_GPIO_SCK_PORT, &gpio_Init);

	/*!< Configure SPI_RC522_SPI pins: MOSI */
	// stm32->rfid
	RFID_GPIO_MOSI_CLK_FUN(RFID_GPIO_MOSI_CLK, ENABLE);
	gpio_Init.GPIO_Pin = RFID_GPIO_MOSI_PIN;
	gpio_Init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_Init.GPIO_Mode = RFID_GPIO_MOSI_MODE;
	GPIO_Init(RFID_GPIO_MOSI_PORT, &gpio_Init);

	/*!< Configure SPI_RC522_SPI pins: MISO */
	// rfid->stm32
	RFID_GPIO_MISO_CLK_FUN(RFID_GPIO_MISO_CLK, ENABLE);
	gpio_Init.GPIO_Pin = RFID_GPIO_MISO_PIN;
	gpio_Init.GPIO_Mode = RFID_GPIO_MISO_MODE;
	GPIO_Init(RFID_GPIO_MISO_PORT, &gpio_Init);	
		
	/*!< Configure SPI_RC522_SPI pins: RST */
	// 复位
	RFID_GPIO_RST_CLK_FUN(RFID_GPIO_RST_CLK, ENABLE);
	gpio_Init.GPIO_Pin = RFID_GPIO_RST_PIN;
	gpio_Init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_Init.GPIO_Mode = RFID_GPIO_RST_MODE;
	GPIO_Init(RFID_GPIO_RST_PORT, &gpio_Init);

	Debug_Info(Bsp_Rfid_TAG, "bsp rfid gpio init");
}

void Rfid_Spi_Init(void)
{
#if Rfid_Mode == 1
	SPI_InitTypeDef spi_Init;
	RFID_SPI_CLK_FUN(RFID_SPI_CLK, ENABLE); //①GPIO,SPI 时钟使能
	spi_Init.SPI_Direction = RFID_SPI_DIR; //设置 SPI 全双工
	spi_Init.SPI_Mode = RFID_SPI_MODE; //设置 SPI 工作模式:设置为主 SPI
	spi_Init.SPI_DataSize = RFID_SPI_DATA_SIZE; // 8 位帧结构
	spi_Init.SPI_CPOL = RFID_SPI_CPOL;//选择了串行时钟的稳态:时钟悬空高
	spi_Init.SPI_CPHA = RFID_SPI_CPHA; //数据捕获于第二个时钟沿
	spi_Init.SPI_NSS = RFID_SPI_NSS; //NSS 信号由硬件管理
	spi_Init.SPI_BaudRatePrescaler = RFID_SPI_BAURATE; //预分频 256
	spi_Init.SPI_FirstBit = RFID_SPI_FISRT_BIT; //数据传输从 MSB 位开始
	spi_Init.SPI_CRCPolynomial = RFID_SPI_CRC; //CRC 值计算的多项式
	SPI_Init(RFID_SPI, &spi_Init); //②根据指定的参数初始化外设 SPIx 寄存器
	SPI_Cmd(RFID_SPI, ENABLE); //③使能 SPI 外设
	//SPI1_ReadWriteByte(0xff);//④启动传输
	Debug_Info(Bsp_Rfid_TAG, "bsp rfid spi init");
#endif
}

static void Rfid_delay()
{
	delay_us(20);
}

// 条件编译，如果有硬件SPI且启用
#if Rfid_Mode == 1
// 硬件spi
byte Rfid_Read_Byte(void) 
{
	byte data;
	data = Spi_Read_Write_Byte(RFID_SPI, DummyData);
	return data;
}

void Rfid_Write_Byte(byte data)
{
	Spi_Read_Write_Byte(RFID_SPI, data);
}

#else
		// ???? ????
		// 1000 0000
		// 		   &
		// ?000 0000
 /**
  * @brief  向RC522发送1 Byte 数据
  * @param  data，要发送的数据
  * @retval RC522返回的数据
  */
void Rfid_Write_Byte(byte data)
{
	uint8_t cnt;

	for(cnt=0;cnt<8;cnt++)
	{     
		if (data&0x80) RFID_MOSI_Set();
		else RFID_MOSI_Clr();

		Rfid_delay();
		RFID_SCK_Clr();
		Rfid_delay();
		RFID_SCK_Set();
		Rfid_delay();

		data <<= 1; 
	} 	
	// Tips_Glint(1,100);
}


/**
  * @brief  Rfid使用Spi读取1字节数据
  * @param  无
  * @retval Spi读取的数据
  */
byte Rfid_Read_Byte(void)
{
	uint8_t cnt;
	byte SPI_Data;//读取的spi数据

	for(cnt=0;cnt<8;cnt++)
	{
		SPI_Data <<= 1;
		RFID_SCK_Clr();
		Rfid_delay();
		
		if ( RFID_MISO_GET() == 1) SPI_Data |= 0x01;
		Rfid_delay();
		RFID_SCK_Set();
		Rfid_delay();
	}
	
	return SPI_Data;
}

#endif
