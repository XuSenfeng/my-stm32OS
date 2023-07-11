#include "task.h"
//�����б�, �������ȼ�
List_t pxReadyTasksLists[configMAX_PRIORITIES];
TCB_t * volatile pxCurrentTCB = NULL;//ָ��ָ�������������е�
//���������
static volatile UBaseType_t uxCurrentNumberOfTasks 	= ( UBaseType_t ) 0U;
static UBaseType_t uxTaskNumber 					= ( UBaseType_t ) 0U;

static TaskHandle_t xIdleTaskHandle					= NULL;
//ʱ�Ӽ�����
static volatile TickType_t xTickCount 				= ( TickType_t ) 0U;
//��ǰ��������ȼ�
static volatile UBaseType_t uxTopReadyPriority 		= tskIDLE_PRIORITY;

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
//������̬����
TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,           /* ������� */
					            const char * const pcName,           /* �������ƣ��ַ�����ʽ */
					            const uint32_t ulStackDepth,         /* ����ջ��С����λΪ�� */
					            void * const pvParameters,           /* �����β� */
                                UBaseType_t uxPriority,              /* �������ȼ�����ֵԽ�����ȼ�Խ�� */
					            StackType_t * const puxStackBuffer,  /* ����ջ��ʼ��ַ */
					            TCB_t * const pxTaskBuffer )         /* ������ƿ� */
{
	TCB_t *pxNewTCB;
	TaskHandle_t xReturn;

	if( ( pxTaskBuffer != NULL ) && ( puxStackBuffer != NULL ) )
	{		
		pxNewTCB = ( TCB_t * ) pxTaskBuffer; 
		pxNewTCB->pxStack = ( StackType_t * ) puxStackBuffer;

		/* �����µ����� */
		prvInitialiseNewTask( pxTaskCode, pcName, ulStackDepth, pvParameters,uxPriority, &xReturn, pxNewTCB);
        
		/* ��������ӵ������б� */
		prvAddNewTaskToReadyList( pxNewTCB );

	}
	else
	{
		xReturn = NULL;
	}

	return xReturn;
}
#endif /* configSUPPORT_STATIC_ALLOCATION */


static void prvInitialiseNewTask( 	TaskFunction_t pxTaskCode,              /* ������� */
									const char * const pcName,              /* �������ƣ��ַ�����ʽ */
									const uint32_t ulStackDepth,            /* ����ջ��С����λΪ�� */
									void * const pvParameters,              /* �����β� */
                                    UBaseType_t uxPriority,                 /* �������ȼ�����ֵԽ�����ȼ�Խ�� */
									TaskHandle_t * const pxCreatedTask,     /* ������ */
									TCB_t *pxNewTCB )                       /* ������ƿ� */

{
	StackType_t *pxTopOfStack;
	UBaseType_t x;	
	
	/* ��ȡջ����ַ */
	pxTopOfStack = pxNewTCB->pxStack + ( ulStackDepth - ( uint32_t ) 1 );
	//pxTopOfStack = ( StackType_t * ) ( ( ( portPOINTER_SIZE_TYPE ) pxTopOfStack ) & ( ~( ( portPOINTER_SIZE_TYPE ) portBYTE_ALIGNMENT_MASK ) ) );
	/* ������8�ֽڶ��� */
	pxTopOfStack = ( StackType_t * ) ( ( ( uint32_t ) pxTopOfStack ) & ( ~( ( uint32_t ) 0x0007 ) ) );	

	/* ����������ִ洢��TCB�� */
	for( x = ( UBaseType_t ) 0; x < ( UBaseType_t ) configMAX_TASK_NAME_LEN; x++ )
	{
		pxNewTCB->pcTaskName[ x ] = pcName[ x ];

		if( pcName[ x ] == 0x00 )
		{
			break;
		}
	}
	/* �������ֵĳ��Ȳ��ܳ���configMAX_TASK_NAME_LEN */
	pxNewTCB->pcTaskName[ configMAX_TASK_NAME_LEN - 1 ] = '\0';
    
    /* ��ʼ��TCB�е�xStateListItem�ڵ� */
    vListInitialiseItem( &( pxNewTCB->xStateListItem ) );
    /* ����xStateListItem�ڵ��ӵ���� */
	listSET_LIST_ITEM_OWNER( &( pxNewTCB->xStateListItem ), pxNewTCB );
	
    /* ��ʼ�����ȼ� */
	if( uxPriority >= ( UBaseType_t ) configMAX_PRIORITIES )
	{
		uxPriority = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
	}
	pxNewTCB->uxPriority = uxPriority;
    
	/* ��ʼ������ջ */
	pxNewTCB->pxTopOfStack = pxPortInitialiseStack( pxTopOfStack, pxTaskCode, pvParameters ); 
    
    /* ��������ָ��������ƿ� */
    if( ( void * ) pxCreatedTask != NULL )
	{		
		*pxCreatedTask = ( TaskHandle_t ) pxNewTCB;
	}
}


/* ��ʼ��������ص��б� */
void prvInitialiseTaskLists( void )
{
    UBaseType_t uxPriority;
    
    
    for( uxPriority = ( UBaseType_t ) 0U; uxPriority < ( UBaseType_t ) configMAX_PRIORITIES; uxPriority++ )
	{
		vListInitialist( &( pxReadyTasksLists[ uxPriority ] ) );
	}
}

