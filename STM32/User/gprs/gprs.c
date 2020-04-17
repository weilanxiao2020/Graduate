#include "gprs.h"

char AT_End_Char[2] = {0x1A, 0x00};//�����ַ�
char Gprs_Server_Ip_Port_Cmd[45];

// Gprsģ���ʼ��
void Gprs_Init(void)
{
    Gprs_Gpio_Init();
    Gprs_Usart_Init();
    Gprs_Usart_NVIC();
	Debug_Info(Gprs_TAG, "gprs running...");
}

// gprs����ATָ��
boolean Gprs_Write_AT_Cmd(const char *str, const char *ack, uint8_t wait_time, uint32_t interval_time)         
{
	uint8_t cnt = 0;
	while(cnt < wait_time)    //���û���ҵ� �ͼ����ٷ�һ��ָ�� �ٽ��в���Ŀ���ַ���                
	{
		Gprs_Write_Cmd(str);//����2���� b �ַ��� �����Զ�����\r\n  �൱�ڷ�����һ��ָ��
		delay_ms(interval_time); //�ȴ�һ��ʱ�� ��50�Ļ����� 50*20ms = 1��
		if(Find_Str(Gprs_Get_Buffer(), ack))  {          //������ҪӦ����ַ���ack
			return true;
		}
		cnt++;
	}
	
	return false;
}

// gprs����AT����ָ��
boolean Gprs_Write_AT_Cmd_End(const char *str, const char *ack, uint8_t wait_time, uint32_t interval_time)         
{
	uint8_t cnt = 0;
	while(cnt < wait_time)    //���û���ҵ� �ͼ����ٷ�һ��ָ�� �ٽ��в���Ŀ���ַ���                
	{
		Gprs_Write_Cmd_END(str);//����2���� b �ַ��� ���ﲻ����\r\n 
		delay_ms(interval_time); //�ȴ�һ��ʱ�� ��50�Ļ����� 50*20ms = 1��
		if(Find_Str(Gprs_Get_Buffer(), ack))            //������ҪӦ����ַ��� a
		{
			return true;
		}
		cnt++;
	}
	return false;
}

// �ȴ�ע��
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

// ���ӷ�����
int Gprs_Send_Data_to_Server(const char *server_IP_and_port, const char *content)
{
	boolean flag;
	
	flag = Gprs_Write_AT_Cmd("AT+CIPSTATUS","CONNECT OK",3,50*2);//��ѯ����״̬
	if(flag)//˵����������������״̬
	{
		flag = Gprs_Write_AT_Cmd("AT+CIPSEND",">",3,50);//������������
		if(flag == 0)
		{
			return AT_CIPSEND_ERROR;
		}
		
		Gprs_Wrtie_String(content);
		
		flag = Gprs_Write_AT_Cmd_End(AT_End_Char,"SEND OK",1,250);//���ͽ��������ȴ�����ok,�ȴ�5S��һ�Σ���Ϊ��������ʱ����ܽϳ�
		if(flag == 0)
		{
			return END_CHAR_ERROR;
		}
		
		return 1;
	}
	
	flag = Gprs_Write_AT_Cmd("AT+CGATT=1","OK",3,50*7);//��������
	if(flag == 0)
	{
		return AT_CGATT_ERROR;
	}
	
	flag = Gprs_Write_AT_Cmd("AT+CGACT=1,1","OK",3,50*2);//��������
	if(flag == 0)
	{
		return AT_CGATT1_ERROR;
	}

	memset(Gprs_Server_Ip_Port_Cmd,'\0',45);
	strcpy(Gprs_Server_Ip_Port_Cmd,"AT+CIPSTART=\"TCP\",");
	strcat(Gprs_Server_Ip_Port_Cmd,server_IP_and_port);
	
	flag = Gprs_Write_AT_Cmd(Gprs_Server_Ip_Port_Cmd,"CONNECT OK",3,50*2);//���ӷ�����
	if(flag == 0)
	{
		return AT_CIPSTART_ERROR;
	}
	
	flag = Gprs_Write_AT_Cmd("AT+CIPSEND",">",3,50);//������������
	if(flag == 0)
	{
		return AT_CIPSEND_ERROR;
	}
	
	Gprs_Wrtie_String(content);
	flag = Gprs_Write_AT_Cmd(AT_End_Char,"SEND OK",1,250);//���ͽ��������ȴ�����ok,�ȴ�5S��һ�Σ���Ϊ��������ʱ����ܽϳ�
    
    if(flag == 0)
	{
		return END_CHAR_ERROR;
	}
	
	return 1;
}
