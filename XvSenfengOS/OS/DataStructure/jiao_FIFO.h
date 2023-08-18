#ifndef __JIAO_FIFO_H__
#define __JIAO_FIFO_H__

#include "jiao_os.h"

#define FLASG_OVERRUN			0x0001

struct FIFO_8{
	uint8_t *buf;
	//下一个读取的位置,下一个写入的位置,整个容器的大小,空闲的位置,标志
	uint32_t next_r, next_w, size, free, flags;
};

#endif