extern TCB_t TASK1_TCB;
extern TCB_t TASK2_TCB;
void vTaskStartScheduler( void )
{
    /*======================================������������start==============================================*/     
    TCB_t *pxIdleTaskTCBBuffer = NULL;               /* ����ָ�����������ƿ� */
    StackType_t *pxIdleTaskStackBuffer = NULL;       /* ���ڿ�������ջ��ʼ��ַ */
    uint32_t ulIdleTaskStackSize;
    
    /* ��ȡ����������ڴ棺����ջ������TCB */
    vApplicationGetIdleTaskMemory( &pxIdleTaskTCBBuffer, 
                                   &pxIdleTaskStackBuffer, 
                                   &ulIdleTaskStackSize );    
    
    xIdleTaskHandle = xTaskCreateStatic( (TaskFunction_t)prvIdleTask,              /* ������� */
					                     (char *)"IDLE",                           /* �������ƣ��ַ�����ʽ */
					                     (uint32_t)ulIdleTaskStackSize ,           /* ����ջ��С����λΪ�� */
					                     (void *) NULL,                            /* �����β� */
                                         (UBaseType_t) tskIDLE_PRIORITY,           /* �������ȼ�����ֵԽ�����ȼ�Խ�� */
					                     (StackType_t *)pxIdleTaskStackBuffer,     /* ����ջ��ʼ��ַ */
					                     (TCB_t *)pxIdleTaskTCBBuffer );           /* ������ƿ� */

/*======================================������������end================================================*/
	
	/* �ֶ�ָ����һ�����е����� */
    //pxCurrentTCB = &TASK1_TCB;
    
    /* ���������� */
    if( xPortStartScheduler() != pdFALSE )
    {
        /* �����������ɹ����򲻻᷵�أ��������������� */
    }
}

void vTaskSwitchContext( void )
{
	/* ��ȡ���ȼ���ߵľ��������TCB��Ȼ����µ�pxCurrentTCB */
    taskSELECT_HIGHEST_PRIORITY_TASK();
}
//ʵ�ֿ�������
static portTASK_FUNCTION( prvIdleTask, pvParameters )
{
	/* ��ֹ�������ľ��� */
	( void ) pvParameters;
    
    for(;;)
    {
        /* ����������ʱʲô������ */
    }
}
//ʵ��������ʱ
void vTaskDelay( const TickType_t xTicksToDelay )
{
    TCB_t *pxTCB = NULL;
    
    /* ��ȡ��ǰ�����TCB */
    pxTCB = pxCurrentTCB;
    
    /* ������ʱʱ�� */
    pxTCB->xTicksToDelay = xTicksToDelay;
    
	    /* ������Ӿ����б��Ƴ� */
    //uxListRemove( &( pxTCB->xStateListItem ) );
    taskRESET_READY_PRIORITY( pxTCB->uxPriority );
    
	
    /* �����л� */
    taskYIELD();
}
//����ʱ��
void xTaskIncrementTick( void )
{
    TCB_t *pxTCB = NULL;
    BaseType_t i = 0;
    
    /* ����ϵͳʱ��������xTickCount��xTickCount��һ����port.c�ж����ȫ�ֱ��� */
    const TickType_t xConstTickCount = xTickCount + 1;
    xTickCount = xConstTickCount;

    
    /* ɨ������б��������̵߳�xTicksToDelay�������Ϊ0�����1 */
	for(i=0; i<configMAX_PRIORITIES; i++)
	{	
		//��ȡ��һ������ĵ�һ������
        pxTCB = ( TCB_t * ) listGET_OWNER_OF_HEAD_ENTRY( ( &pxReadyTasksLists[i] ) );
		if(pxTCB->xTicksToDelay > 0)
		{
			pxTCB->xTicksToDelay --;
			/* ��ʱʱ�䵽����������� */
            if( pxTCB->xTicksToDelay ==0 )
            {
                taskRECORD_READY_PRIORITY( pxTCB->uxPriority );
            }
		}
	}
    
    /* �����л� */
    portYIELD();
}

static void prvAddNewTaskToReadyList( TCB_t *pxNewTCB )
{
	/* �����ٽ�� */
	taskENTER_CRITICAL();
	{
		/* ȫ�������ʱ����һ���� */
        uxCurrentNumberOfTasks++;
        
        /* ���pxCurrentTCBΪ�գ���pxCurrentTCBָ���´��������� */
		if( pxCurrentTCB == NULL )
		{
			pxCurrentTCB = pxNewTCB;

			/* ����ǵ�һ�δ�����������Ҫ��ʼ��������ص��б� */
            if( uxCurrentNumberOfTasks == ( UBaseType_t ) 1 )
			{
				/* ��ʼ��������ص��б� */
                prvInitialiseTaskLists();
			}
		}
		else /* ���pxCurrentTCB��Ϊ�գ��������������ȼ���pxCurrentTCBָ��������ȼ������TCB */
		{
				if( pxCurrentTCB->uxPriority <= pxNewTCB->uxPriority )
				{
					pxCurrentTCB = pxNewTCB;
				}
		}
		uxTaskNumber++;
        
		/* ��������ӵ������б� */
        prvAddTaskToReadyList( pxNewTCB );

	}
	/* �˳��ٽ�� */
	taskEXIT_CRITICAL();
}
 
