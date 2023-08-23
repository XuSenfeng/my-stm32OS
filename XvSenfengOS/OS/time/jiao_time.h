#ifndef __JIAO_TIME_H__
#define __JIAO_TIME_H__

#include "jiao_os.h"

#define MAX_TIMER		5
#define TIMER_FLAGS_ALLOC		1	/* 初始化 */
#define TIMER_FLAGS_USING		2	/* 工作中 */
struct TIMER {
	unsigned int timeout, flags;
	struct FIFO8 *fifo;
	unsigned char data;
};
struct TIMERCTL {
	unsigned int count;
	struct TIMER timer[MAX_TIMER];
};


void Timer_init(void);
void init_time_ctl(void);
void Time_OutEventHandler(void);

#endif
