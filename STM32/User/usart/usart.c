#include "usart.h"

// 发送数据，最大长度size
void Usart_Send(USART_TypeDef* pUsart, uint8_t* data, uint8_t size)
{
	uint8_t i;
	for(i = 0; i < size; i++)
	{
		Usart_SendByte(pUsart, *(data+i));
	}
	while(USART_GetFlagStatus(pUsart, USART_FLAG_TC) == RESET);	    
}

// 发送字符串
void Usart_SendStr(USART_TypeDef* pUsart, const char* data)
{
	uint8_t i=0;
	do{
		Usart_SendByte(pUsart, (uint8_t)*(data+i));
		i++;
	} while(*(data+i)!='\0');
	while(USART_GetFlagStatus(pUsart, USART_FLAG_TC) == RESET);	       
}

// 发送一个字节数据
void Usart_SendByte(USART_TypeDef* pUsart, uint8_t data)
{	   
	USART_SendData(pUsart, data);
	while(USART_GetFlagStatus(pUsart, USART_FLAG_TXE) == RESET);	    
}  

// // 接收一个字节数据
// boolean Usart_GetByte(USART_TypeDef* pUsart, uint8_t* data)
// {   	   
// 	if(USART_GetFlagStatus(pUsart, USART_FLAG_RXNE) == RESET)
// 	{
// 		return false;//没有收到数据 
// 	}
// 	*data = USART_ReceiveData(pUsart); 
// 	return true;//收到数据
// }

// 接收一个字节数据
byte Usart_GetByte(USART_TypeDef* pUsart)
{   	   
	while(USART_GetFlagStatus(pUsart, USART_FLAG_RXNE) == RESET);
	return USART_ReceiveData(pUsart); 
}
