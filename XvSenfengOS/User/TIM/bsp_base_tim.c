#include "bsp_base_tim.h"
/**
  * @brief  ��ʼ����ʱ��6, �������д������ж�ʵ��
  * @param  ��
  * @retval None
  */
static void BASIC_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_BaseStructure;
	BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
	
	
	TIM_BaseStructure.TIM_Period = BASIC_TIM_Period;
	
	TIM_BaseStructure.TIM_Prescaler = BASIC_TIM_Prescaler;
	/****������ʱ��û��****/
	TIM_BaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseStructure.TIM_RepetitionCounter = 0;
	/********************/
	
	TIM_TimeBaseInit(BASIC_TIM, &TIM_BaseStructure);
	
	TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	
	TIM_ITConfig(BASIC_TIM, TIM_FLAG_Update, ENABLE);	
}
/**
  * @brief  ���ô����������ȼ�
  * @param  ��
  * @retval None
  */
static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  ��ʼ����ʱ��
  * @param  ��
  * @retval None
  */
void BASIC_TIM_Init(void)
{
	BASIC_TIM_Config();
	BASIC_TIM_NVIC_Config();
}
