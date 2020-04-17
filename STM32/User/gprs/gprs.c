#include "gprs.h"

char AT_End_Char[2] = {0x1A, 0x00};//结束字符
char Gprs_Server_Ip_Port_Cmd[45];

// Gprs模块初始化
void Gprs_Init(void)
{
    Gprs_Gpio_Init();
    Gprs_Usart_Init();
    Gprs_Usart_NVIC();
	Debug_Info(Gprs_TAG, "gprs running...");
}

// gprs发送AT指令
boolean Gprs_Write_AT_Cmd(const char *str, const char *ack, uint8_t wait_time, uint32_t interval_time)         
{
	uint8_t cnt = 0;
	while(cnt < wait_time)    //如果没有找到 就继续再发一次指令 再进行查找目标字符串                
	{
		Gprs_Write_Cmd(str);//串口2发送 b 字符串 他会自动发送\r\n  相当于发送了一个指令
		delay_ms(interval_time); //等待一定时间 传50的话就是 50*20ms = 1秒
		if(Find_Str(Gprs_Get_Buffer(), ack))  {          //查找需要应答的字符串ack
			return true;
		}
		cnt++;
	}
	
	return false;
}

// gprs发送AT结束指令
boolean Gprs_Write_AT_Cmd_End(const char *str, const char *ack, uint8_t wait_time, uint32_t interval_time)         
{
	uint8_t cnt = 0;
	while(cnt < wait_time)    //如果没有找到 就继续再发一次指令 再进行查找目标字符串                
	{
		Gprs_Write_Cmd_END(str);//串口2发送 b 字符串 这里不发送\r\n 
		delay_ms(interval_time); //等待一定时间 传50的话就是 50*20ms = 1秒
		if(Find_Str(Gprs_Get_Buffer(), ack))            //查找需要应答的字符串 a
		{
			return true;
		}
		cnt++;
	}
	return false;
}

// 等待注册
boolean Gprs_Wait_CREG(uint8_t query_times)
{
	uint8_t cnt = 0;
	uint8_t j = 0;
	Gprs_Clear_Buffer();
	while(cnt == 0)        			
	{
		Gprs_Write_Cmd("AT+CREG?");
		delay_ms(100); 
        if( Find_Str(Gprs_Get_Buffer(), CREG) ) return true;
		j++;
		if(j > query_times) return false;
	}
	return false;
}

// 连接服务器
int Gprs_Send_Data_to_Server(const char *server_IP_and_port, const char *content)
{
	boolean flag;
	
	flag = Gprs_Write_AT_Cmd("AT+CIPSTATUS","CONNECT OK",3,50*2);//查询连接状态
	if(flag)//说明服务器处于连接状态
	{
		flag = Gprs_Write_AT_Cmd("AT+CIPSEND",">",3,50);//开发发送数据
		if(flag == 0)
		{
			return AT_CIPSEND_ERROR;
		}
		
		Gprs_Wrtie_String(content);
		
		flag = Gprs_Write_AT_Cmd_End(AT_End_Char,"SEND OK",1,250);//发送结束符，等待返回ok,等待5S发一次，因为发送数据时间可能较长
		if(flag == 0)
		{
			return END_CHAR_ERROR;
		}
		
		return 1;
	}
	
	flag = Gprs_Write_AT_Cmd("AT+CGATT=1","OK",3,50*7);//附着网络
	if(flag == 0)
	{
		return AT_CGATT_ERROR;
	}
	
	flag = Gprs_Write_AT_Cmd("AT+CGACT=1,1","OK",3,50*2);//激活网络
	if(flag == 0)
	{
		return AT_CGATT1_ERROR;
	}

	memset(Gprs_Server_Ip_Port_Cmd,'\0',45);
	strcpy(Gprs_Server_Ip_Port_Cmd,"AT+CIPSTART=\"TCP\",");
	strcat(Gprs_Server_Ip_Port_Cmd,server_IP_and_port);
	
	flag = Gprs_Write_AT_Cmd(Gprs_Server_Ip_Port_Cmd,"CONNECT OK",3,50*2);//连接服务器
	if(flag == 0)
	{
		return AT_CIPSTART_ERROR;
	}
	
	flag = Gprs_Write_AT_Cmd("AT+CIPSEND",">",3,50);//开发发送数据
	if(flag == 0)
	{
		return AT_CIPSEND_ERROR;
	}
	
	Gprs_Wrtie_String(content);
	flag = Gprs_Write_AT_Cmd(AT_End_Char,"SEND OK",1,250);//发送结束符，等待返回ok,等待5S发一次，因为发送数据时间可能较长
    
    if(flag == 0)
	{
		return END_CHAR_ERROR;
	}
	
	return 1;
}
