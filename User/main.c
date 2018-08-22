#include "includes.h"
#include "LED.h"
#include "key_drive.h"
#include "EXTI_Dirve.h"		
#include "iap.h"

void delay(unsigned int time)
{
	unsigned int i;
	while(time--)
	{for(i=0;i<0xffff;i++);}
}

int main(void)
{
	LED_R_Config();
	LED_R_control(1);
	delay(0xff);
	LED_R_control(0);
	delay(0xff);
	while(1)
	{
		IAP_load( FLASH_APP1_ADDR );
	}
}

