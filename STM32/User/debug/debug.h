#pragma once
#ifndef __DEBUG_H
#define __DEBUG_H

#include "stm32f10x.h"
#include "constants.h"
#include "usart.h"
#include "bsp_debug.h"
#include "utils.h"
#include <stdio.h>
#include <stdarg.h>

#ifdef GZ_DEBUG
    #define Debug_TAG       "debug"
#else 
    #define Debug_TAG       ""  
#endif

#define Debug_Buf_Length    128
#define Debug_Usart         DEBUG_USART
#define Debug_Rx_Length  128

void Debug_Init(void);

uint8_t parseCMD_Rfid_R();
uint8_t parseCMD_Rfid_W();
uint8_t parseCMD_Gps_S();
uint8_t parseCMD_Gps_E();

void logI(const char *type, const char *func, const char *file, const char *format, ...);
void logE(const char *type, const char *func, const char *file, const char *format, ...);
#define Debug_Info(type, format, ...) logI(type, __func__, __FILE__, format, ##__VA_ARGS__)
#define Debug_Error(type, format, ...) logE(type, __func__, __FILE__, format, ##__VA_ARGS__)
// printf
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
#endif
