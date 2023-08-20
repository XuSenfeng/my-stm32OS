#ifndef __BSP_BASE_TIM_H__
#define __BSP_BASE_TIM_H__


#include "stm32f10x.h"


#define            BASIC_TIM                   TIM6
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM6
#define            BASIC_TIM_Period            1000-1
#define            BASIC_TIM_Prescaler         720-1
#define            BASIC_TIM_IRQ               TIM6_IRQn
#define            BASIC_TIM_IRQHandler        TIM6_IRQHandler



void BASIC_TIM_Init(void);

#endif
