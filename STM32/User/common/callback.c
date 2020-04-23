#include "callback.h"

extern uint8_t tim2_flag;
static Tim2Callback _tim2Callback[TIM2_1s_CALLBACK_LEN]; 

static uint8_t cnt;

void Tim2_Callback_Init(void)
{
    cnt = 0;
}

int8_t Tim2_Callback_Add(Tim2Callback callback)
{
    if(cnt < TIM2_1s_CALLBACK_LEN) {
        if(Tim2_Callback_Find(callback)==-1) {
            _tim2Callback[cnt++] = callback;
            Debug_Info(Callback_TAG, "callback添加成功");
            return cnt-1;
        }
        return -1;
    } else {
        return -1;
    }
}

int8_t Tim2_Callback_Find(Tim2Callback callback)
{
    int8_t i;
    for(i=0;i<cnt;i++) {
        if(_tim2Callback[i]==callback)
        {
            Debug_Info(Callback_TAG, "callback已存在");
            return i;
        }
    }
    return -1;
}

uint8_t Tim2_Callback_Cnt()
{
    return cnt;
}

int8_t Tim2_Callback_Remove(Tim2Callback callback)
{
    int8_t index = -1;
    if(cnt > 0) {
        index = Tim2_Callback_Find(callback);
        if(index != -1) {
            _tim2Callback[index] = 0;
            Debug_Info(Callback_TAG, "callback移除成功");
            cnt--;
            return cnt;
        }
        return -1;
    } else {
        return -1;
    }
    return -1;
}

void Tim2_1s_Callback(void)
{
    if(tim2_flag == 1)
    {
        // Debug_Info(Callback_TAG, "tim2 1s callback");
        Tim2_Callback();
        tim2_flag = 0;
    }
}

void Tim2_Callback()
{
    uint8_t i=0;
    for(i=0;i<cnt;i++) {
        _tim2Callback[i]();
    }
}