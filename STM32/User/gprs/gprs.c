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
boolean Gprs_Write_AT_Cmd(const char *str, const char *ack, uint8_t wait_cnt, uint32_t interval_time)         
{
	uint8_t cnt = 0;
	while(cnt < wait_cnt)    //���û���ҵ� �ͼ����ٷ�һ��ָ�� �ٽ��в���Ŀ���ַ���                
	{
		Gprs_Write_Cmd(str);//����2���� b �ַ��� �����Զ�����\r\n  �൱�ڷ�����һ��ָ��
		delay_ms(interval_time); //�ȴ�һ��ʱ�� ��50�Ļ����� 50*20ms = 1��
		// Debug_Info(Gprs_TAG, "receive at");
		Debug_Info(Gprs_TAG, Gprs_Get_Buffer());
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
		Gprs_Write_Cmd_End(str);//����2���� b �ַ��� ���ﲻ����\r\n 
		delay_ms(interval_time); //�ȴ�һ��ʱ�� ��50�Ļ����� 50*20ms = 1��
		printf("receive:%s\n", Gprs_Get_Buffer());
		if(Find_Str(Gprs_Get_Buffer(), ack))            //������ҪӦ����ַ��� a
		{
			return true;
		}
		cnt++;
	}
	return false;
}

boolean Gprs_Restart()
{
	boolean flag;
	uint8_t cnt=0;
	Gprs_Clear_Buffer();
	while(cnt<5)
	{
		Debug_Info(Gprs_TAG, "wait gprs restart");
		flag = Gprs_Write_AT_Cmd("AT+CFUN=1,1", "AT Ready",1,2500);
		if(flag) return true;
		else cnt++;
		Gprs_Clear_Buffer();
	}
	return false;
}

boolean Gprs_Wait_Net()
{
	boolean flag;
	uint8_t cnt=0;
	char *buf = NULL;
	Gprs_Clear_Buffer();
	while(cnt<50)
	{
		Debug_Info(Gprs_TAG, "wait gprs connect net");
		delay_ms(1000);
		buf = Gprs_Get_Buffer();
		flag = Find_Str(buf, "+NITZ");
		if(flag) return true;
		else cnt++;
		Gprs_Clear_Buffer();

	}
	return false;
}

boolean Gprs_Ok()
{
	boolean flag;
	uint8_t cnt=0;
	Gprs_Clear_Buffer();
	while(cnt<5)
	{
		Debug_Info(Gprs_TAG, "wait gprs read sim");
		flag = Gprs_Write_AT_Cmd("AT+CPIN?", "READY",3,500);
		if(flag) return true;
		else cnt++;
		Gprs_Clear_Buffer();

	}
	return false;
}


// �ȴ�ע��
boolean Gprs_Set_CREG(uint8_t query_cnt)
{
	uint8_t cnt = 0;
	boolean flag;
	Gprs_Clear_Buffer();
	Debug_Info(Gprs_TAG, "�ȴ�ע������");
	while(cnt < query_cnt)        			
	{
		flag = Gprs_Write_AT_Cmd("AT+CGREG=1", "OK",3,500);
		if(flag) {
			Debug_Info(Gprs_TAG, "ע������ɹ�");
			return true;
		}
		else cnt++;
	}
	Debug_Info(Gprs_TAG, "ע������ʧ��");
	return false;
}

// �ȴ�ע��
boolean Gprs_CREG(uint8_t query_cnt)
{
	uint8_t cnt = 0;
	boolean flag;
	Gprs_Clear_Buffer();
	Debug_Info(Gprs_TAG, "�ȴ�ע������");
	while(cnt < query_cnt)        			
	{
		flag = Gprs_Write_AT_Cmd("AT+CGREG?", "+CREG: 1,1",3,500);
		if(flag) {
			Debug_Info(Gprs_TAG, "ע������ɹ�");
			return true;
		}
		else cnt++;
	}
	Debug_Info(Gprs_TAG, "ע������ʧ��");
	return false;
}

