/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   测试led
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"

#include "FreeRTOS.h"					//FreeRTOS使用		  
#include "task.h" 
#define SOFT_DELAY Delay(0x0FFFFF);
static void BSP_Init(void);
static void AppTaskCreate(void);
static void LED2_Task(void* parameter);

/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */
/* AppTaskCreate任务任务堆栈 */
//static StackType_t AppTaskCreate_Stack[128];
/* LED任务堆栈 */
//static StackType_t LED_Task_Stack[128];

/* AppTaskCreate 任务控制块 */
//static StaticTask_t AppTaskCreate_TCB;
/* AppTaskCreate 任务控制块 */
//static StaticTask_t LED_Task_TCB;

/* 空闲任务任务堆栈 */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/* 定时器任务堆栈 */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

/* 空闲任务控制块 */
static StaticTask_t Idle_Task_TCB;	
/* 定时器任务控制块 */
static StaticTask_t Timer_Task_TCB;

 /* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle;
/* LED任务句柄 */
static TaskHandle_t LED_Task_Handle;
static TaskHandle_t LED2_Task_Handle;
static TaskHandle_t LED3_Task_Handle;

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	BSP_Init();

	BaseType_t xReturn = pdPASS;
	/* 创建LED_Task任务 */
	xReturn = xTaskCreate(	(TaskFunction_t)AppTaskCreate,
							(const char *)"APPTaskCreat",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)3,
							&AppTaskCreate_Handle );
	if(xReturn == pdPASS)
	{
		printf("App_Task任务创建成功!\n");
		vTaskStartScheduler();   /* 启动任务，开启调度 */
	}else
	{
		printf("LED_Task任务创建失败!\n");
	}
	
	while(1);
}


static void BSP_Init(void)
{
	//优先级设置, 统一设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	LED_GPIO_Config();
	
	USART_Config();
	printf("初始化完成\n");
}


/**************LED任务*****************/
static void LED_Task(void* parameter)
{	
    while (1)
    {
        LED1_ON;
        vTaskDelay(500);   /* 延时500个tick */
        printf("LED_Task Running,LED1_ON\r\n");
        
        LED1_OFF;     
        vTaskDelay(500);   /* 延时500个tick */		 		
        printf("LED_Task Running,LED1_OFF\r\n");
    }
}
static void LED2_Task(void* parameter)
{	
    while (1)
    {
        LED2_ON;
        vTaskDelay(700);   /* 延时500个tick */
        printf("LED_Task Running,LED1_ON\r\n");
        
        LED2_OFF;     
        vTaskDelay(700);   /* 延时500个tick */		 		
        printf("LED_Task Running,LED1_OFF\r\n");
    }
}
static void LED3_Task(void* parameter)
{	
    while (1)
    {
        LED3_ON;
        vTaskDelay(800);   /* 延时500个tick */
        printf("LED_Task Running,LED1_ON\r\n");
        
        LED3_OFF;     
        vTaskDelay(800);   /* 延时500个tick */		 		
        printf("LED_Task Running,LED1_OFF\r\n");
    }
}
static void AppTaskCreate(void)
{
	taskENTER_CRITICAL();           //进入临界区
	BaseType_t xReturn = pdPASS;
	/* 创建LED_Task任务 */
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
	if(xReturn == pdPASS)/* 创建成功 */
		printf("LED_Task任务创建成功!\n");
	else
		printf("LED_Task任务创建失败!\n");
	
	vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
  
	taskEXIT_CRITICAL();            //退出临界区
}

//空闲任务, 在进入任务的时候使用
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
								   StackType_t **ppxIdleTaskStackBuffer, 
								   uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer=&Idle_Task_TCB;/* 任务控制块内存 */
	*ppxIdleTaskStackBuffer=Idle_Task_Stack;/* 任务堆栈内存 */
	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;/* 任务堆栈大小 */
}

/**
  *********************************************************************
  * @brief  获取定时器任务的任务堆栈和任务控制块内存
	*					ppxTimerTaskTCBBuffer	:		任务控制块内存
	*					ppxTimerTaskStackBuffer	:	任务堆栈内存
	*					pulTimerTaskStackSize	:		任务堆栈大小
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  **********************************************************************
  */ 
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
									StackType_t **ppxTimerTaskStackBuffer, 
									uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer=&Timer_Task_TCB;/* 任务控制块内存 */
	*ppxTimerTaskStackBuffer=Timer_Task_Stack;/* 任务堆栈内存 */
	*pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;/* 任务堆栈大小 */
}
