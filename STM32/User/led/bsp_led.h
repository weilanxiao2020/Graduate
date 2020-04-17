#pragma once
#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f10x.h"
#include "constants.h"
#include "common.h"
#include "debug.h"


#ifdef GZ_DEBUG
    #define Bsp_Led_TAG    "bsp_led"
#else
    #define Bsp_Led_TAG    ""
#endif

// Led IO define
#define LED_CLK_FUN     RCC_APB2PeriphClockCmd      // 总线时钟
#define LED_CLK 		RCC_APB2Periph_GPIOC        // 对应GPIO时钟
#define LED_GPIO		GPIOC                       // GPIO口
#define LED_PIN			GPIO_Pin_13                 // Pin口
#define LED_PIN_MODE	GPIO_Mode_Out_PP            // IO口模式
#define LED_PIN_SPEED	GPIO_Speed_50MHz            // IO口速度

#define Led_On()        GPIO_ResetBits(LED_GPIO, LED_PIN)   // 打开Led
#define Led_Off()       GPIO_SetBits(LED_GPIO, LED_PIN)     // 关闭Led
#define Led_Toggle()    LED_GPIO->ODR ^=LED_PIN             // 翻转Led

// led tips cmd success
#define Led_Success()   Led_Glint(1)                        // Led提示成功
// led tips cmd failure
#define Led_Failure()   Led_Glint_ms(3,300)                 // Led提示失败
#define Led_Default_Interval    500

// Led初始化
void Led_Init(void);
// Led闪烁n次，间隔500ms
void Led_Glint(int cnt);
// Led闪烁n次，间隔ms
void Led_Glint_ms(int cnt, uint32_t ms);

#endif
