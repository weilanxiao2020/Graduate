#include "task.h"
#include "string.h"

char task_line0[16];
char task_line1[16];
char task_line2[16];
char task_line3[16];

// // mqtt_s:0x00,0x01,0x02,0x03,0x04
// volatile uint8_t mqtt_s;
// // rfid_s:0x00,0x01,0x02,0x03
// volatile uint8_t rfid_s;
// volatile uint8_t gps_s;

volatile Task _task;

void Task_Status(void)
{
    Debug_Info(Task_TAG, "status");
    sprintf(task_line0, "E:%s   G:%s   R:%s", Task_Mqtt_Status(),Task_Gps_Status(), Task_Rfid_Status());
    Oled_Clear();
    Oled_Show_String(0,OLED_LINE0,task_line0);
    if(_task.sys_s!=On) {
        Oled_Show_String(0,2,"none");
        return;
    }
    if(_task.mqtt_s==On) {
        Oled_Show_String(0,2,"wait...");
        return;
    }
    if(_task.rfid_s==0x00) {
        Oled_Show_String(0,2,"running...");
        if(_task.mqtt_s==0xf0) Oled_Show_String(0,2,"restart...");
        if(_task.mqtt_s==0xe1) Oled_Show_String(0,2,"sim...");
        if(_task.mqtt_s==0xd1) Oled_Show_String(0,2,"net...");
        if(_task.mqtt_s==0xc1) Oled_Show_String(0,2,"connect...");
    }
}

char *Task_Mqtt_Status(void)
{
    switch (_task.mqtt_s)
    {
        case 0x00: return "N";
        case 0x01: return "Y";
        case 0x02: return "S";
        case 0x03: return "T";
        case 0x04: return "C";
        case 0x0f: return "E";
        default: return " ";
    }
}

char *Task_Gps_Status(void)
{
    switch (_task.gps_s)
    {
        case 0x00: return "N";
        case 0x01: return "Y";
        case 0x02: return "E";
        default: return " ";
    }
}

char *Task_Rfid_Status(void)
{
    switch (_task.rfid_s)
    {
        case 0x00: return "N";
        case 0x01: return "Y";
        default: return " ";
    }
}

const char* mission = "123456789-416421";

static char* RFID_MQTT_STR = "{\"code\":%d,\"mission\":\"%s\",\"trackId\":\"%s\",\"status\":%d}";
static char* GPS_MQTT_STR = "{\"code\":%d,\"mission\":\"%s\",\"latitude\":\"%s\",\"longitude\":\"%s\"}";

// #define Test

#ifdef Test
void Task_Rfid_Scan_Test(void)
{
    // sprintf(task_line2, "%s", "475126789-416421");
	// Oled_Show_String(0,OLED_LINE2,task_line2);
    // sprintf(task_line3, "%s", "mission error");
    // Oled_Show_String(0,OLED_LINE3,task_line3);
    // Debug_Info(Task_TAG, "missionId不符合，不能装车");
    // Debug_Info(Task_TAG, "missionId:%s,%s[x]","123456789-416421","475126789-416421");

    sprintf(task_line2, "%s", "945613574452475");
	Oled_Show_String(0,OLED_LINE2,task_line2);
    Debug_Info(Task_TAG, "该订单已配送");
    Debug_Info(Task_TAG, "trackId:%s","945613574452475");
    sprintf(task_line3, "%s", "status error");
    Oled_Show_String(0,OLED_LINE3,task_line3);

    char *buf = (char*)calloc(256,sizeof(char));
    sprintf(task_line2, "%s", "945613574452475");
	Oled_Show_String(0,OLED_LINE2,task_line2);
    sprintf(task_line3, "%s", "success");
    Oled_Show_String(0,OLED_LINE3,task_line3);
    sprintf(buf, RFID_MQTT_STR, 0xf1, mission, "945613574452475", 0x01);
    Debug_Info(Task_TAG, "%s=>%s已装车，目的地:%s，状态:0x%02x", 
                mission, "945613574452475", 
                "110106", 0x01);
    Debug_Info(Task_TAG, "mqtt data:");
    Mqtt_Publish(Topic_Card_Scan_Post, buf, strlen(buf),0);
}
#endif

uint8_t cardCnt =0;

void Task_Sys_Show(void)
{
    Task_Status();
}

