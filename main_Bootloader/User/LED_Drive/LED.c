#include "LED.h"

void LED_G_Config(void)
{
	GPIO_InitTypeDef GPIO_LED;
	
	RCC_APB2PeriphClockCmd(LED_G_RCC, ENABLE);
	
	GPIO_LED.GPIO_Pin = LED_G_PIN;
	GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_Init(LED_G_RORT,&GPIO_LED);	
}
