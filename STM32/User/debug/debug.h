#pragma once
#ifndef __DEBUG_H
#define __DEBUG_H

#include "stm32f10x.h"
#include "constants.h"
#include "usart.h"
#include "bsp_debug.h"
#include "utils.h"
#include <stdio.h>

#ifdef GZ_DEBUG
    #define Debug_TAG       "debug"
#else 
    #define Debug_TAG       ""  
#endif

#define Debug_Buf_Length    128
#define Debug_Usart         DEBUG_USART

void Debug_Init(void);
void Debug_Info(const char *tag, char *msg);
void Debug_Error(const char *tag, char *msg);

// printf
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
#endif
