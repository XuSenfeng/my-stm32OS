#include "task.h"

/***********������ʱ�б�************/
static List_t xDelayedTaskList1;
static List_t xDelayedTaskList2;
static List_t * volatile pxDelayedTaskList;
static List_t * volatile pxOverflowDelayedTaskList;
/******************��һ��������ʱ��*******************/
static volatile TickType_t xNextTaskUnblockTime		= ( TickType_t ) 0U;
static volatile BaseType_t xNumOfOverflows 			= ( BaseType_t ) 0;



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
    
    vListInitialiseItem( &( pxNewTCB->xStateListItem ) );
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
    
    /* ��ʼ�������б� */
    for( uxPriority = ( UBaseType_t ) 0U; uxPriority < ( UBaseType_t ) configMAX_PRIORITIES; uxPriority++ )
	{
		vListInitialist( &( pxReadyTasksLists[ uxPriority ] ) );
	}
    
    vListInitialist( &xDelayedTaskList1 );
	vListInitialist( &xDelayedTaskList2 );
    
    pxDelayedTaskList = &xDelayedTaskList1;
	pxOverflowDelayedTaskList = &xDelayedTaskList2;//����ָ��ָ���Ӧ�б�
}

extern TCB_t TASK1_TCB;
extern TCB_t TASK2_TCB;
void vTaskStartScheduler( void )
{
/*======================================������������start==============================================*/     
    TCB_t *pxIdleTaskTCBBuffer = NULL;
    StackType_t *pxIdleTaskStackBuffer = NULL;
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
    xNextTaskUnblockTime = portMAX_DELAY;
    xTickCount = ( TickType_t ) 0U;

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

/******************��������뵽�����б�*********************/
static void prvAddCurrentTaskToDelayedList( TickType_t xTicksToWait )
{
    TickType_t xTimeToWake;
    
    /* ��ȡϵͳʱ��������xTickCount��ֵ */
    const TickType_t xConstTickCount = xTickCount;

    /* ������Ӿ����б����Ƴ� */
	if( uxListRemove( &( pxCurrentTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
	{
		/* �����������ȼ�λͼ�ж�Ӧ��λ��� */
        portRESET_READY_PRIORITY( pxCurrentTCB->uxPriority, uxTopReadyPriority );
	}

    /* ������ʱ����ʱ��ϵͳʱ��������xTickCount��ֵ�Ƕ��� */
    xTimeToWake = xConstTickCount + xTicksToWait;

    /* ����ʱ���ڵ�ֵ����Ϊ�ڵ������ֵ */
    listSET_LIST_ITEM_VALUE( &( pxCurrentTCB->xStateListItem ), xTimeToWake );

    /* ��� */
    if( xTimeToWake < xConstTickCount )
    {
        vListInsert( pxOverflowDelayedTaskList, &( pxCurrentTCB->xStateListItem ) );
    }
    else /* û����� */
    {

        vListInsert( pxDelayedTaskList, &( pxCurrentTCB->xStateListItem ) );

        /* ������һ���������ʱ�̱���xNextTaskUnblockTime��ֵ */
        if( xTimeToWake < xNextTaskUnblockTime )
        {
            xNextTaskUnblockTime = xTimeToWake;
        }
    }	
}

/********������ʱ����**********/
void vTaskDelay( const TickType_t xTicksToDelay )
{
    TCB_t *pxTCB = NULL;
    
    /* ��ȡ��ǰ�����TCB */
    pxTCB = pxCurrentTCB;
    
    /* ������ʱʱ�� */
    //pxTCB->xTicksToDelay = xTicksToDelay;
    
    /* ��������뵽��ʱ�б� */
    prvAddCurrentTaskToDelayedList( xTicksToDelay );
    
    /* �����л� */
    taskYIELD();
}
//����ʱ��
void xTaskIncrementTick( void )
{
	TCB_t * pxTCB;
	TickType_t xItemValue;

	const TickType_t xConstTickCount = xTickCount + 1;
	xTickCount = xConstTickCount;

	/* ���xConstTickCount��������л���ʱ�б� */
	if( xConstTickCount == ( TickType_t ) 0U )
	{
		taskSWITCH_DELAYED_LISTS();
	}

	/* �������ʱ������ʱ���� */
	if( xConstTickCount >= xNextTaskUnblockTime )
	{
		for( ;; )
		{
			if( listLIST_IS_EMPTY( pxDelayedTaskList ) != pdFALSE )
			{
				/* ��ʱ�б�Ϊ�գ�����xNextTaskUnblockTimeΪ���ܵ����ֵ */
				xNextTaskUnblockTime = portMAX_DELAY;
				break;
			}
			else /* ��ʱ�б�Ϊ�� */
			{
				pxTCB = ( TCB_t * ) listGET_OWNER_OF_HEAD_ENTRY( pxDelayedTaskList );
				xItemValue = listGET_LIST_ITEM_VALUE( &( pxTCB->xStateListItem ) );

				/* ֱ������ʱ�б���������ʱ���ڵ������Ƴ�������forѭ�� */
                if( xConstTickCount < xItemValue )
				{
					xNextTaskUnblockTime = xItemValue;
					break;
				}

				/* ���������ʱ�б��Ƴ��������ȴ�״̬ */
				( void ) uxListRemove( &( pxTCB->xStateListItem ) );

				/* ������ȴ���������ӵ������б� */
				prvAddTaskToReadyList( pxTCB );
			}
		}
	}/* xConstTickCount >= xNextTaskUnblockTime */
    
    /* �����л� */
    portYIELD();
}

static void prvAddNewTaskToReadyList( TCB_t *pxNewTCB )
{
	/* �����ٽ�� */
	taskENTER_CRITICAL();
	{
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
 
static void prvResetNextTaskUnblockTime( void )
{
    TCB_t *pxTCB;

	if( listLIST_IS_EMPTY( pxDelayedTaskList ) != pdFALSE )
	{
		/* The new current delayed list is empty.  Set xNextTaskUnblockTime to
		the maximum possible value so it is	extremely unlikely that the
		if( xTickCount >= xNextTaskUnblockTime ) test will pass until
		there is an item in the delayed list. */
		xNextTaskUnblockTime = portMAX_DELAY;
	}
	else
	{
		/* The new current delayed list is not empty, get the value of
		the item at the head of the delayed list.  This is the time at
		which the task at the head of the delayed list should be removed
		from the Blocked state. */
		( pxTCB ) = ( TCB_t * ) listGET_OWNER_OF_HEAD_ENTRY( pxDelayedTaskList );
		xNextTaskUnblockTime = listGET_LIST_ITEM_VALUE( &( ( pxTCB )->xStateListItem ) );
	}
}
