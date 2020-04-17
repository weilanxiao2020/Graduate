#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "constants.h"

/* 发送一个字节数据 */
void Usart_SendByte(USART_TypeDef* pUsart, uint8_t data);
void Usart_SendHalfWorld(USART_TypeDef* pUsart, uint16_t data);
/* 接收一个字节数据 */
// boolean Usart_GetByte(USART_TypeDef* pUsart, uint8_t* data);
byte Usart_GetByte(USART_TypeDef* pUsart);
void Usart_Send(USART_TypeDef* pUsart, uint8_t* data, uint8_t size);
void Usart_SendStr(USART_TypeDef* pUsart, const char* data);

#endif
