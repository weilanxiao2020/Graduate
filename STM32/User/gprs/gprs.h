#ifndef __GPRS_H
#define __GPRS_H

#include "bsp_gprs.h"
#include "ga6_conf.h"
#include "utils.h"

#ifdef GZ_DEBUG
    #define Gprs_TAG       "gprs"
#else 
    #define Gprs_TAG       ""  
#endif

#define CREG    "+CREG:115"

void Gprs_Init(void);
boolean Gprs_Wait_CREG(uint8_t query_times);
int Gprs_Send_Data_to_Server(const char *server_IP_and_port, const char *content);
#endif
