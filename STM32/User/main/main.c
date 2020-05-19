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
#include "eeprom.h"
#include <stdlib.h>

#ifdef GZ_DEBUG
	#define Main_TAG 			"main"
#else 
	#define Main_TAG			""
#endif

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
	E2prom_Init();
	Tim2_1ms_Init();
    Tim2_NVIC();
	Tim2_Callback_Init();
	Tim2_Start();
	// Tim2_Callback_Add(Task_Status);
	Key_Init();
	Oled_Init();
    Led_Init();
	Task_Status();
	// Task_Btn_Init();
	Rfid_Init();
	Gprs_Init();
	Gps_Init();

	Mqtt_Init();
	

	/*-------------------打印系统信息-------------------*/
	Debug_Error(Main_TAG, "GraduationProject");
	Debug_Error(Main_TAG, "魏蓝骁");
	Debug_Error(Main_TAG, "2016212677");

	/*-------------------模块功能准备-------------------*/
	//Oled_Display_On();
	// printf("gprs server result:%d\n", Gprs_Send_Data_to_Server());
	/*-------------------模块功能开始-------------------*/
	
	// E2prom_Write_Byte(0xA2, 8, 0x26);
	// Debug_Info(Main_TAG, "eeprom write ok");

	while(1)
	{	
		Tim2_Handler_Callback();
		switch (cmd_task)
		{
			case CMD_SYS_S:
				Debug_Info(Main_TAG, Data_Buf); 
				Debug_Info(Main_TAG, "更新状态");
				Task_Sys_Show();
				cmd_task = 0x00;
				break;
			case CMD_RFID_R:
				Debug_Info(Main_TAG, Data_Buf); 
				Debug_Info(Main_TAG, "rfid读取");
				Task_Rfid_Scan();
				cmd_task = 0x00;
				break;
			case CMD_RFID_W:
				Debug_Info(Main_TAG, Data_Buf); 
				Debug_Info(Main_TAG, "rfid写入");
				Task_Rfid_Write(Data_Buf);
				Debug_Info(Main_TAG, "rfid完成");
				cmd_task = 0x00;
				break;
			case CMD_GPS_S:
				Debug_Info(Main_TAG, "开启gps");
				Task_Gps_S();
				cmd_task = 0x00;
				break;
			case CMD_GPS_E:
				Debug_Info(Main_TAG, "关闭gps");
				Task_Gps_E();
				cmd_task = 0x00;
				break;
			case CMD_RFID_E:
				Debug_Info(Main_TAG, "结束Rfid");
				Task_Gps_E();
				cmd_task = 0x00;
				break;
			default:
				break;
		}
	}

	return 0;
}
