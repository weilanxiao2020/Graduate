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

uint16_t ptr;
char Debug_Rx_Buf[Debug_Rx_Length];
volatile uint8_t cmd_task;
uint8_t dataLen;
char Data_Buf[Debug_Rx_Length];
void USART1_IRQHandler(void)
{
    uint8_t result;
	if(USART_GetITStatus(DEBUG_USART, USART_IT_RXNE) != RESET) 
	{
        USART_ClearITPendingBit(DEBUG_USART,USART_IT_RXNE);
        result = USART_ReceiveData(DEBUG_USART);//(USART1->DR);	//读取接收到的数据
        if(result != '<') {
            // USART_SendData(DEBUG_USART, result);
            Debug_Rx_Buf[ptr++] = result;
            // Debug_Info(Debug_TAG, Debug_Rx_Buf);
        }
        else {
            Debug_Info(Debug_TAG, Debug_Rx_Buf);
            Debug_Info(Debug_TAG, "end"); 
            if(parseCMD_Rfid_R()) {	
                // 读取rfid
                dataLen = ptr-7;
                cmd_task = 0x03;
                memset(Data_Buf, 0, Debug_Rx_Length);      //清空
				memcpy(Data_Buf, Debug_Rx_Buf+7, dataLen); 	//保存数据
                Debug_Error(Debug_TAG, "cmd:%x,%s", cmd_task, Data_Buf);
            } else if(parseCMD_Rfid_W()) {
                // 写入rfid
                dataLen = ptr-7;
                cmd_task = 0x04;
                memset(Data_Buf, 0, Debug_Rx_Length);      //清空
				memcpy(Data_Buf, Debug_Rx_Buf+7, dataLen); 	//保存数据
                Debug_Error(Debug_TAG, "cmd:%x,%s", cmd_task, Data_Buf);
            } else if (parseCMD_Gps_S()) {
                dataLen = ptr-6;
                cmd_task = 0x05;
                memset(Data_Buf, 0, Debug_Rx_Length);      //清空
                Debug_Error(Debug_TAG, "cmd:%x,%s", cmd_task, Data_Buf);
            } else if (parseCMD_Gps_E()) {
                dataLen = ptr-6;
                cmd_task = 0x06;
                memset(Data_Buf, 0, Debug_Rx_Length);      //清空
                Debug_Error(Debug_TAG, "cmd:%x,%s", cmd_task, Data_Buf);
            }
            
            ptr = 0;
            memset(Debug_Rx_Buf, 0, Debug_Rx_Length);      //清空
        }
    }
}

uint8_t parseCMD_Rfid_R()
{
    if(Debug_Rx_Buf[0] == 'r' && 
                Debug_Rx_Buf[1] == 'f' && 
                Debug_Rx_Buf[2] == 'i' && 
                Debug_Rx_Buf[3] == 'd' &&
                Debug_Rx_Buf[4] == '|' &&
                Debug_Rx_Buf[5] == 'R' &&
                Debug_Rx_Buf[6] == '>')
    return 1;
    else return 0;
}


uint8_t parseCMD_Rfid_W()
{
    if(Debug_Rx_Buf[0] == 'r' && 
                Debug_Rx_Buf[1] == 'f' && 
                Debug_Rx_Buf[2] == 'i' && 
                Debug_Rx_Buf[3] == 'd' &&
                Debug_Rx_Buf[4] == '|' &&
                Debug_Rx_Buf[5] == 'W' &&
                Debug_Rx_Buf[6] == '>')
    return 1;
    else return 0;
}

uint8_t parseCMD_Gps_S()
{
    if(Debug_Rx_Buf[0] == 'g' && 
            Debug_Rx_Buf[1] == 'p' && 
            Debug_Rx_Buf[2] == 's' && 
            Debug_Rx_Buf[3] == '|' &&
            Debug_Rx_Buf[4] == 'S' &&
            Debug_Rx_Buf[5] == '>')
    return 1;
    else return 0;
}

uint8_t parseCMD_Gps_E()
{
    if(Debug_Rx_Buf[0] == 'g' && 
            Debug_Rx_Buf[1] == 'p' && 
            Debug_Rx_Buf[2] == 's' && 
            Debug_Rx_Buf[3] == '|' &&
            Debug_Rx_Buf[4] == 'E' &&
            Debug_Rx_Buf[5] == '>')
    return 1;
    else return 0;
}

void logI(const char *type, const char *func, const char *file, const char *format, ...)
{
#ifdef GZ_DEBUG
    va_list ap;
    va_start(ap, format);
    Buf_Clear((byte *)Debug_Receive_Buf, Debug_Buf_Length);
    vsnprintf(Debug_Receive_Buf, sizeof(Debug_Receive_Buf), format, ap);
    va_end(ap);
    printf("[%s:Info] [%s]=> %s\r\n", type, func, Debug_Receive_Buf);
#endif
}

void logE(const char *type, const char *func, const char *file, const char *format, ...)
{
#ifdef GZ_DEBUG
    va_list ap;
    va_start(ap, format);
    Buf_Clear((byte *)Debug_Receive_Buf, Debug_Buf_Length);
    vsnprintf(Debug_Receive_Buf, sizeof(Debug_Receive_Buf), format, ap);
    va_end(ap);
    printf("[%s:Error] [%s]=> %s\r\n", type, func, Debug_Receive_Buf);
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
