#include "bsp_base_tim.h"
/**
  * @brief  初始化定时器6, 用来进行触摸屏中断实现
  * @param  无
  * @retval None
  */
static void BASIC_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_BaseStructure;
	BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
	
	
	TIM_BaseStructure.TIM_Period = BASIC_TIM_Period;
	
	TIM_BaseStructure.TIM_Prescaler = BASIC_TIM_Prescaler;
	/****基础定时器没有****/
	TIM_BaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseStructure.TIM_RepetitionCounter = 0;
	/********************/
	
	TIM_TimeBaseInit(BASIC_TIM, &TIM_BaseStructure);
	
	TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	
	TIM_ITConfig(BASIC_TIM, TIM_FLAG_Update, ENABLE);	
}
/**
  * @brief  设置触摸屏的优先级
  * @param  无
  * @retval None
  */
static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  初始化定时器
  * @param  无
  * @retval None
  */
void BASIC_TIM_Init(void)
{
	BASIC_TIM_Config();
	BASIC_TIM_NVIC_Config();
}
