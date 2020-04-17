#include "callback.h"

extern uint8_t tim2_flag;
static Tim2Callback _tim2Callback; 

void Tim2_Callback_Init(void)
{
    _tim2Callback = Gps_Data_Get;
}

void Tim2_1s_Callback(void)
{
    // Debug_Info(Callback_TAG, "tim2 1s callback");
    // _tim2Callback();
    // tim2_flag = 0;
    if(tim2_flag == 1)
    {
        Debug_Info(Callback_TAG, "tim2 1s callback");
        _tim2Callback();
        tim2_flag = 0;
    }
}