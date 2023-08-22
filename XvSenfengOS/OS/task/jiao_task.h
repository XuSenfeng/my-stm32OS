#ifndef __JIAO_TASK_H__
#define __JIAO_TASK_H__

#include "jiao_list.h"
#include <stdio.h>
#include "jiao_os.h"
#include "bsp_led.h"
#define configMAX_TASK_NAME_LEN		            ( 16 )
#define configMAX_PRIORITIES		            ( 5 )
#define portINITIAL_XPSR			        ( 0x01000000 )
#define portSTART_ADDRESS_MASK				( ( StackType_t ) 0xfffffffeUL )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	191   /* ����λ��Ч��������0xb0��������11 */
//���������
typedef struct tskTaskControlBlock
{
	volatile uint32_t    *pxTopOfStack;    /* ջ�� */

	ListItem_t			    xStateListItem;   /* ����ڵ� */
    
    uint32_t             *pxStack;         /* ����ջ��ʼ��ַ */
	                                          /* �������ƣ��ַ�����ʽ */
	char                    pcTaskName[ configMAX_TASK_NAME_LEN ];  
} tskTCB;

//�ں˿��ƼĴ���
#define portNVIC_SYSPRI2_REG				( * ( ( volatile uint32_t * ) 0xe000ed20 ) )
//���ȼ�����
#define configKERNEL_INTERRUPT_PRIORITY 		255   /* ����λ��Ч��������0xff��������15 */

#define portNVIC_PENDSV_PRI					( ( ( uint32_t ) configKERNEL_INTERRUPT_PRIORITY ) << 16UL )
#define portNVIC_SYSTICK_PRI				( ( ( uint32_t ) configKERNEL_INTERRUPT_PRIORITY ) << 24UL )
//ͬ���жϺ���������
#define xPortPendSVHandler   PendSV_Handler
#define xPortSysTickHandler  SysTick_Handler
#define vPortSVCHandler      SVC_Handler

//һЩ�������¶���
typedef tskTCB TCB_t;
typedef void * TaskHandle_t;
typedef void (*TaskFunction_t)( void * );
typedef uint32_t StackType_t;
typedef long BaseType_t;


/* �жϿ���״̬�Ĵ�����0xe000ed04
 * Bit 28 PENDSVSET: PendSV ����λ
 */
 //PandSV�Ŀ������ĵ�ַ
#define portNVIC_INT_CTRL_REG		( * ( ( volatile uint32_t * ) 0xe000ed04 ) )
//�����λ��
#define portNVIC_PENDSVSET_BIT		( 1UL << 28UL )

#define portSY_FULL_READ_WRITE		( 15 )
//��������λ�ĺ���,֮������PandSV����
#define portYIELD()																\
{																				\
	/* ����PendSV�������������л� */								                \
	portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;								\
	__dsb( portSY_FULL_READ_WRITE );											\
	__isb( portSY_FULL_READ_WRITE );											\
}
#define taskYIELD()			portYIELD()



#define pdFALSE			( ( BaseType_t ) 0 )
#define pdTRUE			( ( BaseType_t ) 1 )

StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters );

TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,           /* ������� */
					            const char * const pcName,           /* �������ƣ��ַ�����ʽ */
					            const uint32_t ulStackDepth,         /* ����ջ��С����λΪ�� */
					            void * const pvParameters,           /* �����β� */
					            uint32_t * const puxStackBuffer,  /* ����ջ��ʼ��ַ */
					            TCB_t * const pxTaskBuffer );         /* ������ƿ�ָ�� */
static void prvInitialiseNewTask( 	TaskFunction_t pxTaskCode,              /* ������� */
									const char * const pcName,              /* �������ƣ��ַ�����ʽ */
									const uint32_t ulStackDepth,            /* ����ջ��С����λΪ�� */
									void * const pvParameters,              /* �����β� */
									TaskHandle_t * const pxCreatedTask,     /* ������ */
									TCB_t *pxNewTCB );                     /* ������ƿ�ָ�� */
void prvInitialiseTaskLists( void );
BaseType_t xPortStartScheduler( void );
void vTaskSwitchContext( void );
									
									
#if Jiao_Debug

void Task_main(void);
#endif

#endif
