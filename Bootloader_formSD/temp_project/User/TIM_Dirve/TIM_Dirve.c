#include "includes.h"

static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_TIM_Initstruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_TIM_Initstruct.NVIC_IRQChannel = TIMX_IRQn;
	NVIC_TIM_Initstruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_TIM_Initstruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_TIM_Initstruct.NVIC_IRQChannelSubPriority = 3;
	
	NVIC_Init(&NVIC_TIM_Initstruct);
}

//2 uint16_t TIM_Prescaler; // 预分频器
//3 uint16_t TIM_CounterMode; // 计数模式
//4 uint32_t TIM_Period; // 定时器周期
//5 uint16_t TIM_ClockDivision; // 时钟分频
//6 uint8_t TIM_RepetitionCounter; // 重复计算器

void TIMx_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	NVIC_Config();
	RCC_APB1PeriphClockCmd(RCC_TIMx, ENABLE); //使能AHB1_TIM
	
	TIM_TimeBaseInitStruct.TIM_Prescaler	=	RCC_TIM_Prescaler;
	TIM_TimeBaseInitStruct.TIM_Period			=	RCC_TIM_Period;
	
	TIM_TimeBaseInit(TIM_X,&TIM_TimeBaseInitStruct);
	TIM_ClearFlag(TIM_X, TIM_FLAG_Update);
	TIM_ITConfig(TIM_X, TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM_X, ENABLE);
}

void TIM6_IRQHandler (void)
{
	static uint32_t time,LED_flag_add;
	if(TIM_GetFlagStatus(TIM_X, TIM_FLAG_Update)!=RESET)
	{
		time++;
		LED_flag_add++;
		if(LED_flag_add == 100)
		{
			LED_G_TOGGLE;		
			LED_flag_add = 0;
		}
		if(time==10000)
		{

			time = 0;
			Resource_release();
			IAP_load( FLASH_APP1_ADDR );
		}
		TIM_ClearITPendingBit(TIM_X, TIM_FLAG_Update);
	}
}	

void TIM7_IRQHandler (void)
{
	static uint32_t time;
	if(TIM_GetFlagStatus(TIM_X, TIM_FLAG_Update)!=RESET)
	{
		time++;
		TIM_ClearITPendingBit(TIM_X, TIM_FLAG_Update);
	}
}	

