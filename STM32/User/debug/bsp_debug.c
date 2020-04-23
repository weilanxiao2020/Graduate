#include "bsp_debug.h"

// debugIO口初始化
void Debug_Gpio_Init(void)
{
	GPIO_InitTypeDef gpio_Init;
	/* 打开USART的GPIO时钟*/
	// TX发送
	DEBUG_USART_GPIO_TX_CLK_FUN(DEBUG_USART_GPIO_TX_CLK, ENABLE);
	gpio_Init.GPIO_Pin = DEBUG_USART_GPIO_TX_PIN;
	gpio_Init.GPIO_Mode = DEBUG_USART_GPIO_TX_MODE;
	gpio_Init.GPIO_Speed = DEBUG_USART_GPIO_TX_SPEED;
	GPIO_Init(DEBUG_USART_GPIO_TX_PORT, &gpio_Init);

	// RX接收
	DEBUG_USART_GPIO_RX_CLK_FUN(DEBUG_USART_GPIO_RX_CLK, ENABLE);
	gpio_Init.GPIO_Pin = DEBUG_USART_GPIO_RX_PIN;
	gpio_Init.GPIO_Mode = DEBUG_USART_GPIO_RX_MODE;
	GPIO_Init(DEBUG_USART_GPIO_RX_PORT, &gpio_Init);
}

// debug串口初始化
void Debug_Usart_Init(void)
{
	USART_InitTypeDef usart_Init;
	/* 打开USART1外设功能时钟 */
	DEBUG_USART_CLK_FUN(DEBUG_USART_CLK, ENABLE);
    /* USART1 工作模式配置 */
	usart_Init.USART_BaudRate = DEBUG_USART_BAUDRATE;	
	usart_Init.USART_WordLength = DEBUG_USART_WORDLEN;	
	usart_Init.USART_StopBits = DEBUF_USART_STOPBITS; 	
	usart_Init.USART_Parity = DEBUG_USART_PARITY ;  
	usart_Init.USART_HardwareFlowControl = DEBUG_USART_CONTROL;	
	usart_Init.USART_Mode = DEBUG_USART_MODE;
	USART_Init(DEBUG_USART, &usart_Init);  
	USART_ITConfig(DEBUG_USART, DEBUG_USART_IT_CONFIG,ENABLE);
	USART_Cmd(DEBUG_USART, ENABLE);// USART1使能
}

// debug中断初始化
void Debug_Usart_NVIC(void)
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
	nvic_Init.NVIC_IRQChannel = DEBUG_USART_IRQ;
	nvic_Init.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_Init.NVIC_IRQChannelSubPriority = 0;
	nvic_Init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_Init);
}


