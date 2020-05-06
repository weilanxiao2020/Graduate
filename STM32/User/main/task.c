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
    // Oled_Show_String(0,2,"1234567890abcde1");
    // Oled_Show_String(0,4,"1234567890abcde2");
    // Oled_Show_String(0,6,"1234567890abcde3");
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

#define Test

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

    // char *buf = (char*)calloc(256,sizeof(char));
    // sprintf(task_line2, "%s", "945613574452475");
	// Oled_Show_String(0,OLED_LINE2,task_line2);
    // sprintf(task_line3, "%s", "success");
    // Oled_Show_String(0,OLED_LINE3,task_line3);
    // sprintf(buf, RFID_MQTT_STR, 0xf1, mission, "945613574452475", 0x01);
    // Debug_Info(Task_TAG, "%s=>%s已装车，目的地:%s，状态:0x%02x", 
    //             mission, "945613574452475", 
    //             "110106", 0x01);
    // Debug_Info(Task_TAG, "mqtt data:");
    // Mqtt_Publish(Topic_Card_Scan_Post, buf, strlen(buf),0);
}
#endif

void Task_Rfid_Scan(void)
{
    RfidType *data = NULL;
	char cmp[16];
    _task.rfid_s = 0x01;
    Task_Status();
    // 显示mission
    sprintf(task_line1, "%s", mission);
    Oled_Show_String(0,OLED_LINE1,task_line1);
    #ifdef Test
        Task_Rfid_Scan_Test();
    #else
    data = Rfid_Read_TrackId_Mission_Address_Status('A', false);
    if(data == NULL) {
        Debug_Info(Task_TAG, "rfid读取错误");
        Task_Status();
        return;
    }
    // 显示trackId
    sprintf(task_line2, "%s", data->pData->trackId);
	Oled_Show_String(0,OLED_LINE2,task_line2);
    sscanf(mission, "%[^-]-%[^-]", cmp, cmp+9);
    if(strcmp(data->pData->missionId, cmp)!=0) {
        sprintf(task_line3, "%s", "error");
	    Oled_Show_String(0,OLED_LINE3,task_line3);
        Debug_Info(Task_TAG, "missionId不符合，不能装车");
        Debug_Info(Task_TAG, "missionId:%s",data->pData->missionId);
        sprintf(task_line3, "%s", "mission error");
	    Oled_Show_String(0,OLED_LINE3,task_line3);
    } else if(0x00!=data->pData->status){
        Debug_Info(Task_TAG, "该订单已配送");
        Debug_Info(Task_TAG, "trackId:%s",data->pData->trackId);
        sprintf(task_line3, "%s", "status error");
	    Oled_Show_String(0,OLED_LINE3,task_line3);
    } else {
        char *buf = (char*)calloc(256,sizeof(char));
        sprintf(task_line3, "%s", "success");
	    Oled_Show_String(0,OLED_LINE3,task_line3);
        data->pData->status = 0x01;
        sprintf(buf, RFID_MQTT_STR, 0xf1, mission, data->pData->trackId, data->pData->status);
        Debug_Info(Task_TAG, "%s=>%s已装车，目的地:%s，状态:0x%02x", 
                    mission, data->pData->trackId, 
                    data->pData->address, data->pData->status);
        if(Rfid_Write_TrackId_Mission_Address_Status(data, true)) {
            Debug_Info(Task_TAG, "Rfid操作完毕");   
        } else {
            Debug_Info(Task_TAG, "rfid写入错误");   
        }
        Debug_Info(Task_TAG, "mqtt data:");
        Mqtt_Publish(Topic_Card_Test, buf, strlen(buf),0);
    }
    free(data);
    #endif
}

uint16_t Mqtt_Rfid_Data_Build(const RfidType *data, uint8_t len, char *buf)
{
    char head1[2] = {(16>>8)&0xff, 16&0xff};
    char head2[2] = {0x00, 1&0xff};
    uint16_t size = 2+4+len;
    buf = (char*)calloc(size, sizeof(char));
    *buf=0xff;*(buf+1)=0xee;
    memcpy(buf+2,head1,2);
    memcpy(buf+4,head2,2);
    memcpy(buf+6,data->pData->trackId,16);
    memcpy(buf+6+16,&(data->pData->status),1);
    return size;
}

void Task_Rfid_Write(const char *data)
{
	RfidType *rfid = (RfidType *)calloc(1, sizeof(RfidType));
	Model *model = (Model *)calloc(1, sizeof(Model));
	uint8_t len;
    _task.rfid_s = 0x01;
    Task_Status();
	Debug_Info(Task_TAG, "开始写数据");
	len = sscanf(data, "%[^,],%[^,],0x%02x", model->trackId, model->address, &(model->status));
	if(len!=3) {
		Debug_Info(Task_TAG, "输入数据格式错误");
	} else {
		if(strlen(model->trackId)>Model_TrackId_Len ||
			strlen(model->address)>Model_Address_Len) {
			Debug_Info(Task_TAG, "输入数据错误");
		} else {
			Debug_Info(Task_TAG, "trackId:%s",model->trackId);
			Debug_Info(Task_TAG, "address:%s",model->address);
			Debug_Info(Task_TAG, "status:0x%02x",model->status);
            sscanf("461237889-541354", "%[^-]-%[^-]", model->missionId, (model->missionId)+9);
            // strcpy(model->missionId, "461237889541354");
            Debug_Info(Task_TAG, "mission:%s",model->missionId);
			rfid->pData = model;
			rfid->cardType = 'A';
			if(Rfid_Write_TrackId_Mission_Address_Status(rfid, true)) {
			    Debug_Info(Task_TAG, "Rfid操作完毕");   
            } else {
                Debug_Info(Task_TAG, "rfid写入错误");   
            }

		}
	}

	free(model);
	free(rfid);
}

void Task_Mqtt_Pub(void)
{
    _task.rfid_s = 0x00;
    Task_Status();
    Debug_Info(Task_TAG, "Mqtt Pub");
}

void Task_Gps_S(void)
{
    _task.rfid_s = 0x00;
    _task.gps_s = 0x01;
    Task_Status();
    Debug_Info(Task_TAG, "Gps开启");
}

void Task_Gps_E(void)
{
    _task.rfid_s = 0x00;
    _task.gps_s = 0x00;
    Task_Status();
    Debug_Info(Task_TAG, "Gps结束");
}