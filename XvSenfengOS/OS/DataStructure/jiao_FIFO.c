#include "jiao_FIFO.h"
/**
  * @brief  初始化FIFO
  * @param  要初始化的结构体指针
  * @param  结构体的大小
  * @param  数据存储的数组
  * @retval None
  */
void FIFO8_Init(struct FIFO8 *fifo, int size, unsigned char *buf)
{
	fifo->size = size;
	fifo->buf = buf;
	fifo->free = size;
	fifo->next_w = 0;
	fifo->next_r = 0;
	
}
/**
  * @brief  存放一个数据
  * @param  数据存储的结构体指针
  * @param  要保存的数据
  * @retval 数据是否溢出,溢出的话为-1,否则为0
  */
int fifo8_put(struct FIFO8 *fifo, char data)
{
	if(fifo->free==0)
	{
		fifo->flags |= FLASG_OVERRUN;
		return -1;
	}
	fifo->buf[fifo->next_w] = data;
	fifo->next_w++;
	if(fifo->next_w == fifo->size)
	{
		fifo->next_w = 0;
	}
	fifo->free--;
	return 0;
}
/**
  * @brief  获取一个数据
  * @param  操作的结构体
  * @retval 数据是否为空,溢出的话为-1,否则为获取到的数据
  */
int FIFO8_Get(struct FIFO8 *fifo)
{
	int data;
	if(fifo->free == fifo->size)
	{
		return -1;
	}
	data = fifo->buf[fifo->next_r];
	fifo->next_r++;
	if(fifo->next_r==fifo->size)
	{
		fifo->next_r=0;
	}
	fifo->free++;
	return data;
}
/**
  * @brief  获得数据的多少
  * @param  操作的结构体
  * @retval 存储数据的个数
  */
int FIFO8_Status(struct FIFO8 *fifo)
{
	return fifo->size - fifo->free;
}




