#include "bsp_beep.h"

void Beep_Init(void)
{
	GPIO_InitTypeDef beep_GPIO;
	RCC_APB2PeriphClockCmd(BEEP_CLK, ENABLE);
	beep_GPIO.GPIO_Pin = BEEP_PIN;
	beep_GPIO.GPIO_Mode = BEEP_PIN_MODE;
	beep_GPIO.GPIO_Speed = BEEP_PIN_SPEED;
	GPIO_Init(BEEP_GPIO, &beep_GPIO);
	Debug_Info(Bsp_Beep_TAG, "bsp beep gpio running...");
}

void Beep_Glint(int cnt)
{
	Beep_Glint_ms(cnt, 500);
}

void Beep_Glint_ms(int cnt, uint32_t ms)
{
	int i;
    Beep_Off();
	for(i=0;i<cnt;i++){
		Beep_On();
		delay_ms(ms);
		Beep_Off();
		delay_ms(ms);
	}
    Beep_Off();
}

