#pragma once
#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H

#include "stm32f10x.h"
#include "constants.h"
#include "debug.h"

#ifdef GZ_DEBUG
    #define Bsp_Beep_TAG    "bsp_beep"
#else
    #define Bsp_Beep_TAG    ""
#endif

#define BEEP_CLK_FUN        RCC_APB2PeriphClockCmd
#define BEEP_CLK		    RCC_APB2Periph_GPIOC
#define BEEP_GPIO		    GPIOC
#define BEEP_PIN			GPIO_Pin_13
#define BEEP_PIN_MODE	    GPIO_Mode_Out_PP
#define BEEP_PIN_SPEED	    GPIO_Speed_50MHz

#define Beep_On()           GPIO_ResetBits(BEEP_GPIO, BEEP_PIN)      
#define Beep_Off()          GPIO_SetBits(BEEP_GPIO, BEEP_PIN)
#define Beep_Toggle()       BEEP_GPIO->ODR ^=BEEP_PIN

#define Beep_Default_Interval    500
// Beep tips cmd success
#define Beep_Success()      Beep_Glint(1)
// Beep tips cmd failure
#define Beep_Failure()      Beep_Glint_ms(3,300)

void Beep_Init(void);
void Beep_Glint(int cnt);
void Beep_Glint_ms(int cnt, uint32_t ms);

#endif
