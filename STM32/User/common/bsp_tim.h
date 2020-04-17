#ifndef __BSP_TIM_H
#define __BSP_TIM_H

#include "stm32f10x.h"
#include "constants.h"

#ifdef GZ_DEBUG
    #define Bsp_Tim_TAG       "bsp_tim"
#else 
    #define Bsp_Tim_TAG       ""  
#endif

void Tim2_1ms_Init(void);
void Tim2_NVIC(void);
void Tim2_Start(void);
void TIM2_IRQHandler(void);
#endif
