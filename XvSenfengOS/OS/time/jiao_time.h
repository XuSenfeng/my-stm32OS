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
	//记录开机到现在的时间,下一个需要处理的时间,正在使用的时钟的数量
	unsigned int count, next, usings;
	//记录待处理的时钟
	struct TIMER *timers[MAX_TIMER];
	//记录所有的时钟
	struct TIMER timers0[MAX_TIMER];
	
};


void Timer_init(void);
void init_time_ctl(void);
void Time_OutEventHandler(void);

#endif
