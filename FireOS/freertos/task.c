#include "task.h"
//任务列表, 设置优先级
List_t pxReadyTasksLists[configMAX_PRIORITIES];
TCB_t * volatile pxCurrentTCB = NULL;//指针指向现在正在运行的

static TaskHandle_t xIdleTaskHandle					= NULL;
//时钟计数器
static volatile TickType_t xTickCount 				= ( TickType_t ) 0U;


#if( configSUPPORT_STATIC_ALLOCATION == 1 )
//创建静态函数
TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,           /* 任务入口 */
					            const char * const pcName,           /* 任务名称，字符串形式 */
					            const uint32_t ulStackDepth,         /* 任务栈大小，单位为字 */
					            void * const pvParameters,           /* 任务形参 */
					            StackType_t * const puxStackBuffer,  /* 任务栈起始地址 */
					            TCB_t * const pxTaskBuffer )         /* 任务控制块指针 */
{
	TCB_t *pxNewTCB;
	TaskHandle_t xReturn;

	if( ( pxTaskBuffer != NULL ) && ( puxStackBuffer != NULL ) )
	{		
		pxNewTCB = ( TCB_t * ) pxTaskBuffer; 
		pxNewTCB->pxStack = ( StackType_t * ) puxStackBuffer;

		/* 创建新的任务 */
		prvInitialiseNewTask( pxTaskCode,        /* 任务入口 */
                              pcName,            /* 任务名称，字符串形式 */
                              ulStackDepth,      /* 任务栈大小，单位为字 */ 
                              pvParameters,      /* 任务形参 */
                              &xReturn,          /* 任务句柄 */ 
                              pxNewTCB);         /* 任务栈起始地址 */      

	}
	else
	{
		xReturn = NULL;
	}

	/* 返回任务句柄，如果任务创建成功，此时xReturn应该指向任务控制块 */
    return xReturn;
}
#endif /* configSUPPORT_STATIC_ALLOCATION */


static void prvInitialiseNewTask( 	TaskFunction_t pxTaskCode,              /* 任务入口 */
									const char * const pcName,              /* 任务名称，字符串形式 */
									const uint32_t ulStackDepth,            /* 任务栈大小，单位为字 */
									void * const pvParameters,              /* 任务形参 */
									TaskHandle_t * const pxCreatedTask,     /* 任务句柄 */
									TCB_t *pxNewTCB )                       /* 任务控制块指针 */
{
	StackType_t *pxTopOfStack;
	UBaseType_t x;	
	
	/* 获取栈顶地址 */
	pxTopOfStack = pxNewTCB->pxStack + ( ulStackDepth - ( uint32_t ) 1 );
	//pxTopOfStack = ( StackType_t * ) ( ( ( portPOINTER_SIZE_TYPE ) pxTopOfStack ) & ( ~( ( portPOINTER_SIZE_TYPE ) portBYTE_ALIGNMENT_MASK ) ) );
	/* 向下做8字节对齐, 前三位直接置零 */
	pxTopOfStack = ( StackType_t * ) ( ( ( uint32_t ) pxTopOfStack ) & ( ~( ( uint32_t ) 0x0007 ) ) );	

	/* 将任务的名字存储在TCB中 */
	for( x = ( UBaseType_t ) 0; x < ( UBaseType_t ) configMAX_TASK_NAME_LEN; x++ )
	{
		pxNewTCB->pcTaskName[ x ] = pcName[ x ];

		if( pcName[ x ] == 0x00 )
		{
			break;
		}
	}
	/* 任务名字的长度不能超过configMAX_TASK_NAME_LEN */
	pxNewTCB->pcTaskName[ configMAX_TASK_NAME_LEN - 1 ] = '\0';

    /* 初始化TCB中的xStateListItem节点,这里是把所在的列表指向NULL */
    vListInitialiseItem( &( pxNewTCB->xStateListItem ) );
    /* 设置xStateListItem节点的拥有者 */
	listSET_LIST_ITEM_OWNER( &( pxNewTCB->xStateListItem ), pxNewTCB );
    
    
    /* 初始化任务栈,参数是栈顶地址, 任务函数, 任务参数 */
	pxNewTCB->pxTopOfStack = pxPortInitialiseStack( pxTopOfStack, pxTaskCode, pvParameters );   


	/* 让任务句柄指向任务控制块 */
    if( ( void * ) pxCreatedTask != NULL )
	{		
		*pxCreatedTask = ( TaskHandle_t ) pxNewTCB;
	}
}


