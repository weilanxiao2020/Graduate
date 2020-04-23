#ifndef __BSP_GPRS_H
#define __BSP_GPRS_H

#include "stm32f10x.h"
#include "debug.h"

#ifdef GZ_DEBUG
    #define Bsp_Gprs_TAG       "bsp_gprs"
#else 
    #define Bsp_Gprs_TAG       ""  
#endif

/* usartӲ��io�� */
/*				usart2��sim900a
				1. GND ��Դ��
				2. VCC ��Դ����3��5.5V��
PA2(USART2_TX)	3. rx ���ݽ��գ���stm32��tx
PA3(USART3_RX)	4. tx ���ݷ��ͣ���stm32��rx
*/

/*********************************** USART2 GPIO ʹ�����Ŷ��� *********************************************/
//����:PA2��STM32��USART2_TX���ţ����͸�ga6������
#define GPRS_USART_GPIO_TX_CLK_FUN     RCC_APB2PeriphClockCmd
#define GPRS_USART_GPIO_TX_CLK         RCC_APB2Periph_GPIOA
#define GPRS_USART_GPIO_TX_PORT        GPIOA			   
#define GPRS_USART_GPIO_TX_PIN		   GPIO_Pin_2
#define GPRS_USART_GPIO_TX_MODE        GPIO_Mode_AF_PP
#define GPRS_USART_GPIO_TX_SPEED       GPIO_Speed_50MHz

//����:PA3��STM32��USART2_RX���ţ�����ga6���͵�����
#define GPRS_USART_GPIO_RX_CLK_FUN     RCC_APB2PeriphClockCmd
#define GPRS_USART_GPIO_RX_CLK         RCC_APB2Periph_GPIOA
#define GPRS_USART_GPIO_RX_PORT        GPIOA			   
#define GPRS_USART_GPIO_RX_PIN		   GPIO_Pin_3
#define GPRS_USART_GPIO_RX_MODE        GPIO_Mode_IN_FLOATING

/*********************************** USART2 �����ж϶��� *********************************************/
#define GPRS_USART                     USART2  
#define GPRS_USART_CLK_FUN             RCC_APB1PeriphClockCmd
#define GPRS_USART_CLK                 RCC_APB1Periph_USART2  
#define GPRS_USART_BAUDRATE            115200                              //���������ã�115200
#define GPRS_USART_WORDLEN             USART_WordLength_8b                 //����λ�����ã�8λ
#define GPRS_USART_STOPBITS            USART_StopBits_1                    //ֹͣλ���ã�1λ
#define GPRS_USART_PARITY              USART_Parity_No                     //�Ƿ���żУ�飺��
#define GPRS_USART_CONTROL             USART_HardwareFlowControl_None      //Ӳ��������ģʽ���ã�û��ʹ��
#define GPRS_USART_MODE                USART_Mode_Rx | USART_Mode_Tx       //�����뷢�Ͷ�ʹ��
#define GPRS_USART_IT_CONFIG           USART_IT_RXNE                       //ʹ�ܴ��ڽ����ж�

// �������ݲ�Ϊ���ж�
#define GPRS_USART_IRQ                 USART2_IRQn 
#define GPRS_USART_IRQHandler          USART2_IRQHandler

#define STR_END         "\r\n"          // �ַ����ͽ�����
#define Buffer_Max      128                // ���ջ�������С

/* ��ʼ������ */
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
