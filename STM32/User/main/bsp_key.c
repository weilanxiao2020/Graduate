#include "bsp_key.h"

void Key_Init(void)
{
    GPIO_InitTypeDef gpio_Init;
    EXTI_InitTypeDef exti_Init;
    NVIC_InitTypeDef nvic_Init;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE );

    gpio_Init.GPIO_Pin = GPIO_Pin_0;
    gpio_Init.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &gpio_Init);

    gpio_Init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    gpio_Init.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &gpio_Init);

    // RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE );
    // GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,  GPIO_PinSource8 | GPIO_PinSource9);
}

KeyType _keyType;
extern volatile Task _task;
extern volatile uint8_t cmd_task;

void Key_Scan(void)
{
    uint8_t flag;
	flag = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
    if(flag==0) {
        if((_keyType.key_flag&KEY1)==KEY1) {
            Debug_Info(Bsp_Key_TAG, "按键1未释放");
        } else {
            _keyType.key_flag |= KEY1;
            Debug_Info(Bsp_Key_TAG, "按键1按下");
            Debug_Info(Bsp_Key_TAG, "显示状态");
            _task.sys_s = On;
            cmd_task = CMD_SYS_S;
        }
    } else {
        _keyType.key_flag &= 0xfe;
    }
	
    flag = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
    if(flag==0) {
        if((_keyType.key_flag&KEY2)==KEY2) {
            Debug_Info(Bsp_Key_TAG, "按键2未释放");
        } else {
            _keyType.key_flag |= KEY2;
            Debug_Info(Bsp_Key_TAG, "按键2按下");
            if(_task.gps_s == Off) {
                Debug_Info(Bsp_Key_TAG, "gps开启");
                _task.gps_s = On;
                cmd_task = CMD_GPS_S;
            } else {
                Debug_Info(Bsp_Key_TAG, "gps关闭");
                _task.gps_s = Off;
                cmd_task = CMD_GPS_E;
            }
        }
    } else {
        _keyType.key_flag &=0xfd;
    }

    flag = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
    if(flag==0) {
        if((_keyType.key_flag&KEY3)==KEY3) {
            Debug_Info(Bsp_Key_TAG, "按键3未释放");
        } else {
            _keyType.key_flag |= KEY3;
            Debug_Info(Bsp_Key_TAG, "按键3按下");
            if(_task.rfid_s == Off) {
                Debug_Info(Bsp_Key_TAG, "rfid开启");
                _task.rfid_s = On;
                cmd_task = CMD_RFID_R;
            } else {
                 Debug_Info(Bsp_Key_TAG, "rfid已开启");
            }
        }
    } else {
        _keyType.key_flag &=0xfb;
    }

    flag = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2);
    if(flag==0) {
        if((_keyType.key_flag&KEY4)==KEY4) {
            Debug_Info(Bsp_Key_TAG, "按键4未释放");
        } else {
            _keyType.key_flag |= KEY4;
            Debug_Info(Bsp_Key_TAG, "按键4按下");
            if(_task.rfid_s == On) {
                Debug_Info(Bsp_Key_TAG, "rfid关闭");
                _task.rfid_s = Off;
                cmd_task = CMD_RFID_E;
            } else {
                Debug_Info(Bsp_Key_TAG, "rfid未开启");
            }
        }
    } else {
        _keyType.key_flag &= 0xf7;
    }
}