char card_mission[17] = "123456789-416421";
char mission_temp[17];
int car_Id;

char mqtt_buf[256];
char mqtt_data[128];
const char* GET_MISSION_MQTT_STR = "{\"carId\":%d,\"mission\":\"%[^\"]\"}";
void Task_Get_Mission(void)
{
    int i, len;

    Debug_Info(Task_TAG, "get sub");
    if(Find_Str(Gprs_Get_Buffer(), "+STATUS: MQTT CONNECT OK\r\n\r\nOK\r\n\r\ntopic"))  {          //查找需要应答的字符串ack
        memcpy(mqtt_buf, Gprs_Get_Buffer(),256);
        Gprs_Clear_Buffer();
        Debug_Info(Task_TAG, "sub:%s",mqtt_buf);
        i = sscanf(mqtt_buf, "\r\n+STATUS: MQTT CONNECT OK\r\n\r\nOK\r\n\r\ntopic: /a1Tdu6Ry02R/graduate_node_1/user/card_mission_get,%d\r\n%s", &len,mqtt_data);
    } else if(Find_Str(Gprs_Get_Buffer(), "topic:")) {
        memcpy(mqtt_buf, Gprs_Get_Buffer(),256);
        Gprs_Clear_Buffer();
        Debug_Info(Task_TAG, "sub:%s",mqtt_buf);
        i = sscanf(mqtt_buf, "\r\ntopic: /a1Tdu6Ry02R/graduate_node_1/user/card_mission_get,%d\r\n%s", &len,mqtt_data);
    } else {
        return;
    }
    Debug_Info(Task_TAG, "len:%d, data:%s",i, mqtt_data);
    sscanf(mqtt_data,GET_MISSION_MQTT_STR, &car_Id, mission_temp);
    Debug_Info(Task_TAG, "carId:%d, mission:%s", car_Id, mission_temp);
    if(car_Id!=123) {
        Debug_Info(Task_TAG, "carId error,mission:%s",card_mission);
    } else {
        sprintf(card_mission,"%s",mission_temp);
    }
    Buf_Clear(mqtt_buf,256);
    Buf_Clear(mqtt_data,128);
    _task.mqtt_s = Off;
    _task.sys_s = Off;
    Tim2_Callback_Remove(Task_Get_Mission);
}

void Task_Rfid_Scan(void)
{
    RfidType *data = (RfidType*)calloc(1, sizeof(RfidType));
	char cmp[16];
    _task.rfid_s = 0x01;
    while (1)
    {
        Task_Status();
        // 显示mission
        sprintf(task_line1, "%s", card_mission);
        Oled_Show_String(0,OLED_LINE1,task_line1);
        Oled_Show_String(0,OLED_LINE2,"...");
        #ifdef Test
        Task_Rfid_Scan_Test();
        #else
        // data = Rfid_Read_TrackId_Mission_Address_Status(data, 'A', false);
        if(Rfid_Read_TrackId_Mission_Address_Status(data, 'A', false)==false) {
            Debug_Info(Task_TAG, "rfid读取错误");
            Task_Status();
            return;
        }
        // 显示trackId
        sprintf(task_line2, "%s", data->data.trackId);
        Oled_Show_String(0,OLED_LINE2,task_line2);
        sscanf(card_mission, "%[^-]-%[^-]", cmp, cmp+9);
        Debug_Info(Task_TAG, "mission:%s",cmp);
        if(strcmp(data->data.missionId, cmp)!=0) {
            sprintf(task_line3, "%s", "error");
            Oled_Show_String(0,OLED_LINE3,task_line3);
            Debug_Info(Task_TAG, "missionId不符合，不能装车");
            Debug_Info(Task_TAG, "missionId:%s",data->data.missionId);
            sprintf(task_line3, "%s", "mission error");
            Oled_Show_String(0,OLED_LINE3,task_line3);
        } else if(0x00!=data->data.status){
            Debug_Info(Task_TAG, "该订单已配送");
            Debug_Info(Task_TAG, "trackId:%s",data->data.trackId);
            sprintf(task_line3, "%s", "status error");
            Oled_Show_String(0,OLED_LINE3,task_line3);
        } else {
            char *buf = (char*)calloc(256,sizeof(char));
            sprintf(task_line3, "%s", "success,exit");
            Oled_Show_String(0,OLED_LINE3,task_line3);
            data->data.status = 0x01;
            sprintf(buf, RFID_MQTT_STR, 0xf1, mission, data->data.trackId, data->data.status);
            Debug_Info(Task_TAG, "%s=>%s已装车，目的地:%s，状态:0x%02x", 
                        mission, data->data.trackId, 
                        data->data.address, data->data.status);
            Debug_Info(Task_TAG, "8");
            if(Rfid_Write_TrackId_Mission_Address_Status(data, true)) {
                Debug_Info(Task_TAG, "Rfid操作完毕");   
            } else {
                Debug_Info(Task_TAG, "rfid写入错误");   
            }
            Debug_Info(Task_TAG, "mqtt data:%s",buf);
            Mqtt_Publish(Topic_Card_Scan_Post, buf, strlen(buf),1);
            free(buf);
        }
        delay_ms(2000);
        cardCnt++;
        Debug_Info(Task_TAG, "scan card cnt:%d", cardCnt);
        #endif
    }
    free(data);
    
}

