#ifndef __JIAO_FIFO_H__
#define __JIAO_FIFO_H__

#include "jiao_os.h"

#define FLASG_OVERRUN			0x0001

struct FIFO_8{
	uint8_t *buf;
	//��һ����ȡ��λ��,��һ��д���λ��,���������Ĵ�С,���е�λ��,��־
	uint32_t next_r, next_w, size, free, flags;
};

#endif
