#include "common.h"

__IO uint32_t TimingDelay;

void SysTick_init(void)
{
	SysTick_Config(SystemCoreClock / 1000000);
}

void delay_ms(uint32_t ms)
{
	TimingDelay = ms*1000;
    //使能系统滴答定时器
    while(TimingDelay !=0);
}

void delay_us(uint32_t us)
{ 
	TimingDelay = us;
    //使能系统滴答定时器
    while(TimingDelay !=0);	
}



