#ifndef __COMMON_H
#define __COMMON_H

#include "stm32f10x.h"
#include "core_cm3.h"
#include "stm32f10x_conf.h"
#include "bsp_tim.h"
#include "constants.h"

void SysTick_init(void);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

// 枚举定义
// 阶段:0x00:未启动或未工作
//      0x01:启动
//      0x02:入车
//      0x04:运送中
//      0x08:出车
#define NONE 0x00
#define START 0x01
#define INTO 0x02
#define RUN 0x04
#define OUT 0x08

#define CMD_MQTT_G      0x01

#define CMD_RFID_R      0x10
#define CMD_RFID_W      0x11
#define CMD_RFID_E      0x12

#define CMD_GPS_S       0x20
#define CMD_GPS_E       0x21

typedef struct {
    // mqtt_s:0x00,0x01
    volatile uint8_t mqtt_s;
    // rfid_s:0x00,0x01
    volatile uint8_t rfid_s;
    volatile uint8_t gps_s;
    // sys_s:0x00可以显示所有状态，0x01显示完成
    volatile uint8_t sys_s;
}Task;

typedef enum {Off=0x00, On=!Off};

#endif
