#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "debug.h"

#ifdef GZ_DEBUG
    #define Bsp_Key_TAG    "bsp_key"
#else
    #define Bsp_Key_TAG    ""
#endif

typedef struct {
    volatile uint8_t key_flag;
}KeyType;

typedef enum {
    KEY1=0x01, KEY2=0x02,KEY3=0x04,KEY4=0x08
};

void Key_Init(void);
void Key_Scan(void);

#endif
