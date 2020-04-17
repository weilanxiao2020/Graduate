#ifndef __BSP_GPS_H
#define __BSP_GPS_H

#include "stm32f10x.h"
#include "debug.h"
#include "string.h"

#ifdef GZ_DEBUG
    #define Bsp_Gps_TAG       "bsp_gps"
#else 
    #define Bsp_Gps_TAG       ""  
#endif

/* usart硬件io口 */
/*				usart3接gps
				1. GND 电源地
				2. VCC 电源正（3～5.5V）
PB10(USART3_TX)	3. rx 数据接收，接stm32的tx
PB11(USART3_RX)	4. tx 数据发送，接stm32的rx
*/

/*********************************** USART3 GPIO 使用引脚定义 *********************************************/
//发送:PB10，STM32的USART3_TX引脚，发送给gps的数据
#define GPS_USART_GPIO_TX_CLK_FUN     RCC_APB2PeriphClockCmd
#define GPS_USART_GPIO_TX_CLK         RCC_APB2Periph_GPIOB
#define GPS_USART_GPIO_TX_PORT        GPIOB			   
#define GPS_USART_GPIO_TX_PIN		  GPIO_Pin_10
#define GPS_USART_GPIO_TX_MODE        GPIO_Mode_AF_PP
#define GPS_USART_GPIO_TX_SPEED       GPIO_Speed_50MHz

//接收:PB11，STM32的USART3_RX引脚，接收gps发送的数据
#define GPS_USART_GPIO_RX_CLK_FUN     RCC_APB2PeriphClockCmd
#define GPS_USART_GPIO_RX_CLK         RCC_APB2Periph_GPIOB
#define GPS_USART_GPIO_RX_PORT        GPIOB			   
#define GPS_USART_GPIO_RX_PIN		  GPIO_Pin_11
#define GPS_USART_GPIO_RX_MODE        GPIO_Mode_IN_FLOATING

/*********************************** USART3 外设中断定义 *********************************************/
#define GPS_USART                     USART3 
#define GPS_USART_CLK_FUN             RCC_APB1PeriphClockCmd
#define GPS_USART_CLK                 RCC_APB1Periph_USART3  
#define GPS_USART_BAUDRATE            9600                                //波特率设置：9600
#define GPS_USART_WORDLEN             USART_WordLength_8b                 //数据位数设置：8位
#define GPS_USART_STOPBITS            USART_StopBits_1                    //停止位设置：1位
#define GPS_USART_PARITY              USART_Parity_No                     //是否奇偶校验：无
#define GPS_USART_CONTROL             USART_HardwareFlowControl_None      //硬件流控制模式设置：没有使能
#define GPS_USART_MODE                USART_Mode_Rx | USART_Mode_Tx       //接收与发送都使能
#define GPS_USART_IT_CONFIG           USART_IT_RXNE                       //使能串口接收中断

// 接收数据不为空中断
#define GPS_USART_IRQ                 USART3_IRQn 
#define GPS_USART_IRQHandler          USART3_IRQHandler

#define STR_END         "\r\n"          // 字符发送结束符
#define Buffer_Max  128                // 接收缓冲区大小


#define GPS_Buffer_Length               200
#define UTCTime_Length                  11
#define latitude_Length                 11
#define N_S_Length                      2    
#define longitude_Length                12
#define E_W_Length                      2 
#define Gps_Rx_Length					GPS_Buffer_Length


typedef struct 
{
	char GPS_Buffer[GPS_Buffer_Length]; //GPS数据缓冲区
	char isGetData;		                //是否获取到GPS数据
	char isParseData;	                //是否解析完成
	char UTCTime[UTCTime_Length];		//UTC时间
	char latitude[latitude_Length];		//纬度
	char N_S[N_S_Length];		        //N/S
	char longitude[longitude_Length];	//经度
	char E_W[E_W_Length];		        //E/W
	char isUsefull;		                //定位信息是否有效
}GpsData;

void Gps_Gpio_Init(void);
void Gps_Usart_Init(void);
void Gps_Usart_NVIC(void);
GpsData *Gps_Buf_Init(void);

void Gps_Buf_free(void);

void USART3_IRQHandler(void);
uint8_t Hand(const char *cmd);
void Gps_Rx_Buf_Clear(void);
void Gps_Data_Clear(void);

#endif
