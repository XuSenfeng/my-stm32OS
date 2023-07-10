#include "list.h"
#include "task.h"
/***********��������ջ************/
#define TASK1_STACK_SIZE			20
StackType_t Task1Stack[TASK1_STACK_SIZE];
TCB_t TASK1_TCB;


#define TASK2_STACK_SIZE			20
StackType_t Task2Stack[TASK2_STACK_SIZE];
TCB_t TASK2_TCB;
/*************���������õı�־*************/
portCHAR flag1;
portCHAR flag2;



extern List_t pxReadyTasksLists[configMAX_PRIORITIES];

/************�����ļ����***********/
TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;




/**************����������**************/
void delay(uint32_t count);
void Task1_Entry(void *p_arg);
void Task2_Entry(void *p_arg);

int main(void)
{
	prvInitialiseTaskLists();
	//��������
	Task1_Handle = xTaskCreateStatic(Task1_Entry, "Task1_Entry", TASK1_STACK_SIZE, NULL, Task1Stack, &TASK1_TCB);	
	vListInsert(&pxReadyTasksLists[1], &TASK1_TCB.xStateListItem);	
	
	Task1_Handle = xTaskCreateStatic(Task2_Entry, "Task2_Entry", TASK2_STACK_SIZE, NULL, Task2Stack, &TASK2_TCB);
	vListInsert(&pxReadyTasksLists[1], &TASK1_TCB.xStateListItem);	
	vTaskStartScheduler();

	for(;;)
	{
		/*�պ���*/
	}
}

//����һ������,��һ�����޵�ѭ��
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
		//�����л�
		portYIELD();
	}
}

void delay(uint32_t count)
{
	for(;count !=0;count--);
}




