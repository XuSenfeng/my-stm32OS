#ifndef __JIAO_FIFO_H__
#define __JIAO_FIFO_H__

#include "jiao_os.h"

#define FLASG_OVERRUN			0x0001

struct FIFO8{
	uint8_t *buf;
	//��һ����ȡ��λ��,��һ��д���λ��,���������Ĵ�С,���е�λ��,��־
	uint32_t next_r, next_w, size, free, flags;
};
void FIFO8_Init(struct FIFO8 *fifo, int size, unsigned char *buf);
int fifo8_put(struct FIFO8 *fifo, char data);
int FIFO8_Get(struct FIFO8 *fifo);
int FIFO8_Status(struct FIFO8 *fifo);

#endif
