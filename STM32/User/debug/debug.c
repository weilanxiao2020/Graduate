#include "debug.h"

char Debug_Receive_Buf[Debug_Buf_Length];
char Debug_Receive_Head;
char Debug_Send_Buf[Debug_Buf_Length];
char Debug_Send_Head;

void Debug_Init(void)
{
#ifdef GZ_DEBUG
    Debug_Gpio_Init();
    Debug_Usart_Init();
    Debug_Usart_NVIC();
    Debug_Info(Debug_TAG, "debug start...");
    Debug_Info(Debug_TAG, "debug running...");
#endif
}

void Debug_Info(const char *tag, char *msg)
{
#ifdef GZ_DEBUG
    Buf_Clear(Debug_Receive_Buf, Debug_Buf_Length);
    sprintf(Debug_Send_Buf, "[%s:info]=> %s\r\n", tag, msg);
    printf(Debug_Send_Buf);
#endif
}

void Debug_Error(const char *tag, char *msg)
{
#ifdef GZ_DEBUG
    Buf_Clear(Debug_Receive_Buf, Debug_Buf_Length);
    sprintf(Debug_Send_Buf, "[%s:error]=> %s\r\n", tag, msg);
    printf(Debug_Send_Buf);
#endif
}

//==============================================================//
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE
{
	int handle;
 
};
FILE __stdout;
 
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x)
{
	x = x;
}

//__use_no_semihosting was requested, but _ttywrch was 
_ttywrch(int ch)
{
	ch = ch;
}

int fputc(int ch, FILE *f)
{
#ifdef GZ_DEBUG
    Usart_SendByte(Debug_Usart, (uint8_t)ch);
    while(USART_GetFlagStatus(Debug_Usart, USART_FLAG_TXE) == RESET);
    return (ch);
#endif
}

int fgetc(FILE *f)
{
#ifdef GZ_DEBUG
    while(USART_GetFlagStatus(Debug_Usart, USART_FLAG_RXNE) == RESET);
    return (int)USART_ReceiveData(Debug_Usart);
#endif
}
