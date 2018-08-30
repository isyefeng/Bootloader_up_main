#ifndef __BUZ_DIRVE_H
#define __BUZ_DIRVE_H

#define BUZ_PIN		GPIO_Pin_8
#define BUZ_RORT  GPIOA
#define BUZ_RCC  	RCC_APB2Periph_GPIOA

#define 	BUZ_ON		GPIO_SetBits(BUZ_RORT,BUZ_PIN)
#define 	BUZ_OFF		GPIO_ResetBits(BUZ_RORT,BUZ_PIN)


void BUZ_config(void);

#endif /*__BUZ_DIRVE_H*/
