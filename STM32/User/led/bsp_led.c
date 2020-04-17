#include "bsp_led.h"

void Led_Init(void)
{
	GPIO_InitTypeDef led_GPIO;
	LED_CLK_FUN(LED_CLK, ENABLE);
	led_GPIO.GPIO_Pin = LED_PIN;
	led_GPIO.GPIO_Mode = LED_PIN_MODE;
	led_GPIO.GPIO_Speed = LED_PIN_SPEED;
	GPIO_Init(LED_GPIO, &led_GPIO);
	Debug_Info(Bsp_Led_TAG, "bsp led gpio running...");
}

void Led_Glint(int cnt)
{
	Led_Glint_ms(cnt, Led_Default_Interval);
}

void Led_Glint_ms(int cnt, uint32_t ms)
{
	int i;
	Led_Off();
	for(i=0;i<cnt;i++){
		Led_On();
		delay_ms(ms);
		Led_Off();
		delay_ms(ms);
	}
	Led_Off();
}