void Task_Rfid_Write(const char *data)
{
	RfidType *rfid = (RfidType *)calloc(1, sizeof(RfidType));
	Model model;
	uint8_t len;
    char temp[17];
    _task.rfid_s = 0x01;
    Task_Status();
	Debug_Info(Task_TAG, "开始写数据");
	len = sscanf(data, "%[^,],%[^,],%[^,],0x%02x", temp, &(model.trackId), &(model.address), &(model.status));
	if(len!=4) {
		Debug_Info(Task_TAG, "输入数据格式错误");
	} else {
		if(strlen(model.trackId)>Model_TrackId_Len ||
			strlen(model.address)>Model_Address_Len) {
			Debug_Info(Task_TAG, "输入数据错误");
		} else {
			Debug_Info(Task_TAG, "trackId:%s",model.trackId);
			Debug_Info(Task_TAG, "address:%s",model.address);
			Debug_Info(Task_TAG, "status:0x%02x",model.status);
            sscanf(temp, "%[^-]-%[^-]", model.missionId, (model.missionId)+9);
            // strcpy(model->missionId, "461237889541354");
            Debug_Info(Task_TAG, "mission:%s",model.missionId);
			rfid->data = model;
			rfid->cardType = 'A';
			if(Rfid_Write_TrackId_Mission_Address_Status(rfid, true)) {
			    Debug_Info(Task_TAG, "Rfid操作完毕");   
            } else {
                Debug_Info(Task_TAG, "rfid写入错误");   
            }

		}
	}
	free(rfid);
}

void Task_Mqtt_Pub(void)
{
    _task.rfid_s = 0x00;
    Task_Status();
    Debug_Info(Task_TAG, "Mqtt Pub");
}

// 获取Mqtt
void Task_Mqtt_Get(void)
{
    _task.rfid_s = 0x00;
    Task_Status();
    Tim2_Callback_Add(Task_Get_Mission);
    Debug_Info(Task_TAG, "Mqtt Get");
}

volatile uint16_t _gps_cnt;

void Task_Gps_S(void)
{
    _task.rfid_s = 0x00;
    _task.gps_s = 0x01;
    Task_Status();
    Tim2_Callback_Add(Gps_Data_Get);
    Tim2_Callback_Add(Task_Mqtt_Gps);
    Debug_Info(Task_TAG, "Gps开启");
}

void Task_Mqtt_Gps()
{
    GpsData* data = Gps_Data_Buf();
    char buf[256];
    char lat[13], lon[14];
    if(data->isUsefull) {
        sprintf(lat, "%s-%s",data->latitude, data->N_S);
        sprintf(lon, "%s-%s",data->longitude, data->E_W);
        sprintf(buf, GPS_MQTT_STR, 0xf1, mission, lat, lon);
        Debug_Info(Task_TAG, "mqtt data:%s",buf);
        // Mqtt_Publish(Topic_Card_Gps_Post, buf, strlen(buf),0);
    } else {
        sprintf(lat, "%s-%s","none", "x");
        sprintf(lon, "%s-%s","none", "x");
        sprintf(buf, GPS_MQTT_STR, 0xf1, mission, lat, lon);
        Debug_Info(Task_TAG, "mqtt data:%s",buf);
    }
    sprintf(task_line1, "%s", "gps data:");
    Oled_Show_String(0,OLED_LINE1,task_line1);
    sprintf(task_line2, "%s:%s", "lat", lat);
    sprintf(task_line3, "%s:%s", "lng", lon);
    Oled_Show_String(0,OLED_LINE2, task_line2);
    Oled_Show_String(0,OLED_LINE3, task_line3);
    // Mqtt_Publish(Topic_Card_Gps_Post, buf, strlen(buf),0);

}