/* 初始化任务相关的列表 */
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
    /*======================================创建空闲任务start==============================================*/     
    TCB_t *pxIdleTaskTCBBuffer = NULL;               /* 用于指向空闲任务控制块 */
    StackType_t *pxIdleTaskStackBuffer = NULL;       /* 用于空闲任务栈起始地址 */
    uint32_t ulIdleTaskStackSize;
    
    /* 获取空闲任务的内存：任务栈和任务TCB */
    vApplicationGetIdleTaskMemory( &pxIdleTaskTCBBuffer, 
                                   &pxIdleTaskStackBuffer, 
                                   &ulIdleTaskStackSize );    
    
    xIdleTaskHandle = xTaskCreateStatic( (TaskFunction_t)prvIdleTask,              /* 任务入口 */
					                     (char *)"IDLE",                           /* 任务名称，字符串形式 */
					                     (uint32_t)ulIdleTaskStackSize ,           /* 任务栈大小，单位为字 */
					                     (void *) NULL,                            /* 任务形参 */
					                     (StackType_t *)pxIdleTaskStackBuffer,     /* 任务栈起始地址 */
					                     (TCB_t *)pxIdleTaskTCBBuffer );           /* 任务控制块 */
    /* 将任务添加到就绪列表 */                                 
    vListInsertEnd( &( pxReadyTasksLists[0] ), &( ((TCB_t *)pxIdleTaskTCBBuffer)->xStateListItem ) );
/*======================================创建空闲任务end================================================*/
	
	/* 手动指定第一个运行的任务 */
    pxCurrentTCB = &TASK1_TCB;
    
    /* 启动调度器 */
    if( xPortStartScheduler() != pdFALSE )
    {
        /* 调度器启动成功，则不会返回，即不会来到这里 */
    }
}

void vTaskSwitchContext( void )
{
	/* 如果当前线程是空闲线程，那么就去尝试执行线程1或者线程2，
       看看他们的延时时间是否结束，如果线程的延时时间均没有到期，
       那就返回继续执行空闲线程 */
	if( pxCurrentTCB == &IdleTaskTCB )
	{
		if(TASK1_TCB.xTicksToDelay == 0)
		{            
            pxCurrentTCB =&TASK1_TCB;
		}
		else if(TASK2_TCB.xTicksToDelay == 0)
		{
            pxCurrentTCB =&TASK2_TCB;
		}
		else
		{
			return;		/* 线程延时均没有到期则返回，继续执行空闲线程 */
		} 
	}
	else
	{
		/*如果当前线程是线程1或者线程2的话，检查下另外一个线程,如果另外的线程不在延时中，就切换到该线程
        否则，判断下当前线程是否应该进入延时状态，如果是的话，就切换到空闲线程。否则就不进行任何切换 */
		if(pxCurrentTCB == &TASK1_TCB)
		{
			if(TASK2_TCB.xTicksToDelay == 0)
			{
                pxCurrentTCB =&TASK2_TCB;
			}
			else if(pxCurrentTCB->xTicksToDelay != 0)
			{
                pxCurrentTCB = &IdleTaskTCB;
			}
			else 
			{
				return;		/* 返回，不进行切换，因为两个线程都处于延时中 */
			}
		}
		else if(pxCurrentTCB == &TASK2_TCB)
		{
			if(TASK1_TCB.xTicksToDelay == 0)
			{
                pxCurrentTCB =&TASK1_TCB;
			}
			else if(pxCurrentTCB->xTicksToDelay != 0)
			{
                pxCurrentTCB = &IdleTaskTCB;
			}
			else 
			{
				return;		/* 返回，不进行切换，因为两个线程都处于延时中 */
			}
		}
	}
}
//实现空闲任务
static portTASK_FUNCTION( prvIdleTask, pvParameters )
{
	/* 防止编译器的警告 */
	( void ) pvParameters;
    
    for(;;)
    {
        /* 空闲任务暂时什么都不做 */
    }
}
//实现阻塞延时
void vTaskDelay( const TickType_t xTicksToDelay )
{
    TCB_t *pxTCB = NULL;
    
    /* 获取当前任务的TCB */
    pxTCB = pxCurrentTCB;
    
    /* 设置延时时间 */
    pxTCB->xTicksToDelay = xTicksToDelay;
    
    /* 任务切换 */
    taskYIELD();
}
//更新时钟
void xTaskIncrementTick( void )
{
    TCB_t *pxTCB = NULL;
    BaseType_t i = 0;
    
    /* 更新系统时基计数器xTickCount，xTickCount是一个在port.c中定义的全局变量 */
    const TickType_t xConstTickCount = xTickCount + 1;
    xTickCount = xConstTickCount;

    
    /* 扫描就绪列表中所有线程的xTicksToDelay，如果不为0，则减1 */
	for(i=0; i<configMAX_PRIORITIES; i++)
	{	
		//获取下一个链表的第一个任务
        pxTCB = ( TCB_t * ) listGET_OWNER_OF_HEAD_ENTRY( ( &pxReadyTasksLists[i] ) );
		if(pxTCB->xTicksToDelay > 0)
		{
			pxTCB->xTicksToDelay --;
		}
	}
    
    /* 任务切换 */
    portYIELD();
}


