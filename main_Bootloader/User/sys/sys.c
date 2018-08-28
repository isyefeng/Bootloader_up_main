#include "sys.h"

__asm void MSR_MSP( u32 addr )
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}

void Sys_Soft_Reset( void )
{
	SCB->AIRCR = 0X05FA0000 | ( u32 )0x04;
}


