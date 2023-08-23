#include "jiao_FIFO.h"
/**
  * @brief  ��ʼ��FIFO
  * @param  Ҫ��ʼ���Ľṹ��ָ��
  * @param  �ṹ��Ĵ�С
  * @param  ���ݴ洢������
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
  * @brief  ���һ������
  * @param  ���ݴ洢�Ľṹ��ָ��
  * @param  Ҫ���������
  * @retval �����Ƿ����,����Ļ�Ϊ-1,����Ϊ0
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
  * @brief  ��ȡһ������
  * @param  �����Ľṹ��
  * @retval �����Ƿ�Ϊ��,����Ļ�Ϊ-1,����Ϊ��ȡ��������
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
  * @brief  ������ݵĶ���
  * @param  �����Ľṹ��
  * @retval �洢���ݵĸ���
  */
int FIFO8_Status(struct FIFO8 *fifo)
{
	return fifo->size - fifo->free;
}




