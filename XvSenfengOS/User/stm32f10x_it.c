/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_usart.h"
#include "stm32f10x.h"
#include "bsp_base_tim.h"
#include "./lcd/bsp_xpt2046_lcd.h"
#include "jiao_os.h"
extern struct Event_Flog EventFlog;

uint32_t time=0;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

void SysTick_Handler(void)
{
}
#if IT_Config
void DEBUG_USART_IRQHandler(void)
{
	uint8_t ucTemp;
	if(USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) != RESET)
	{
		ucTemp = USART_ReceiveData(DEBUG_USARTx);
		USART_SendData(DEBUG_USARTx, ucTemp);
	}
}
#endif
/**
  * @brief  中断函数,触摸屏按下的时候触发
  * @param  无
  * @retval None
  */
void EXTI4_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{

		//使能时钟用来进行消抖
		TIM_Cmd(BASIC_TIM, ENABLE);
		//关闭自己的中断
		EXTI_InitTypeDef EXTI_InitStruct;
		EXTI_InitStruct.EXTI_Line = EXTI_Line4;	//设置为EXTI0
		EXTI_InitStruct.EXTI_LineCmd = DISABLE;	//关闭中断
		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	//设置为中断
		EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//上升沿触发
		EXTI_Init(&EXTI_InitStruct);

		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}
/**
  * @brief  中断函数,Key1按下的时候触发
  * @param  无
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		if(KEY1_Read()==KEY_ON)
			EventFlog.Key1_num=1;
		if(KEY1_Read()==KEY_OFF)
			EventFlog.Key1_num=2;

		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
/**
  * @brief  中断函数,Key2按下的时候触发
  * @param  无
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		if(KEY2_Read()==KEY_ON)
			EventFlog.Key2_num=1;
		if(KEY2_Read()==KEY_OFF)
			EventFlog.Key2_num=2;
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
/**
  * @brief  时钟中断函数,触摸屏按下的时候触发
  * @param  无
  * @retval None
  */
void  BASIC_TIM_IRQHandler (void)
{
	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
	{	
		//判断现在的时钟触摸屏状态
		if(XPT2046_PENIRQ_Read() == Bit_RESET)
		{
			EventFlog.Touch_num=1;
		}else
		{
			TIM_Cmd(BASIC_TIM, DISABLE);
			EXTI_InitTypeDef EXTI_InitStruct;
			EXTI_InitStruct.EXTI_Line = EXTI_Line4;	//设置为EXTI0
			EXTI_InitStruct.EXTI_LineCmd = ENABLE;	//关闭中断
			EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	//设置为中断
			EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//上升沿触发
			EXTI_Init(&EXTI_InitStruct);
		}

		TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update);  		 
	}		 	
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
