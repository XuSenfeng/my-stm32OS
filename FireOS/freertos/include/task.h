#ifndef __TASK_H
#define __TASK_H
#include "list.h"

typedef void * TaskHandle_t;


typedef struct tskTaskControlBlock
{
	//栈顶
	volatile StackType_t	*pxTopOfStack;		
	
	//任务节点
	ListItem_t				xStateListItem;	
	StackType_t 			*pxStack; 		//任务栈的起始位置
	
	char 					pcTaskName[configMAX_TASK_NAME_LEN];
}tskTCB;

typedef tskTCB TCB_t;

//这个是在portmacro.h文件中实现的, 目的是使能PendSV
#define taskYIELD()			portYIELD()

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,
					            const char * const pcName,
					            const uint32_t ulStackDepth,
					            void * const pvParameters,
					            StackType_t * const puxStackBuffer,
					            TCB_t * const pxTaskBuffer );
#endif /* configSUPPORT_STATIC_ALLOCATION */
                                
void prvInitialiseTaskLists( void );                                
void vTaskStartScheduler( void );
void vTaskSwitchContext( void );

static void prvInitialiseNewTask( 	TaskFunction_t pxTaskCode,              /* 任务入口 */
									const char * const pcName,              /* 任务名称，字符串形式 */
									const uint32_t ulStackDepth,            /* 任务栈大小，单位为字 */
									void * const pvParameters,              /* 任务形参 */
									TaskHandle_t * const pxCreatedTask,     /* 任务句柄 */
									TCB_t *pxNewTCB );   

void prvInitialiseTaskLists( void );
									
									
#endif
