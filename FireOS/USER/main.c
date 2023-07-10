#include "list.h"
#include "task.h"
/***********定义任务栈************/
#define TASK1_STACK_SIZE			20
StackType_t Task1Stack[TASK1_STACK_SIZE];
TCB_t TASK1_TCB;


#define TASK2_STACK_SIZE			20
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
	prvInitialiseTaskLists();
	//创建任务
	Task1_Handle = xTaskCreateStatic(Task1_Entry, "Task1_Entry", TASK1_STACK_SIZE, NULL, Task1Stack, &TASK1_TCB);	
	vListInsert(&pxReadyTasksLists[1], &TASK1_TCB.xStateListItem);	
	
	Task1_Handle = xTaskCreateStatic(Task2_Entry, "Task2_Entry", TASK2_STACK_SIZE, NULL, Task2Stack, &TASK2_TCB);
	vListInsert(&pxReadyTasksLists[1], &TASK1_TCB.xStateListItem);	
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
		delay(100);
		
		flag1=0;
		delay(100);
		portYIELD();
	}
}

void Task2_Entry(void *p_arg)
{

	for(;;)
	{
		flag2=1;
		delay(100);
		
		flag2=0;
		delay(100);
		//进行切换
		portYIELD();
	}
}

void delay(uint32_t count)
{
	for(;count !=0;count--);
}




