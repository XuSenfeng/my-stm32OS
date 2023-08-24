#include "./time/jiao_time.h"

/***********************************
初始化一个系统时钟, 这里使用的是TIM7
************************************/


struct TIMERCTL timerctl;
struct TIMER * timer1;
struct TIMER * timer2;
struct FIFO8 timerfifo;
uint8_t timerbuf[8];




/**
  * @brief  初始化时钟的控制器
  * @param  无
  * @retval None
  */
void init_time_ctl()
{
	int i;

	timerctl.count = 0;
	timerctl.next = 0xffffffff;
	timerctl.usings=0;
	for (i = 0; i < MAX_TIMER; i++) {
		timerctl.timers0[i].flags = 0; /* 未使用 */
	}
	return;
}

/**
  * @brief  申请一个时钟
  * @param  无
  * @retval 申请成功返回一个时钟结构体,失败返回0
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
	return 0; /* 申请失败 */
}
/**
  * @brief  释放一个时钟
  * @param  无
  * @retval 申请成功返回一个时钟结构体,失败返回0
  */
void timer_free(struct TIMER *timer)
{
	timer->flags = 0; /* 未使用 */
	return;
}
/**
  * @brief  初始化一个时钟
  * @param  时钟结构体
  * @param  时钟使用的FIFO
  * @param  时钟时间到的时候返回的数值
  * @retval None
  */
void timer_init(struct TIMER *timer, struct FIFO8 *fifo, unsigned char data)
{
	timer->fifo = fifo;
	timer->data = data;
	return;
}
/**
  * @brief  设置一个时钟多长时间后提醒
  * @param  时钟结构体
  * @param  设置的时间
  * @retval None
  */
void timer_settime(struct TIMER *timer, unsigned int timeout)
{
	int i, j;
	__disable_irq();

	timer->timeout = timeout + timerctl.count;
	timer->flags = TIMER_FLAGS_USING;
	/* 搜索注册的位置 */
	for (i = 0; i < timerctl.usings; i++) {
		if (timerctl.timers[i]->timeout >= timer->timeout) {
			break;
		}
	}
	/* i之后的位置后移一位 */
	for (j = timerctl.usings; j > i; j--) {
		timerctl.timers[j] = timerctl.timers[j - 1];
	}
	timerctl.usings++;
	/* 插入到空位上 */
	timerctl.timers[i] = timer;
	timerctl.next = timerctl.timers[0]->timeout;
	__enable_irq();

	return;
}





/**
  * @brief  初始化时钟,在系统初始化的时候会调用
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
  * @brief  在main函数中调用的对时钟超时的处理函数
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
