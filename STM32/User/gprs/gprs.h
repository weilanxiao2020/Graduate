#ifndef __GPRS_H
#define __GPRS_H

#include "bsp_gprs.h"
#include "ga6_conf.h"
#include "utils.h"
#include "common.h"
#include "string.h"
#include "constants.h"
#include "debug.h"

#ifdef GZ_DEBUG
    #define Gprs_TAG       "gprs"
#else 
    #define Gprs_TAG       ""  
#endif

#define CREG    "+CREG: 1,1"

void Gprs_Init(void);
boolean Gprs_Ok(void);
boolean Gprs_Set_CREG(uint8_t query_cnt);
boolean Gprs_CREG(uint8_t query_cnt);
boolean Gprs_Restart();
boolean Gprs_Wait_Net();
boolean Gprs_Write_AT_Cmd(const char *str, const char *ack, uint8_t wait_cnt, uint32_t interval_time);
boolean Gprs_Write_AT_Cmd_End(const char *str, const char *ack, uint8_t wait_cnt, uint32_t interval_time);
int Gprs_Connect_Server(const char *server_IP, int port);
int Gprs_Close();
int Gprs_Send_Data_to_Server(const char *server_IP_and_port, const char *content);
#endif
