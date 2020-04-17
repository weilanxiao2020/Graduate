#include "bsp_gprs.h"

// gprs使用gpio设置
void Gprs_Gpio_Init(void)
{
	GPIO_InitTypeDef gpio_Init;
	/* 打开USART的GPIO时钟*/
	// TX发送
	GPRS_USART_GPIO_TX_CLK_FUN(GPRS_USART_GPIO_TX_CLK, ENABLE);
	gpio_Init.GPIO_Pin = GPRS_USART_GPIO_TX_PIN;
	gpio_Init.GPIO_Mode = GPRS_USART_GPIO_TX_MODE;
	gpio_Init.GPIO_Speed = GPRS_USART_GPIO_TX_SPEED;
	GPIO_Init(GPRS_USART_GPIO_TX_PORT, &gpio_Init);

	// RX接收
	GPRS_USART_GPIO_RX_CLK_FUN(GPRS_USART_GPIO_RX_CLK, ENABLE);
	gpio_Init.GPIO_Pin = GPRS_USART_GPIO_RX_PIN;
	gpio_Init.GPIO_Mode = GPRS_USART_GPIO_RX_MODE;
	GPIO_Init(GPRS_USART_GPIO_RX_PORT, &gpio_Init);
    Debug_Info(Bsp_Gprs_TAG, "bsp gprs gpio init");
}

// gprs使用串口设置
void Gprs_Usart_Init(void)
{
	USART_InitTypeDef usart_Init;
	/* 打开USART1外设功能时钟 */
	GPRS_USART_CLK_FUN(GPRS_USART_CLK, ENABLE);
    /* USART1 工作模式配置 */
	usart_Init.USART_BaudRate = GPRS_USART_BAUDRATE;	
	usart_Init.USART_WordLength = GPRS_USART_WORDLEN;	
	usart_Init.USART_StopBits = GPRS_USART_STOPBITS; 	
	usart_Init.USART_Parity = GPRS_USART_PARITY ;  
	usart_Init.USART_HardwareFlowControl = GPRS_USART_CONTROL;	
	usart_Init.USART_Mode = GPRS_USART_MODE;
	USART_Init(GPRS_USART, &usart_Init);  
	USART_ITConfig(GPRS_USART, GPRS_USART_IT_CONFIG,ENABLE);
	USART_Cmd(GPRS_USART, ENABLE);                    			//使能串口 
	USART_ClearFlag(GPRS_USART, USART_FLAG_TC);					//清除发送完成标志
    Debug_Info(Bsp_Gprs_TAG, "bsp gprs usart init");
}

// gprs使用串口中断设置
void Gprs_Usart_NVIC(void)
{
	NVIC_InitTypeDef nvic_Init;
	#ifdef  VECT_TAB_RAM  
	/* Set the Vector Table base location at 0x20000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//设置优先级分组：先占优先级和从优先级

	/* Enable the USART1 Interrupt */
	nvic_Init.NVIC_IRQChannel = GPRS_USART_IRQ;
	nvic_Init.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_Init.NVIC_IRQChannelSubPriority = 0;
	nvic_Init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_Init);
    Debug_Info(Bsp_Gprs_TAG, "bsp gprs nvic init");
}

void Gprs_Wrtie_String(const char* str)
{
    Usart_SendStr(GPRS_USART, str);
	// while(*str)//检测字符串结束符
	// {
	// 	while(USART_GetFlagStatus(GPRS_USART, USART_FLAG_TC)==RESET); 
	// 	USART_SendData(GPRS_USART ,*str++);//发送当前字符
	// }
}

void Gprs_Write_Cmd(const char* cmd)
{
	Gprs_Clear_Buffer(); //清空接收数据的buffer
	Gprs_Wrtie_String(cmd); //发出字符串
	Gprs_Wrtie_String(STR_END); //再自动发送 \r\n两个字符
}

void Gprs_Write_Cmd_END(const char* cmd)
{
	Gprs_Clear_Buffer(); //清空接收数据的buffer
	Gprs_Wrtie_String(cmd); //发出字符串
}

char Gprs_Buffer[Buffer_Max];     //串口2接收缓存
uint8_t bufferHead;
static boolean isBufferMax;
char *Gprs_Get_Buffer(void)
{
    return Gprs_Buffer;
}

boolean getBufferMaxFlag()
{
    return isBufferMax;
}

void setBufferMaxFlag(boolean flag)
{
    isBufferMax = flag;
}

void USART2_IRQHandler(void)                	
{
	uint8_t res=0;
	res = Usart_GetByte(GPRS_USART);
	Gprs_Buffer[bufferHead] = res;  	  //将接收到的字符串存到缓存中
	bufferHead++;                	  //缓存指针向后移动
	if(bufferHead >= Buffer_Max)       	  //如果缓存满,将缓存指针指向缓存的首地址
	{
		bufferHead = 0;
        isBufferMax = true;
	}
} 

void Gprs_Clear_Buffer()
{
	uint16_t i;
	for(i=0;i<Buffer_Max;i++)      //将缓存内容清零
	{
		Gprs_Buffer[i] = 0x00;
	}
    bufferHead = 0;              //接收字符串的起始存储位置
    isBufferMax = false;
}