boolean Gprs_CSQ(uint8_t query_cnt) 
{
	uint8_t cnt = 0;
	boolean flag;
	Gprs_Clear_Buffer();
	Debug_Info(Gprs_TAG, "��ѯ�ź�ǿ��");
	while(cnt < query_cnt)        			
	{
		flag = Gprs_Write_AT_Cmd("AT+CSQ", "OK",3,500);
		if(flag) {
			Debug_Info(Gprs_TAG, "��ѯ�ź�ǿ�ȳɹ�");
			return true;
		}
		else cnt++;
	}
	Debug_Info(Gprs_TAG, "��ѯ�ź�ǿ��ʧ��");
	return false;
}

boolean Gprs_CGATT(uint8_t query_cnt)
{
	uint8_t cnt = 0;
	boolean flag;
	Gprs_Clear_Buffer();
	Debug_Info(Gprs_TAG, "����GPRS");
	while(cnt < query_cnt)        			
	{
		flag = Gprs_Write_AT_Cmd("AT+CGATT=1", "OK",3,500);
		if(flag) {
			Debug_Info(Gprs_TAG, "����GPRS�ɹ�");
			return true;
		}
		else cnt++;
	}
	Debug_Info(Gprs_TAG, "����GPRSʧ��");
	return false;
}

boolean Gprs_CIPMUX(uint8_t query_cnt)
{
	uint8_t cnt = 0;
	boolean flag;
	Gprs_Clear_Buffer();
	Debug_Info(Gprs_TAG, "���õ���·����");
	while(cnt < query_cnt)        			
	{
		flag = Gprs_Write_AT_Cmd("AT+CIPMUX=0", "OK",3,500);
		if(flag) {
			Debug_Info(Gprs_TAG, "���õ���·����ɹ�");
			return true;
		}
		else cnt++;
	}
	Debug_Info(Gprs_TAG, "���õ���·����ʧ��");
	return false;
}

boolean Gprs_CIPMODE(uint8_t query_cnt)
{
	uint8_t cnt = 0;
	boolean flag;
	Gprs_Clear_Buffer();
	Debug_Info(Gprs_TAG, "����͸��");
	while(cnt < query_cnt)        			
	{
		flag = Gprs_Write_AT_Cmd("AT+CIPMODE=1", "OK",3,500);
		if(flag) {
			Debug_Info(Gprs_TAG, "����͸���ɹ�");
			return true;
		}
		else cnt++;
	}
	Debug_Info(Gprs_TAG, "����͸��ʧ��");
	return false;
}

boolean Gprs_Activate_NetWork(uint8_t query_cnt)
{
	uint8_t cnt = 0;
	boolean flag;
	Gprs_Clear_Buffer();
	Debug_Info(Gprs_TAG, "��������");
	while(cnt < query_cnt)        			
	{
		flag = Gprs_Write_AT_Cmd("AT+CGACT=1,1", "OK",3,500);
		if(flag) {
			Debug_Info(Gprs_TAG, "��������ɹ�");
			return true;
		}
		else cnt++;
	}
	Debug_Info(Gprs_TAG, "��������ʧ��");
	return false;
}	

int Gprs_Close()
{
	boolean flag;
	flag = Gprs_Write_AT_Cmd("AT+CIPSTATUS","CONNECT OK",3,50*2);//��ѯ����״̬
	if(flag)//˵����������������״̬
	{
		Debug_Info(Gprs_TAG, "server connecting");
		flag = Gprs_Write_AT_Cmd("AT+CIPCLOSE","OK",3,50*2);//��ѯ����״̬
		return 1;
	}
}

