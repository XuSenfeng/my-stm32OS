#include "./time/jiao_time.h"

/***********************************
��ʼ��һ��ϵͳʱ��, ����ʹ�õ���TIM7
************************************/


struct TIMERCTL timerctl;
struct TIMER * timer1;
struct TIMER * timer2;
struct FIFO8 timerfifo;
uint8_t timerbuf[8];




/**
  * @brief  ��ʼ��ʱ�ӵĿ�����
  * @param  ��
  * @retval None
  */
void init_time_ctl()
{
	int i;

	timerctl.count = 0;
	timerctl.next = 0xffffffff;
	timerctl.usings=0;
	for (i = 0; i < MAX_TIMER; i++) {
		timerctl.timers0[i].flags = 0; /* δʹ�� */
	}
	return;
}

/**
  * @brief  ����һ��ʱ��
  * @param  ��
  * @retval ����ɹ�����һ��ʱ�ӽṹ��,ʧ�ܷ���0
  */
struct TIMER *timer_alloc(void)
{
	int i;
	for (i = 0; i < MAX_TIMER; i++) {
		if (timerctl.timers0[i].flags == 0) {
			timerctl.timers0[i].flags = TIMER_FLAGS_ALLOC;
			return &timerctl.timers0[i];
		}
	}
	return 0; /* ����ʧ�� */
}
/**
  * @brief  �ͷ�һ��ʱ��
  * @param  ��
  * @retval ����ɹ�����һ��ʱ�ӽṹ��,ʧ�ܷ���0
  */
void timer_free(struct TIMER *timer)
{
	timer->flags = 0; /* δʹ�� */
	return;
}
/**
  * @brief  ��ʼ��һ��ʱ��
  * @param  ʱ�ӽṹ��
  * @param  ʱ��ʹ�õ�FIFO
  * @param  ʱ��ʱ�䵽��ʱ�򷵻ص���ֵ
  * @retval None
  */
void timer_init(struct TIMER *timer, struct FIFO8 *fifo, unsigned char data)
{
	timer->fifo = fifo;
	timer->data = data;
	return;
}
/**
  * @brief  ����һ��ʱ�Ӷ೤ʱ�������
  * @param  ʱ�ӽṹ��
  * @param  ���õ�ʱ��
  * @retval None
  */
void timer_settime(struct TIMER *timer, unsigned int timeout)
{
	int i, j;
	__disable_irq();

	timer->timeout = timeout + timerctl.count;
	timer->flags = TIMER_FLAGS_USING;
	/* ����ע���λ�� */
	for (i = 0; i < timerctl.usings; i++) {
		if (timerctl.timers[i]->timeout >= timer->timeout) {
			break;
		}
	}
	/* i֮���λ�ú���һλ */
	for (j = timerctl.usings; j > i; j--) {
		timerctl.timers[j] = timerctl.timers[j - 1];
	}
	timerctl.usings++;
	/* ���뵽��λ�� */
	timerctl.timers[i] = timer;
	timerctl.next = timerctl.timers[0]->timeout;
	__enable_irq();

	return;
}





/**
  * @brief  ��ʼ��ʱ��,��ϵͳ��ʼ����ʱ������
  * @param  None
  * @retval None
  */
void Timer_init()
{

	FIFO8_Init(&timerfifo, 8, timerbuf);

	timer1 = timer_alloc();
	timer2 = timer_alloc();
	timer_init(timer1, &timerfifo, 1);
	timer_init(timer2, &timerfifo, 2);
	timer_settime(timer1, 1000);
	timer_settime(timer2, 500);
}
/**
  * @brief  ��main�����е��õĶ�ʱ�ӳ�ʱ�Ĵ�����
  * @param  None
  * @retval None
  */
void Time_OutEventHandler()
{
	static uint8_t i = 0;
	if(FIFO8_Status(&timerfifo))
	{
		
		i = FIFO8_Get(&timerfifo);
		if(i==1)	
		{
			printf("\ntime1out i = %d\n\n", i);
			timer_settime(timer1, 1000);
		}else if(i==2)
		{
			printf("\ntime2out i = %d\n\n", i);
			timer_settime(timer2, 500);
		}
	}
	
}
