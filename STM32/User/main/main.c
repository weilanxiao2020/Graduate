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
#include <stdlib.h>

void Rifd(void)
{
	char *pId;
	RfidType *data;
	Model *model; 
	Debug_Info("main", "开始读数据");
	// data = Rfid_Read('A');
	data = Rfid_Read_All('A');
	printf("read trackId 0x%x\n",data->pData->trackId);
	printf("read timestamp 0x%x\n",data->pData->timestamp);
	printf("read sender 0x%x\n",data->pData->sender);
	printf("read receiver 0x%x\n",data->pData->receiver);
	printf("read address 0x%x\n",data->pData->address);
	// Oled_Display_String(30,0,"OLED TEST");
	// printf("main model read sender : 0x%x\r\n",data->pData->sender);
	Debug_Info("main", "开始写数据");
	model = (Model *)calloc(1, sizeof(Model));
	model->trackId = 0xef2333;
	model->timestamp = 0xff0000ff00001122;
	model->sender = 0x1234;
	model->receiver = 0x5678;
	model->address = 0x9abcef;
	data->pData = model;
	data->cardType = 'A';
	Rfid_Write(data);
	// printf("main model write sender : 0x%x\n",data->pData->sender);
	// Rfid_Write_Sender(data);
	Debug_Info("main", "Rfid操作完毕");
	free(model);
	free(data->pData);
	free(data);
}

void Gps(void)
{
	// while(1)
	// {
	// 	Gps_Data_Parse();
	// 	Gps_Data_Print();
	// }
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

int main(void)
{
	/*-------------------模块初始化-------------------*/
	SystemInit();
	SysTick_init();
	Debug_Init();
	// Oled_Init();
	OLED_Init();
    Led_Init();
	Rfid_Init();
	Gprs_Init();
	Gps_Init();
	Tim2_1ms_Init();
    Tim2_NVIC();
	// Tim2_Callback_Init();
	// Tim2_Start();

	/*-------------------打印系统信息-------------------*/
	Debug_Error("main", "GraduationProject");
	Debug_Error("main", "魏蓝骁");
	Debug_Error("main", "QQ:1187438570");

	/*-------------------模块功能准备-------------------*/
	//Oled_Display_On();
	// printf("gprs server result:%d\n", Gprs_Send_Data_to_Server());
	/*-------------------模块功能开始-------------------*/
	
	Gps();
	Oled_Test();
	while(1)
	{	
		// Tim2_1s_Callback();
		// Led_Glint(1);
	}

	return 0;
}