int Gprs_Connect_Server(const char *server_IP, int port)
{
	boolean flag;
	char ip_port[25];
	sprintf(ip_port, "%s,%d",server_IP, port);
	flag = Gprs_Write_AT_Cmd("AT+CIPSTATUS","CONNECT OK",3,50*2);//��ѯ����״̬
	if(flag)//˵����������������״̬
	{
		Debug_Info(Gprs_TAG, "server connecting");
		return 1;
	}

	// if(Gprs_CREG(1)==false) 
	// {
	// 	Debug_Info(Gprs_TAG, "Gprs_CREG");
	// 	return AT_CGATT_ERROR;
	// }
	// if(Gprs_CSQ(1)==false)
	// {
	// 	Debug_Info(Gprs_TAG, "Gprs_CSQ");
	// 	return 0;
	// }
	// if(Gprs_CGATT(1)==false)
	// {
	// 	Debug_Info(Gprs_TAG, "Gprs_CGATT");
	// 	return 0;
	// }
	// if(Gprs_CIPMUX(1)==false)
	// {
	// 	Debug_Info(Gprs_TAG, "Gprs_CIPMUX");
	// 	return 0;
	// }
	// if(Gprs_CIPMODE(1)==false)
	// {
	// 	Debug_Info(Gprs_TAG, "Gprs_CIPMODE");
	// 	return 0;
	// }
	// if(Gprs_Activate_NetWork(1)==false)
	// {
	// 	Debug_Info(Gprs_TAG, "Gprs_Activate_NetWork");
	// 	return 0;
	// }
	
	flag = Gprs_Write_AT_Cmd("AT+CGATT=1","OK",3,1000);//��������
	if(flag == 0)
	{
		Debug_Info(Gprs_TAG, "��������ʧ��");
		return AT_CGATT_ERROR;
	}

	// flag = Gprs_Write_AT_Cmd("AT+CIPMUX=0","OK",3,50*7);//����·����
	// if(flag == 0)
	// {
	// 	Debug_Info(Gprs_TAG, "���õ���·����ʧ��");
	// 	return AT_CGATT_ERROR;
	// }	

	// flag = Gprs_Write_AT_Cmd("AT+CSTT=\"CMNET\"","OK",3,50*7);//����·����
	// if(flag == 0)
	// {
	// 	Debug_Info(Gprs_TAG, "cmnet");
	// 	return AT_CGATT_ERROR;
	// }

	
	// flag = Gprs_Write_AT_Cmd("AT+CIICR","OK",3,50*7);//����·����
	// if(flag == 0)
	// {
	// 	Debug_Info(Gprs_TAG, "ciicr");
	// 	return AT_CGATT_ERROR;
	// }
	
	flag = Gprs_Write_AT_Cmd("AT+CGACT=1,1","OK",5,1000);//��������
	if(flag == 0)
	{
		Debug_Info(Gprs_TAG, "server AT_CGATT1_ERROR");
		return AT_CGATT1_ERROR;
	}

	memset(Gprs_Server_Ip_Port_Cmd,'\0',45);
	strcpy(Gprs_Server_Ip_Port_Cmd,"AT+CIPSTART=\"TCP\",");
	strcat(Gprs_Server_Ip_Port_Cmd, ip_port);
	
	flag = Gprs_Write_AT_Cmd(Gprs_Server_Ip_Port_Cmd,"CONNECT OK", 3, 1000*2);//���ӷ�����
	// flag = Gprs_Write_AT_Cmd(Gprs_Server_Ip_Port_Cmd,"OK", 3, 50*2);//���ӷ�����
	if(flag == 0)
	{
		Debug_Info(Gprs_TAG, "server AT_CIPSTART_ERROR");
		return AT_CIPSTART_ERROR;
	}
	
	return 1;
}

