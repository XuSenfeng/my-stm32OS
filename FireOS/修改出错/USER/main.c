#include "list.h"
#include "task.h"
/***********定义任务栈************/
#define TASK1_STACK_SIZE		128
StackType_t Task1Stack[TASK1_STACK_SIZE];
TCB_t TASK1_TCB;


#define TASK2_STACK_SIZE		128
StackType_t Task2Stack[TASK2_STACK_SIZE];
TCB_t TASK2_TCB;
/*************测试任务用的标志*************/
portCHAR flag1;
portCHAR flag2;



extern List_t pxReadyTasksLists[configMAX_PRIORITIES];

/************定义文件句柄***********/
TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;




/**************任务函数声明**************/
void delay(uint32_t count);
void Task1_Entry(void *p_arg);
void Task2_Entry(void *p_arg);

int main(void)
{
	//初始化时钟中断
	vPortSetupTimerInterrupt();
	prvInitialiseTaskLists();
	//创建任务
	Task1_Handle = xTaskCreateStatic(Task1_Entry, "Task1_Entry", TASK1_STACK_SIZE, NULL,3, Task1Stack, &TASK1_TCB);	
	//vListInsert(&pxReadyTasksLists[1], &TASK1_TCB.xStateListItem);	
	
	Task1_Handle = xTaskCreateStatic(Task2_Entry, "Task2_Entry", TASK2_STACK_SIZE, NULL,2, Task2Stack, &TASK2_TCB);
	//vListInsert(&pxReadyTasksLists[2], &TASK2_TCB.xStateListItem);	
	vTaskStartScheduler();

	for(;;)
	{
		/*空函数*/
	}
}

//定义一个函数,是一个无限的循环
void Task1_Entry(void *p_arg)
{

	for(;;)
	{
		flag1=1;
		vTaskDelay(2);
		
		flag1=0;
		vTaskDelay(2);
	}
}

void Task2_Entry(void *p_arg)
{

	for(;;)
	{
		flag2=1;
		vTaskDelay(2);
		
		flag2=0;
		vTaskDelay(2);
		//进行切换
	}
}

void delay(uint32_t count)
{
	for(;count !=0;count--);
}

/**************空闲任务*****************/
StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];
TCB_t IdleTaskTCB;	//控制块

void vApplicationGetIdleTaskMemory( TCB_t **ppxIdleTaskTCBBuffer, 
                                    StackType_t **ppxIdleTaskStackBuffer, 
                                    uint32_t *pulIdleTaskStackSize )
{
		*ppxIdleTaskTCBBuffer=&IdleTaskTCB;
		*ppxIdleTaskStackBuffer=IdleTaskStack; 
		*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;
}


