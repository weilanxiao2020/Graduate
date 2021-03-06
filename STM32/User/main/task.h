#ifndef __TASK_H
#define __TASK_H

#include "debug.h"
#include "rfid.h"
#include "mqtt.h"
#include "oled.h"
#include "gps.h"
#include "callback.h"
#include "stm32f10x.h"

#ifdef GZ_DEBUG
    #define Task_TAG    "task"
#else
    #define Task_TAG    ""
#endif

void Task_Status(void);
void Task_Sys_Show(void);
void Task_Rfid_Scan(void);
void Task_Rfid_Write(const char *data);
void Task_Mqtt_Pub(void);
void Task_Gps_S(void);
void Task_Gps_E(void);
void Task_Mqtt_Gps(void);

char *Task_Mqtt_Status(void);
char *Task_Rfid_Status(void);
char *Task_Gps_Status(void);
void Task_Get_Mission(void);

uint16_t Mqtt_Rfid_Data_Build(const RfidType *data, uint8_t len, char *buf);

void Task_Btn_Init(void);

#endif
