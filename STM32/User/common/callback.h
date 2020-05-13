#ifndef __CALLBACK_H
#define __CALLBACK_H

#include "constants.h"
#include "bsp_key.h"
#include "debug.h"

#ifdef GZ_DEBUG
    #define Callback_TAG       "callback"
#else 
    #define Callback_TAG       ""  
#endif

#define TIM2_1s_CALLBACK_LEN    5

typedef void (*Tim2Callback)(void);
void Tim2_Handler_Callback(void);
int8_t Tim2_Callback_Find(Tim2Callback callback);
int8_t Tim2_Callback_Add(Tim2Callback callback);
int8_t Tim2_Callback_Remove(Tim2Callback callback);
uint8_t Tim2_Callback_Cnt();
void Tim2_Callback();
#endif
