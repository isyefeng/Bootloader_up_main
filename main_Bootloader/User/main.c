#include "includes.h"
#include "LED.h"
#include "key_drive.h"
#include "EXTI_Dirve.h"		
#include "iap.h"

#define 		BlockSize				2048

void delay(unsigned int time)
{
	unsigned int i;
	while(time--)
	{for(i=0;i<0xffff;i++);}
}

int main(void)
{
	SCB->VTOR = FLASH_BASE+BlockSize*15;
	LED_G_Config();
	LED_G_control(1);
	delay(0xff);
	LED_G_control(0);
	delay(0xff);
	while(1)
	{
	}
}