void Task_Gps_E(void)
{
    _task.rfid_s = 0x00;
    _task.gps_s = 0x00;
    Task_Status();
    Debug_Info(Task_TAG, "...");
    Tim2_Callback_Remove(Task_Mqtt_Gps);
    Tim2_Callback_Remove(Gps_Data_Get);
    Debug_Info(Task_TAG, "Gps结束");
}

void Task_Btn_Init(void)
{
    GPIO_InitTypeDef gpio_Init;
    EXTI_InitTypeDef exti_Init;
    NVIC_InitTypeDef nvic_Init;

    gpio_Init.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    gpio_Init.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &gpio_Init);

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE );
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,  GPIO_PinSource8 | GPIO_PinSource9);
    exti_Init.EXTI_Line =  EXTI_Line8 | EXTI_Line9;
    /* EXTI 为中断模式 */
    exti_Init.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 下降沿中断 */
    exti_Init.EXTI_Trigger = EXTI_Trigger_Falling;
    /* 使能中断 */
    exti_Init.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti_Init);

    /* 配置 NVIC 为优先级组 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    /* 配置中断源：按键 */
    nvic_Init.NVIC_IRQChannel = EXTI9_5_IRQn;
    /* 配置抢占优先级 */
    nvic_Init.NVIC_IRQChannelPreemptionPriority = 2;
    /* 配置子优先级 */
    nvic_Init.NVIC_IRQChannelSubPriority = 3;
    /* 使能中断通道 */
    nvic_Init.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&nvic_Init);

}

extern volatile uint8_t cmd_task;
extern volatile uint8_t tim2_1s_flag;
extern volatile uint16_t _1ms_cnt;

volatile uint64_t btn9_ms_cnt = 0;
volatile uint16_t btn9_s_cnt = 0;
volatile uint8_t btn9_flag = 0;
#define Btn_9_Delay     100

void EXTI9_5_IRQHandler()
{
    uint8_t flag;
    if (EXTI_GetITStatus(EXTI_Line5) != RESET) 
    { 
        EXTI_ClearITPendingBit(EXTI_Line5);
        Debug_Info(Task_TAG, "中断5");
    }

    if (EXTI_GetITStatus(EXTI_Line8) != RESET) 
    { 
        EXTI_ClearITPendingBit(EXTI_Line8);
        Debug_Info(Task_TAG, "中断8");
    }

    if (EXTI_GetITStatus(EXTI_Line9) != RESET) 
    { 
        Debug_Info(Task_TAG, "中断9");
        // if(btn9_flag==0) {
        //     btn9_flag = 1;
        //     btn9_ms_cnt = Tim2_Time_Stamp();
        //     Debug_Info(Task_TAG, "按下按键:%ld", btn9_ms_cnt);
        // } else {
        //     flag = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9);
        //     // Debug_Info(Task_TAG, "电平:%d",flag);
        //     if(flag==1) {
        //         btn9_flag = 0;
        //         Debug_Info(Task_TAG, "松开按键:%ld", Tim2_Time_Stamp());
        //         if(Tim2_Time_Stamp() - btn9_ms_cnt > Btn_9_Delay) {
        //             Debug_Info(Task_TAG, "按键触发");
        //         } else {
        //             Debug_Info(Task_TAG, "按键误触");
        //         }
        //     }
        // }
        // if(_task.rfid_s == 0x00) {
        //     cmd_task = CMD_RFID_R;
        //     _task.rfid_s = 0x01;
        //     Debug_Info(Task_TAG, "开启RFID扫描");
        // } else {
        //     Debug_Info(Task_TAG, "Rfid已运行，请等待结束");
        // }
        EXTI_ClearITPendingBit(EXTI_Line9);
    }

}