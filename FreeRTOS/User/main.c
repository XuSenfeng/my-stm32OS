/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����led
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-ָ���� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"

#include "FreeRTOS.h"					//FreeRTOSʹ��		  
#include "task.h" 
#define SOFT_DELAY Delay(0x0FFFFF);
static void BSP_Init(void);
static void AppTaskCreate(void);
static void LED2_Task(void* parameter);

/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */
/* AppTaskCreate���������ջ */
//static StackType_t AppTaskCreate_Stack[128];
/* LED�����ջ */
//static StackType_t LED_Task_Stack[128];

/* AppTaskCreate ������ƿ� */
//static StaticTask_t AppTaskCreate_TCB;
/* AppTaskCreate ������ƿ� */
//static StaticTask_t LED_Task_TCB;

/* �������������ջ */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/* ��ʱ�������ջ */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

/* ����������ƿ� */
static StaticTask_t Idle_Task_TCB;	
/* ��ʱ��������ƿ� */
static StaticTask_t Timer_Task_TCB;

 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle;
/* LED������ */
static TaskHandle_t LED_Task_Handle;
static TaskHandle_t LED2_Task_Handle;
static TaskHandle_t LED3_Task_Handle;

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
	BSP_Init();

	BaseType_t xReturn = pdPASS;
	/* ����LED_Task���� */
	xReturn = xTaskCreate(	(TaskFunction_t)AppTaskCreate,
							(const char *)"APPTaskCreat",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)3,
							&AppTaskCreate_Handle );
	if(xReturn == pdPASS)
	{
		printf("App_Task���񴴽��ɹ�!\n");
		vTaskStartScheduler();   /* �������񣬿������� */
	}else
	{
		printf("LED_Task���񴴽�ʧ��!\n");
	}
	
	while(1);
}


static void BSP_Init(void)
{
	//���ȼ�����, ͳһ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	LED_GPIO_Config();
	
	USART_Config();
	printf("��ʼ�����\n");
}


/**************LED����*****************/
static void LED_Task(void* parameter)
{	
    while (1)
    {
        LED1_ON;
        vTaskDelay(500);   /* ��ʱ500��tick */
        printf("LED_Task Running,LED1_ON\r\n");
        
        LED1_OFF;     
        vTaskDelay(500);   /* ��ʱ500��tick */		 		
        printf("LED_Task Running,LED1_OFF\r\n");
    }
}
static void LED2_Task(void* parameter)
{	
    while (1)
    {
        LED2_ON;
        vTaskDelay(700);   /* ��ʱ500��tick */
        printf("LED_Task Running,LED1_ON\r\n");
        
        LED2_OFF;     
        vTaskDelay(700);   /* ��ʱ500��tick */		 		
        printf("LED_Task Running,LED1_OFF\r\n");
    }
}
static void LED3_Task(void* parameter)
{	
    while (1)
    {
        LED3_ON;
        vTaskDelay(800);   /* ��ʱ500��tick */
        printf("LED_Task Running,LED1_ON\r\n");
        
        LED3_OFF;     
        vTaskDelay(800);   /* ��ʱ500��tick */		 		
        printf("LED_Task Running,LED1_OFF\r\n");
    }
}
static void AppTaskCreate(void)
{
	taskENTER_CRITICAL();           //�����ٽ���
	BaseType_t xReturn = pdPASS;
	/* ����LED_Task���� */
	xReturn = xTaskCreate(	(TaskFunction_t)LED_Task,
							(const char *)"LEDCreat",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)3,
							&LED_Task_Handle );
	xReturn = xTaskCreate(	(TaskFunction_t)LED2_Task,
							(const char *)"LED2Creat",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)3,
							&LED2_Task_Handle );
	xReturn = xTaskCreate(	(TaskFunction_t)LED3_Task,
						(const char *)"LED2Creat",
						(uint16_t)128,
						(void *)NULL,
						(UBaseType_t)3,
						&LED3_Task_Handle );	
	if(xReturn == pdPASS)/* �����ɹ� */
		printf("LED_Task���񴴽��ɹ�!\n");
	else
		printf("LED_Task���񴴽�ʧ��!\n");
	
	vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
	taskEXIT_CRITICAL();            //�˳��ٽ���
}

//��������, �ڽ��������ʱ��ʹ��
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
								   StackType_t **ppxIdleTaskStackBuffer, 
								   uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer=&Idle_Task_TCB;/* ������ƿ��ڴ� */
	*ppxIdleTaskStackBuffer=Idle_Task_Stack;/* �����ջ�ڴ� */
	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;/* �����ջ��С */
}

/**
  *********************************************************************
  * @brief  ��ȡ��ʱ������������ջ��������ƿ��ڴ�
	*					ppxTimerTaskTCBBuffer	:		������ƿ��ڴ�
	*					ppxTimerTaskStackBuffer	:	�����ջ�ڴ�
	*					pulTimerTaskStackSize	:		�����ջ��С
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  **********************************************************************
  */ 
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
									StackType_t **ppxTimerTaskStackBuffer, 
									uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer=&Timer_Task_TCB;/* ������ƿ��ڴ� */
	*ppxTimerTaskStackBuffer=Timer_Task_Stack;/* �����ջ�ڴ� */
	*pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;/* �����ջ��С */
}
