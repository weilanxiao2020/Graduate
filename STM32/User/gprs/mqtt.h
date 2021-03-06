#ifndef __MQTT_H
#define __MQTT_H

#include "gprs.h"
#include "oled.h"
#include "bsp_tim.h"

#ifdef GZ_DEBUG
    #define Mqtt_TAG       "mqtt"
#else 
    #define Mqtt_TAG       ""  
#endif

#define Topic_Card_Test "/a1Tdu6Ry02R/graduate_node_1/user/card_info_post_test"
#define Topic_Card_Scan_Post "/a1Tdu6Ry02R/graduate_node_1/user/card_scan_rfid_post"
#define Topic_Card_Gps_Post "/a1Tdu6Ry02R/graduate_node_1/user/card_gps_post"

void Mqtt_Init();
boolean Mqtt_Status();
void Mqtt_Publish(const char *topic, const char *data, uint16_t len, uint8_t qos);

#endif
