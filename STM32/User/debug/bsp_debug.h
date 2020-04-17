#ifndef __BSP_DEBUG_H
#define __BSP_DEBUG_H

#include "stm32f10x.h"

/* usart硬件io口 */
/*				cp2102 ttl转usart
				1. GND 电源地
				2. VCC 电源正（3～5.5V）
PA9(USART1_TX)	3. rx 数据接收，接stm32的tx
PA10(USART1_RX)	4. tx 数据发送，接stm32的rx
PA8(USART1_CLK)
*/

/*********************************** USART1 GPIO 使用引脚定义 *********************************************/
//时钟:PA8，STM32的USART1时钟引脚
#define DEBUG_USART_GPIO_CLK_CLK_FUN    RCC_APB2PeriphClockCmd
#define DEBUG_USART_GPIO_CLK_CLK        RCC_APB2Periph_GPIOA
#define DEBUG_USART_GPIO_CLK_PORT       GPIOA			   
#define DEBUG_USART_GPIO_CLK_PIN	    GPIO_Pin_8
#define DEBUG_USART_GPIO_CLK_MODE	    GPIO_Mode_Out_PP

//发送:PA9，STM32的USART1_TX引脚
#define DEBUG_USART_GPIO_TX_CLK_FUN     RCC_APB2PeriphClockCmd
#define DEBUG_USART_GPIO_TX_CLK         RCC_APB2Periph_GPIOA
#define DEBUG_USART_GPIO_TX_PORT        GPIOA			   
#define DEBUG_USART_GPIO_TX_PIN		    GPIO_Pin_9
#define DEBUG_USART_GPIO_TX_MODE        GPIO_Mode_AF_PP
#define DEBUG_USART_GPIO_TX_SPEED       GPIO_Speed_50MHz

//接收:PA10，STM32的USART1_RX引脚
#define DEBUG_USART_GPIO_RX_CLK_FUN     RCC_APB2PeriphClockCmd
#define DEBUG_USART_GPIO_RX_CLK         RCC_APB2Periph_GPIOA
#define DEBUG_USART_GPIO_RX_PORT        GPIOA			   
#define DEBUG_USART_GPIO_RX_PIN		    GPIO_Pin_10
#define DEBUG_USART_GPIO_RX_MODE        GPIO_Mode_IN_FLOATING

/*********************************** USART1 外设中断定义 *********************************************/
#define DEBUG_USART                     USART1  
#define DEBUG_USART_CLK_FUN             RCC_APB2PeriphClockCmd
#define DEBUG_USART_CLK                 RCC_APB2Periph_USART1  
#define DEBUG_USART_BAUDRATE            115200                              //波特率设置：115200
#define DEBUG_USART_WORDLEN             USART_WordLength_8b                 //数据位数设置：8位
#define DEBUF_USART_STOPBITS            USART_StopBits_1                    //停止位设置：1位
#define DEBUG_USART_PARITY              USART_Parity_No                     //是否奇偶校验：无
#define DEBUG_USART_CONTROL             USART_HardwareFlowControl_None      //硬件流控制模式设置：没有使能
#define DEBUG_USART_MODE                USART_Mode_Rx | USART_Mode_Tx       //接收与发送都使能
#define DEBUG_USART_IT_CONFIG           USART_IT_RXNE

// 接收数据不为空中断
#define DEBUG_USART_IRQ                 USART1_IRQn 
#define DEBUG_USART_IRQHandler          USART1_IRQHandler


/* 初始化串口 */
void Debug_Gpio_Init(void);
void Debug_Usart_Init(void);
void Debug_Usart_NVIC(void);

#endif
