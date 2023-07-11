#ifndef __TASK_H
#define __TASK_H
#include "list.h"
#include "FreeRTOS.h"

typedef void * TaskHandle_t;


typedef struct tskTaskControlBlock
{
	//ջ��
	volatile StackType_t	*pxTopOfStack;		
	
	//����ڵ�
	ListItem_t				xStateListItem;	
	StackType_t 			*pxStack; 		//����ջ����ʼλ��
	
	char 					pcTaskName[configMAX_TASK_NAME_LEN];
	
	TickType_t xTicksToDelay; /* ������ʱ */  
}tskTCB;

typedef tskTCB TCB_t;

//�������portmacro.h�ļ���ʵ�ֵ�, Ŀ����ʹ��PendSV
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
void xTaskIncrementTick( void );

static void prvInitialiseNewTask( 	TaskFunction_t pxTaskCode,              /* ������� */
									const char * const pcName,              /* �������ƣ��ַ�����ʽ */
									const uint32_t ulStackDepth,            /* ����ջ��С����λΪ�� */
									void * const pvParameters,              /* �����β� */
									TaskHandle_t * const pxCreatedTask,     /* ������ */
									TCB_t *pxNewTCB );   

void prvInitialiseTaskLists( void );
void vTaskDelay( const TickType_t xTicksToDelay );

									
									
static portTASK_FUNCTION( prvIdleTask, pvParameters );
									

//ʹ�û�ȡ�����������Ϣ
extern void vApplicationGetIdleTaskMemory( TCB_t **ppxIdleTaskTCBBuffer, 
                                    StackType_t **ppxIdleTaskStackBuffer, 
                                    uint32_t *pulIdleTaskStackSize );
//����������ƿ�
extern TCB_t IdleTaskTCB;	//���ƿ�
extern TCB_t TASK2_TCB;
extern TCB_t TASK1_TCB;
#endif
