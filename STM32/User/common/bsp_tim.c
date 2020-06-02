#include "bsp_tim.h"
#include "bsp_led.h"

volatile uint16_t _1ms_cnt;

void Tim2_Start(void)
{
    _1ms_cnt=0;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    // Debug_Info(Bsp_Tim_TAG, "tim2 1ms start");
}


void Tim2_1ms_Init(void)
{
    TIM_TimeBaseInitTypeDef  timInit;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    TIM_DeInit(TIM2);
    timInit.TIM_Period=1000-1;	//自动重装载寄存器周期的值(计数值) 
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    timInit.TIM_Prescaler= (72 - 1);	//时钟预分频数 72M/72      
    timInit.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式 
    TIM_TimeBaseInit(TIM2, &timInit);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);	// 清除溢出中断标志 
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);// 开启中断标志 
    TIM_Cmd(TIM2, ENABLE);	// 开启时钟    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);	//先关闭等待使用 
    // Debug_Info(Bsp_Tim_TAG, "tim2 1ms init"); 
}

void Tim2_NVIC(void)
{
    NVIC_InitTypeDef nvicInit; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    nvicInit.NVIC_IRQChannel = TIM2_IRQn;	  
    nvicInit.NVIC_IRQChannelPreemptionPriority = 0;
    nvicInit.NVIC_IRQChannelSubPriority = 3;	
    nvicInit.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicInit);
    // Debug_Info(Bsp_Tim_TAG, "tim2 1ms nvic");
}

volatile uint8_t tim2_1s_flag;
volatile uint8_t tim2_50ms_flag;
volatile uint64_t _1s_cnt;
volatile uint64_t _time_stamp_ = 0;

uint64_t Tim2_Time_Stamp(void)
{
    return _time_stamp_;
}

uint64_t Tim2_1s_Cnt(void)
{
    return _1s_cnt;
}

void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);   
        // TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志 
  		_1ms_cnt++;
        _time_stamp_++;
        if((_1ms_cnt%50)==0) {
            tim2_50ms_flag = 1;
            Key_Scan();
        }

        if(_1ms_cnt==1000)
        {
            _1ms_cnt = 0;
            _1s_cnt++;
            // Debug_Info(Bsp_Tim_TAG, "TIM2_IRQHandler");
            tim2_1s_flag = 1;
            // Tim2_1s_Callback();
        }
	}		 	
}