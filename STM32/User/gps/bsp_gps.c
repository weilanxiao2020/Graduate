#include "bsp_gps.h"
#include <stdlib.h>

void Gps_Gpio_Init(void)
{
	GPIO_InitTypeDef gpio_Init;
	/* 打开USART的GPIO时钟*/
	// TX发送
	GPS_USART_GPIO_TX_CLK_FUN(GPS_USART_GPIO_TX_CLK, ENABLE);
	gpio_Init.GPIO_Pin = GPS_USART_GPIO_TX_PIN;
	gpio_Init.GPIO_Mode = GPS_USART_GPIO_TX_MODE;
	gpio_Init.GPIO_Speed = GPS_USART_GPIO_TX_SPEED;
	GPIO_Init(GPS_USART_GPIO_TX_PORT, &gpio_Init);

	// RX接收
	GPS_USART_GPIO_RX_CLK_FUN(GPS_USART_GPIO_RX_CLK, ENABLE);
	gpio_Init.GPIO_Pin = GPS_USART_GPIO_RX_PIN;
	gpio_Init.GPIO_Mode = GPS_USART_GPIO_RX_MODE;
	GPIO_Init(GPS_USART_GPIO_RX_PORT, &gpio_Init);
    Debug_Info(Bsp_Gps_TAG, "bsp gps gpio init");
}

void Gps_Usart_Init(void)
{
    USART_InitTypeDef usart_Init;
	/* 打开USART1外设功能时钟 */
	GPS_USART_CLK_FUN(GPS_USART_CLK, ENABLE);
    /* USART1 工作模式配置 */
	usart_Init.USART_BaudRate = GPS_USART_BAUDRATE;	
	usart_Init.USART_WordLength = GPS_USART_WORDLEN;	
	usart_Init.USART_StopBits = GPS_USART_STOPBITS; 	
	usart_Init.USART_Parity = GPS_USART_PARITY ;  
	usart_Init.USART_HardwareFlowControl = GPS_USART_CONTROL;	
	usart_Init.USART_Mode = GPS_USART_MODE;
	USART_Init(GPS_USART, &usart_Init);  
	USART_ITConfig(GPS_USART, GPS_USART_IT_CONFIG,ENABLE);
	USART_Cmd(GPS_USART, ENABLE);                    			//使能串口 
	USART_ClearFlag(GPS_USART, USART_FLAG_TC);					//清除发送完成标志
    Debug_Info(Bsp_Gps_TAG, "bsp gps usart init");
}

void Gps_Usart_NVIC(void)
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
	nvic_Init.NVIC_IRQChannel = GPS_USART_IRQ;
	nvic_Init.NVIC_IRQChannelPreemptionPriority = 3;
	nvic_Init.NVIC_IRQChannelSubPriority = 3;
	nvic_Init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_Init);
    Debug_Info(Bsp_Gps_TAG, "bsp gps nvic init");
}

static GpsData *ptrGpsData = NULL;

GpsData *Gps_Buf_Init(void)
{
	if(ptrGpsData!=NULL) return ptrGpsData;
	ptrGpsData = (GpsData *)calloc(1, sizeof(GpsData));
	return ptrGpsData;
}

void Gps_Buf_free(void)
{
	if(ptrGpsData == NULL) return;
	free(ptrGpsData);
}


uint16_t point1;
char Gps_Rx_Buf[Gps_Rx_Length];
// GpsData gpsData;
void USART3_IRQHandler(void)
{
    byte result;
	if(USART_GetITStatus(GPS_USART, USART_IT_RXNE) != RESET) 
	{
        result =USART_ReceiveData(GPS_USART);//(USART1->DR);	//读取接收到的数据

        if(result == '$')
        {
            point1 = 0;	
        }

        Gps_Rx_Buf[point1++] = result;

        if(Gps_Rx_Buf[0] == '$' && Gps_Rx_Buf[4] == 'M' && Gps_Rx_Buf[5] == 'C')			//确定是否收到"GPRMC/GNRMC"这一帧数据
        {
            if(result == '\n')									   
            {
				if(ptrGpsData!=NULL)
				{
					memset(ptrGpsData->GPS_Buffer, 0, GPS_Buffer_Length);      //清空
					memcpy(ptrGpsData->GPS_Buffer, Gps_Rx_Buf, point1); 	//保存数据
					ptrGpsData->isGetData = true;
					point1 = 0;
					memset(Gps_Rx_Buf, 0, Gps_Rx_Length);      //清空	
				}
            }	
                
        }

        if(point1 >= Gps_Rx_Length)
        {
            point1 = Gps_Rx_Length;
        }	
    }
}

// 串口命令识别函数
uint8_t Hand(const char *cmd)
{ 
    if(strstr(Gps_Rx_Buf,cmd)!=NULL)
	    return 1;
	else
		return 0;
}

// 清口GPS接收串口缓存
void Gps_Rx_Buf_Clear(void)                           // 串口缓存清理
{
	memset(Gps_Rx_Buf, 0, Gps_Rx_Length);      //清空
    point1 = 0;                    
}

// 清空结构体
void Gps_Data_Clear()
{
	ptrGpsData->isGetData = false;
	ptrGpsData->isParseData = false;
	ptrGpsData->isUsefull = false;
	memset(ptrGpsData->GPS_Buffer, 0, GPS_Buffer_Length);      //清空
	memset(ptrGpsData->UTCTime, 0, UTCTime_Length);
	memset(ptrGpsData->latitude, 0, latitude_Length);
	memset(ptrGpsData->N_S, 0, N_S_Length);
	memset(ptrGpsData->longitude, 0, longitude_Length);
	memset(ptrGpsData->E_W, 0, E_W_Length);
	
}