// ���ӷ�����
int Gprs_Send_Data_to_Server(const char *server_IP_and_port, const char *content)
{
	boolean flag;
	
	flag = Gprs_Write_AT_Cmd("AT+CIPSTATUS","CONNECT OK",3,50*2);//��ѯ����״̬
	if(flag)//˵����������������״̬
	{
		Debug_Info(Gprs_TAG, "server connecting");
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

	// if(Gprs_CREG(1)==false) 
	// {
	// 	Debug_Info(Gprs_TAG, "Gprs_CREG");
	// 	return AT_CGATT_ERROR;
	// }
	// if(Gprs_CSQ(1)==false)
	// {
	// 	Debug_Info(Gprs_TAG, "Gprs_CSQ");
	// 	return 0;
	// }
	// if(Gprs_CGATT(1)==false)
	// {
	// 	Debug_Info(Gprs_TAG, "Gprs_CGATT");
	// 	return 0;
	// }
	// if(Gprs_CIPMUX(1)==false)
	// {
	// 	Debug_Info(Gprs_TAG, "Gprs_CIPMUX");
	// 	return 0;
	// }
	// if(Gprs_CIPMODE(1)==false)
	// {
	// 	Debug_Info(Gprs_TAG, "Gprs_CIPMODE");
	// 	return 0;
	// }
	// if(Gprs_Activate_NetWork(1)==false)
	// {
	// 	Debug_Info(Gprs_TAG, "Gprs_Activate_NetWork");
	// 	return 0;
	// }
	
	flag = Gprs_Write_AT_Cmd("AT+CGATT=1","OK",3,1000);//��������
	if(flag == 0)
	{
		Debug_Info(Gprs_TAG, "��������ʧ��");
		return AT_CGATT_ERROR;
	}

	// flag = Gprs_Write_AT_Cmd("AT+CIPMUX=0","OK",3,50*7);//����·����
	// if(flag == 0)
	// {
	// 	Debug_Info(Gprs_TAG, "���õ���·����ʧ��");
	// 	return AT_CGATT_ERROR;
	// }	

	// flag = Gprs_Write_AT_Cmd("AT+CSTT=\"CMNET\"","OK",3,50*7);//����·����
	// if(flag == 0)
	// {
	// 	Debug_Info(Gprs_TAG, "cmnet");
	// 	return AT_CGATT_ERROR;
	// }

	
	// flag = Gprs_Write_AT_Cmd("AT+CIICR","OK",3,50*7);//����·����
	// if(flag == 0)
	// {
	// 	Debug_Info(Gprs_TAG, "ciicr");
	// 	return AT_CGATT_ERROR;
	// }
	
	flag = Gprs_Write_AT_Cmd("AT+CGACT=1,1","OK",5,1000);//��������
	if(flag == 0)
	{
		Debug_Info(Gprs_TAG, "server AT_CGATT1_ERROR");
		return AT_CGATT1_ERROR;
	}

	memset(Gprs_Server_Ip_Port_Cmd,'\0',45);
	strcpy(Gprs_Server_Ip_Port_Cmd,"AT+CIPSTART=\"TCP\",");
	strcat(Gprs_Server_Ip_Port_Cmd,server_IP_and_port);
	
	flag = Gprs_Write_AT_Cmd(Gprs_Server_Ip_Port_Cmd,"CONNECT OK", 3, 1000*2);//���ӷ�����
	// flag = Gprs_Write_AT_Cmd(Gprs_Server_Ip_Port_Cmd,"OK", 3, 50*2);//���ӷ�����
	if(flag == 0)
	{
		Debug_Info(Gprs_TAG, "server AT_CIPSTART_ERROR");
		return AT_CIPSTART_ERROR;
	}
	
	flag = Gprs_Write_AT_Cmd("AT+CIPSEND",">",3,1000);//��ʼ��������
	if(flag == 0)
	{
		Debug_Info(Gprs_TAG, "server AT_CIPSEND_ERROR");
		return AT_CIPSEND_ERROR;
	}
	
	Gprs_Wrtie_String(content);
	flag = Gprs_Write_AT_Cmd(AT_End_Char,"SEND OK",1,1000);//���ͽ��������ȴ�����ok,�ȴ�5S��һ�Σ���Ϊ��������ʱ����ܽϳ�
    
    if(flag == 0)
	{
		Debug_Info(Gprs_TAG, "server END_CHAR_ERROR");
		return END_CHAR_ERROR;
	}

	flag = Gprs_Write_AT_Cmd("AT+CIPCLOSE","CLOSE OK",1,1000);//���ͽ��������ȴ�����ok,�ȴ�5S��һ�Σ���Ϊ��������ʱ����ܽϳ�
    
    if(flag == 0)
	{
		Debug_Info(Gprs_TAG, "server CLOSE ");
		return END_CHAR_ERROR;
	}
	
	return 1;
}
