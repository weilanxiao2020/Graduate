#include "bsp_key.h"

void Key_Init(void)
{
    GPIO_InitTypeDef gpio_Init;
    EXTI_InitTypeDef exti_Init;
    NVIC_InitTypeDef nvic_Init;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );

    gpio_Init.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9;
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
    flag = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
    if(flag==0) {
        if((_keyType.key_flag&0x01)==0x01) {
            // Debug_Info(Bsp_Key_TAG, "����5δ�ͷ�");
        } else {
            _keyType.key_flag |=0x01;
            Debug_Info(Bsp_Key_TAG, "����5����");
            Debug_Info(Bsp_Key_TAG, "��ʾ״̬");
            // if(_task.sys_s == Off) {
            //     Debug_Info(Bsp_Key_TAG, "gps����");
            //     _task.gps_s = On;
            // } else {
            //     Debug_Info(Bsp_Key_TAG, "gps�ر�");
            //     _task.gps_s = Off;
            // }
            _task.sys_s = On;
            cmd_task = CMD_SYS_S;
        }
    } else {
        _keyType.key_flag &=0xfe;
    }

    flag = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8);
    if(flag==0) {
        if((_keyType.key_flag&0x02)==0x02) {
            Debug_Info(Bsp_Key_TAG, "����8δ�ͷ�");
        } else {
            _keyType.key_flag |=0x02;
            Debug_Info(Bsp_Key_TAG, "����8����");
            if(_task.gps_s == Off) {
                Debug_Info(Bsp_Key_TAG, "gps����");
                _task.gps_s = On;
                cmd_task = CMD_GPS_S;
            } else {
                Debug_Info(Bsp_Key_TAG, "gps�ر�");
                _task.gps_s = Off;
                cmd_task = CMD_GPS_E;
            }
        }
    } else {
        _keyType.key_flag &=0xfd;
    }

    flag = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9);
    if(flag==0) {
        if((_keyType.key_flag&0x04)==0x04) {
            Debug_Info(Bsp_Key_TAG, "����9δ�ͷ�");
        } else {
            _keyType.key_flag |=0x04;
            Debug_Info(Bsp_Key_TAG, "����9����");
            if(_task.rfid_s == Off) {
                Debug_Info(Bsp_Key_TAG, "rfid����");
                _task.rfid_s = On;
                cmd_task = CMD_RFID_R;
            } else {
                Debug_Info(Bsp_Key_TAG, "rfid�ر�");
                _task.rfid_s = Off;
                cmd_task = CMD_RFID_E;
            }
        }
    } else {
        _keyType.key_flag &= 0xfb;
    }
}