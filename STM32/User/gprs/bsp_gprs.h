#ifndef __BSP_GPRS_H
#define __BSP_GPRS_H

#include "stm32f10x.h"
#include "debug.h"

#ifdef GZ_DEBUG
    #define Bsp_Gprs_TAG       "bsp_gprs"
#else 
    #define Bsp_Gprs_TAG       ""  
#endif

/* usart硬件io口 */
/*				usart2接sim900a
				1. GND 电源地
				2. VCC 电源正（3～5.5V）
PA2(USART2_TX)	3. rx 数据接收，接stm32的tx
PA3(USART3_RX)	4. tx 数据发送，接stm32的rx
*/

/*********************************** USART2 GPIO 使用引脚定义 *********************************************/
//发送:PA2，STM32的USART2_TX引脚，发送给ga6的数据
#define GPRS_USART_GPIO_TX_CLK_FUN     RCC_APB2PeriphClockCmd
#define GPRS_USART_GPIO_TX_CLK         RCC_APB2Periph_GPIOA
#define GPRS_USART_GPIO_TX_PORT        GPIOA			   
#define GPRS_USART_GPIO_TX_PIN		   GPIO_Pin_2
#define GPRS_USART_GPIO_TX_MODE        GPIO_Mode_AF_PP
#define GPRS_USART_GPIO_TX_SPEED       GPIO_Speed_50MHz

//接收:PA3，STM32的USART2_RX引脚，接收ga6发送的数据
#define GPRS_USART_GPIO_RX_CLK_FUN     RCC_APB2PeriphClockCmd
#define GPRS_USART_GPIO_RX_CLK         RCC_APB2Periph_GPIOA
#define GPRS_USART_GPIO_RX_PORT        GPIOA			   
#define GPRS_USART_GPIO_RX_PIN		   GPIO_Pin_3
#define GPRS_USART_GPIO_RX_MODE        GPIO_Mode_IN_FLOATING

/*********************************** USART2 外设中断定义 *********************************************/
#define GPRS_USART                     USART2  
#define GPRS_USART_CLK_FUN             RCC_APB1PeriphClockCmd
#define GPRS_USART_CLK                 RCC_APB1Periph_USART2  
#define GPRS_USART_BAUDRATE            115200                              //波特率设置：115200
#define GPRS_USART_WORDLEN             USART_WordLength_8b                 //数据位数设置：8位
#define GPRS_USART_STOPBITS            USART_StopBits_1                    //停止位设置：1位
#define GPRS_USART_PARITY              USART_Parity_No                     //是否奇偶校验：无
#define GPRS_USART_CONTROL             USART_HardwareFlowControl_None      //硬件流控制模式设置：没有使能
#define GPRS_USART_MODE                USART_Mode_Rx | USART_Mode_Tx       //接收与发送都使能
#define GPRS_USART_IT_CONFIG           USART_IT_RXNE                       //使能串口接收中断

// 接收数据不为空中断
#define GPRS_USART_IRQ                 USART2_IRQn 
#define GPRS_USART_IRQHandler          USART2_IRQHandler

#define STR_END         "\r\n"          // 字符发送结束符
#define Buffer_Max      128                // 接收缓冲区大小

/* 初始化串口 */
void Gprs_Gpio_Init(void);
void Gprs_Usart_Init(void);
void Gprs_Usart_NVIC(void);

void Gprs_Send_Data(uint8_t *data, int len);
void Gprs_Wrtie_String(const char* str);

void Gprs_Clear_Buffer(void);
char *Gprs_Get_Buffer(void);
void Gprs_Write_Cmd(const char* cmd);
void Gprs_Write_Cmd_End(const char* cmd);


#endif
