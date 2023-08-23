#include "./time/jiao_time.h"

struct TIMERCTL timerctl;
struct TIMER * timer1;
struct TIMER * timer2;
struct FIFO8 timerfifo1;
uint8_t timerbuf1[8];
struct FIFO8 timerfifo2;
uint8_t timerbuf2[8];
void init_time_ctl()
{
	int i;

	timerctl.count = 0;
	timerctl.next = 0xffffffff;
	for (i = 0; i < MAX_TIMER; i++) {
		timerctl.timer[i].flags = 0; /* 未使用 */
	}
	return;
}
struct TIMER *timer_alloc(void)
{
	int i;
	for (i = 0; i < MAX_TIMER; i++) {
		if (timerctl.timer[i].flags == 0) {
			timerctl.timer[i].flags = TIMER_FLAGS_ALLOC;
			return &timerctl.timer[i];
		}
	}
	return 0; /* 見つからなかった */
}

void timer_free(struct TIMER *timer)
{
	timer->flags = 0; /* 未使用 */
	return;
}

void timer_init(struct TIMER *timer, struct FIFO8 *fifo, unsigned char data)
{
	timer->fifo = fifo;
	timer->data = data;
	return;
}

void timer_settime(struct TIMER *timer, unsigned int timeout)
{
	timer->timeout = timeout + timerctl.count;
	timer->flags = TIMER_FLAGS_USING;
	if (timerctl.next > timer->timeout) {
		/* 更新下一次的时间 */
		timerctl.next = timer->timeout;
	}	
	return;
}






void Timer_init()
{

	FIFO8_Init(&timerfifo1, 8, timerbuf1);
	FIFO8_Init(&timerfifo2, 8, timerbuf2);
	timer1 = timer_alloc();
	timer2 = timer_alloc();
	timer_init(timer1, &timerfifo1, 1);
	timer_init(timer2, &timerfifo2, 1);
	timer_settime(timer1, 1000);
	timer_settime(timer2, 500);
}

void Time_OutEventHandler()
{
	static uint8_t i = 0;
	if(FIFO8_Status(&timerfifo1))
	{
		i = FIFO8_Get(&timerfifo1);
		printf("\ntime1out i = %d\n\n", i);
		i++;
		timer_settime(timer1, 1000);
	}
	if(FIFO8_Status(&timerfifo2))
	{
		i = FIFO8_Get(&timerfifo2);
		printf("\ntime2out i = %d\n\n", i);
		i++;
		timer_settime(timer2, 500);
	}	
}
