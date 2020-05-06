#include "stm32f10x.h"
#include "common.h"
#include "constants.h"
#include "bsp_led.h"
#include "debug.h"
#include "oled.h"
#include "rfid.h"
#include "gps.h"
#include "gprs.h"
#include "bsp_tim.h"
#include "MQTTPacket.h"
#include "transport.h"
#include "callback.h"
#include "mqtt.h"
#include "task.h"
#include <stdlib.h>

void Task_Rfid_Read_Test()
{
	RfidType *data;
	char *str = (char *)malloc(17);
	Debug_Info("main", "开始读数据");
	data = Rfid_Read_TrackId_Address_Status('A');
	Debug_Info("main", "read trackId %s", data->pData->trackId);
	Debug_Info("main", "read address %s", data->pData->address);
	Debug_Info("main", "read status 0x%x", data->pData->status);
	sprintf(str, "%s", data->pData->trackId);
	Oled_Show_String(0,3,str);
	free(str);
	Debug_Info("main", "Rfid操作完毕");
	free(data->pData);
	free(data);
}


void Task_Rfid_Write_Test(const char *data)
{
	RfidType *rfid = (RfidType *)calloc(1, sizeof(RfidType));
	Model *model = (Model *)calloc(1, sizeof(Model));
	uint8_t len;
	Debug_Info("main", "开始写数据");
	len = sscanf(data, "%[^,],%[^,],0x%02x", model->trackId, model->address, &(model->status));
	if(len!=3) {
		Debug_Info("main", "输入数据格式错误");
	} else {
		if(strlen(model->trackId)>Model_TrackId_Len ||
			strlen(model->address)>Model_Address_Len) {
			Debug_Info("main", "输入数据错误");
		} else {
			Debug_Info("main", "trackId:%s",model->trackId);
			Debug_Info("main", "address:%s",model->address);
			Debug_Info("main", "status:0x%02x",model->status);
			rfid->pData = model;
			rfid->cardType = 'A';
			Rfid_Write_TrackId_Address_Status(rfid);
			Debug_Info("main", "Rfid操作完毕");
		}
	}

	free(model);
	free(rfid);
}

void Gps(void)
{
	while(1)
	{
		Gps_Data_Parse();
		Gps_Data_Print();
	}
} 

void Oled_Test(void)
{
	Oled_Clear();
	Oled_Show_Chinese(0,0,0);//中
	Oled_Show_Chinese(18,0,1);//景
	Oled_Show_Chinese(36,0,2);//园
	Oled_Show_Chinese(54,0,3);//电
	Oled_Show_Chinese(72,0,4);//子
	Oled_Show_Chinese(90,0,5);//科
	Oled_Show_Chinese(108,0,6);//技
	Oled_Show_String(63,6,"spi1:233");  
}

int toStop = 0;

#ifdef Mqtt
int Mqtt_Test(void)
{
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	int rc = 0;
	int mysock = 0;
	unsigned char buf[200];
	int buflen = sizeof(buf);
	int msgid = 1;
	MQTTString topicString = MQTTString_initializer;
	int req_qos = 0;
	char* payload = "mypayload";
	int payloadlen = strlen(payload);
	int len = 0;
	char *host = "tcp://a1Tdu6Ry02R.iot-as-mqtt.cn-shanghai.aliyuncs.com";
	int port = 1883;

	mysock = transport_open(host, port);
	if(mysock < 0)
		return mysock;

	printf("Sending to hostname %s port %d\n", host, port);

	data.clientID.cstring = "a1Tdu6Ry02R.graduate_node_1|timestamp=1587201650400,_v=paho-java-1.0.0,securemode=2,signmethod=hmacsha256|";
	data.keepAliveInterval = 20;
	data.cleansession = 1;
	data.username.cstring = "graduate_node_1&a1Tdu6Ry02R";
	data.password.cstring = "65b652ba8821ad886b5b172ce53bd91b6b370597919ee582723978147afb6fb0";

	len = MQTTSerialize_connect(buf, buflen, &data);
	rc = transport_sendPacketBuffer(mysock, buf, len);

	/* wait for connack */
	if (MQTTPacket_read(buf, buflen, transport_getdata) == CONNACK)
	{
		unsigned char sessionPresent, connack_rc;

		if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0)
		{
			printf("Unable to connect, return code %d\n", connack_rc);
			goto exit;
		}
	}
	else
		goto exit;

	/* subscribe */
	// topicString.cstring = "substopic";
	// len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);

	// rc = transport_sendPacketBuffer(mysock, buf, len);
	// if (MQTTPacket_read(buf, buflen, transport_getdata) == SUBACK) 	/* wait for suback */
	// {
	// 	unsigned short submsgid;
	// 	int subcount;
	// 	int granted_qos;

	// 	rc = MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
	// 	if (granted_qos != 0)
	// 	{
	// 		printf("granted qos != 0, %d\n", granted_qos);
	// 		goto exit;
	// 	}
	// }
	// else
	// 	goto exit;

	/* loop getting msgs on subscribed topic */
	topicString.cstring = "/a1Tdu6Ry02R/graduate_node_1/user/card_info_post_test";
	
	while (!toStop)
	{
		/* transport_getdata() has a built-in 1 second timeout,
		your mileage will vary */
		if (MQTTPacket_read(buf, buflen, transport_getdata) == PUBLISH)
		{
			unsigned char dup;
			int qos;
			unsigned char retained;
			unsigned short msgid;
			int payloadlen_in;
			unsigned char* payload_in;
			int rc;
			MQTTString receivedTopic;

			rc = MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,
					&payload_in, &payloadlen_in, buf, buflen);
			printf("message arrived %d,%*s\n", payloadlen_in, payload_in);
		}

		printf("publishing reading\n");
		len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
		rc = transport_sendPacketBuffer(mysock, buf, len);
	}

	printf("disconnecting\n");
	len = MQTTSerialize_disconnect(buf, buflen);
	rc = transport_sendPacketBuffer(mysock, buf, len);

