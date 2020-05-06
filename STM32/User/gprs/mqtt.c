#include "mqtt.h"

extern volatile Task _task;

void Mqtt_Init()
{
    if(Mqtt_Status())
    {
		Debug_Info(Mqtt_TAG, "connect mqtt ok");
		_task.mqtt_s = 0x01;
		return;
    }
    if(Gprs_Restart()==false)
    {
        Debug_Info(Mqtt_TAG, "restart error");
		_task.mqtt_s = 0x0f;
		return;
    }
	Debug_Info(Mqtt_TAG, "restart");
	delay_ms(1000);
    if(Gprs_Ok()==false)
    {
		Debug_Info(Mqtt_TAG, "sim error");
		_task.mqtt_s = 0x02;
		return;
    }
	Debug_Info(Mqtt_TAG, "sim ready");
	delay_ms(1000);
    if(Gprs_Wait_Net()==false)
    {
		Debug_Info(Mqtt_TAG, "net error");
		_task.mqtt_s = 0x03;
		return;
    }
	Debug_Info(Mqtt_TAG, "net ready");
	delay_ms(1000);
    if(Mqtt_Status()==false)
    {
		Debug_Info(Mqtt_TAG, "connect mqtt error");
		_task.mqtt_s = 0x04;
		return;
    }
	Debug_Info(Mqtt_TAG, "mqtt ready");
    Debug_Info(Mqtt_TAG, "mqtt running...");
	_task.mqtt_s = 0x01;
	delay_ms(1000);
}

boolean Mqtt_Status()
{
    boolean flag;
	uint8_t cnt=0;
	Gprs_Clear_Buffer();
	while(cnt<5)
	{
		Debug_Info(Mqtt_TAG, "wait gprs connect mqtt");
		flag = Gprs_Write_AT_Cmd("AT+STATUS", "+STATUS: MQTT CONNECT OK",1, 2500);
		if(flag) return true;
		else cnt++;
	}
	return false;
}

boolean Mqtt_Publish(const char *topic, const char *data, uint16_t len, uint8_t qos)
{
    boolean flag;
	uint8_t cnt=0;
	char *str = NULL;
	if(Mqtt_Status()==false)
    {
		Debug_Info(Mqtt_TAG, "connect mqtt error");
		_task.mqtt_s = 0x04;
		return false;
    }
	Gprs_Clear_Buffer();
	Debug_Info(Mqtt_TAG, "wait mqtt publish");
	str = (char*) calloc(85, sizeof(char));
	sprintf(str, "%s=\"%s\",%d,%d","AT+PUBLISH",topic, len, qos);
	Debug_Info(Mqtt_TAG, str);
	flag = Gprs_Write_AT_Cmd(str, ">",1,1500);
	if(flag) {
		flag = Gprs_Write_AT_Cmd(data, "Publish OK",1,5000);
		if(flag) {
			Debug_Info(Mqtt_TAG, "publish ok");
		}else{
			Debug_Info(Mqtt_TAG, "publish failed");
		}
	} else {
		Debug_Info(Mqtt_TAG, "publish error");
	}

	free(str);
	return false;
}