exit:
	transport_close(mysock);
}
#endif

void Mqtt()
{
	char *data = (char*) calloc(10, sizeof(char*));
	data = (char*)u32_to_str(123456789);
	Mqtt_Init();
	Mqtt_Publish(Topic_Card_Test, data,10,0);
}

extern volatile uint8_t cmd_task;
extern char Data_Buf[Debug_Rx_Length];
extern uint8_t dataLen;
int main(void)
{
	char cmdBuf[16];
	int8_t gps_index = -1;
	/*-------------------模块初始化-------------------*/
	SystemInit();
	SysTick_init();
	Debug_Init();
	Oled_Init();
    Led_Init();
	Rfid_Init();
	Gprs_Init();
	Gps_Init();
	Tim2_1ms_Init();
    Tim2_NVIC();
	Tim2_Callback_Init();
	Tim2_Start();
	Mqtt_Init();
	Oled_Clear();

	Task_Status();
	/*-------------------打印系统信息-------------------*/
	Debug_Error("main", "GraduationProject");
	Debug_Error("main", "魏蓝骁");
	Debug_Error("main", "QQ:1187438570");

	/*-------------------模块功能准备-------------------*/
	//Oled_Display_On();
	// printf("gprs server result:%d\n", Gprs_Send_Data_to_Server());
	/*-------------------模块功能开始-------------------*/
	
	// Oled_Test();
	// Mqtt();
	while(1)
	{	
		// Task_Status();
		switch (cmd_task)
		{
			case CMD_RFID_R:
				Debug_Info("main", Data_Buf); 
				Debug_Info("main", "rfid读取");
				// Task_Rfid_Read();
				Task_Rfid_Scan();
				cmd_task = 0x00;
				break;
			case CMD_RFID_W:
				Debug_Info("main", Data_Buf); 
				Debug_Info("main", "rfid写入");
				Task_Rfid_Write(Data_Buf);
				Debug_Info("main", "rfid完成");
				cmd_task = 0x00;
				break;
			case CMD_GPS_S:
				Debug_Info("main", "开启gps");
				gps_index = Tim2_Callback_Add(Gps_Data_Get);
				if(gps_index == -1) {
					Debug_Info("main", "gps回调设置失败");	
				}
				Task_Gps_S();
				cmd_task = 0x00;
				break;
			case CMD_GPS_E:
				Debug_Info("main", "关闭gps");
				if(Tim2_Callback_Remove(Gps_Data_Get) == -1) {
					Debug_Info("main", "gps回调移除失败");	
				}
				Task_Gps_E();
				cmd_task = 0x00;
				break;
			case CMD_RFID_E:
				Debug_Info("main", "结束Rfid");
				Task_Gps_E();
				cmd_task = 0x00;
				break;
			default:
				break;
		}
	}

	return 0;
}
