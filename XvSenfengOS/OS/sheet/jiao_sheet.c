#include "jiao_sheet.h"


#define SHEET_USE		1
//ͼ����ƽṹ��
struct SHTCTL ctl;
struct SHEET * Mouse_sht;
extern Mouse_Message_Def Mouse_def;

/**
  * @brief  ��ʼ��ͼ����ƽṹ��
  * @param  ��Ļ�ĳ��Ϳ�
  * @retval None
  */
void shtctl_init(int xsize, int ysize)
{
	int i;
	ctl.xsize = xsize;
	ctl.ysize = ysize;
	ctl.top = -1; /* û��ʹ�õ�ͼ�� */
	for (i = 0; i < MAX_SHEETS; i++) {
		ctl.sheets0[i].flags = 0; /* ���е�ͼ����Ϊδʹ�� */
	}

}
/**
  * @brief  ����һ���յ�ͼ��
  * @param  ͼ����ƼĴ�����ָ��
  * @retval None
  */
struct SHEET *sheet_alloc(struct SHTCTL *ctl)
{
	struct SHEET *sht;
	int i;
	//���һ�����е�ͼ��
	for (i = 0; i < MAX_SHEETS; i++) {
		if (ctl->sheets0[i].flags == 0) {
			sht = &ctl->sheets0[i];
			sht->flags = SHEET_USE; /* ��־λ���� */
			sht->height = -1; /* ��ʾʹ���� */
			return sht;
		}
	}
	return 0;	/* û������ɹ� */
}
/**
  * @brief  ����һ��ͼ�������
  * @param  ͼ���ָ
  * @param  ����ͼ�������
  * @param  ͼ��ĳ�
  * @param  ͼ��Ŀ�
  * @param  ͸������ɫ
  * @retval None
  */
void sheet_setbuf(struct SHEET *sht, uint16_t *buf, int xsize, int ysize, int col_inv)
{
	sht->buf = buf;
	sht->bxsize = xsize;
	sht->bysize = ysize;
	sht->col_inv = col_inv;
	return;
}
/**
  * @brief  ˢ��ĳһ������
  * @param  ��
  * @retval None
  */
uint16_t temp_buf[32*32];

void sheet_refreshsub(struct SHTCTL *ctl, int vx0, int vy0, int vx1, int vy1)
{
	int h, bx, by, bx0, by0, bx1, by1;
	uint16_t *buf, width, high;
	struct SHEET *sht;

	/* refreh�ķ�Χ��������ʾ���ķ�Χ */
	if (vx0 < 0) { vx0 = 0; }
	if (vy0 < 0) { vy0 = 0; }
	if (vx1 > ctl->xsize) { vx1 = ctl->xsize; }
	if (vy1 > ctl->ysize) { vy1 = ctl->ysize; }

	//����һ����ʱ��ͼ��
	width =vx1-vx0;
	high = vy1-vy0;
	Get_Dasktop_Part(temp_buf, vx0, vy0, width, high);
	for (h = 0; h <= ctl->top; h++) {
		//�������е�ͼ��
		sht = ctl->sheets[h];
		//���ͼ����Ϣ
		buf = sht->buf;
		/* ��ȡ���λ�õĳ�ʼλ�� */
		bx0 = vx0 - sht->vx0;
		by0 = vy0 - sht->vy0;
		bx1 = vx1 - sht->vx0;
		by1 = vy1 - sht->vy0;
		if (bx0 < 0) { bx0 = 0; }
		if (by0 < 0) { by0 = 0; }
		if (bx1 > sht->bxsize) { bx1 = sht->bxsize; }
		if (by1 > sht->bysize) { by1 = sht->bysize; }
		for (by = by0; by < by1; by++) {
			for (bx = bx0; bx < bx1; bx++) {
				//�޸���ʱͼ��
				if(buf[bx + by*sht->bxsize] != sht->col_inv){
					//�������͸����ɫ�ͽ��и���
					printf("����");
					temp_buf[bx + by*width] = buf[bx + by*sht->bxsize];
				}
			}
		}
	}
	printf("***********width = %d, high = %d**********\n", width, high);
	boxfill_buf(temp_buf, vx0, vy0, width, high );
		
}
	

//ͼ��λ�õı仯
void sheet_updown(struct SHTCTL *ctl, struct SHEET *sht, int height)
{
	int h, old = sht->height; /* ����ı�֮ǰ�ĸ߶� */

	/* �Ը߶Ƚ������� */
	if (height > ctl->top + 1) {
		height = ctl->top + 1;
	}
	if (height < -1) {
		height = -1;
	}
	sht->height = height; /* �������ڵĸ߶� */

	/* ����������з��� */
	if (old > height) {	/* ����ǰ��ͼ��� */
		if (height >= 0) {
			/* ���ڵ�ͼ��������ʾ */
			for (h = old; h > height; h--) {
				ctl->sheets[h] = ctl->sheets[h - 1];
				ctl->sheets[h]->height = h;
			}
			ctl->sheets[height] = sht;
		} else {	/* ֮������ʾ��ʾ */
			if (ctl->top > old) {
				/* ֮ǰ��ͼ�����滹�б��ͼ�� */
				for (h = old; h < ctl->top; h++) {
					ctl->sheets[h] = ctl->sheets[h + 1];
					ctl->sheets[h]->height = h;
				}
			}
			ctl->top--; /* �߶��½� */
		}
		sheet_refreshsub(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize);
	} else if (old < height) {	/* ��֮ǰ��ͼ��� */
		if (old >= 0) {
			/* ֮ǰ��ͼ���Ѿ���ʾ�� */
			for (h = old; h < height; h++) {
				ctl->sheets[h] = ctl->sheets[h + 1];
				ctl->sheets[h]->height = h;
			}
			ctl->sheets[height] = sht;
		} else {	/* ֮ǰû����ʾ */
			/* ���Ȱ������ͼ������ */
			for (h = ctl->top; h >= height; h--) {
				ctl->sheets[h + 1] = ctl->sheets[h];
				ctl->sheets[h + 1]->height = h + 1;
			}
			ctl->sheets[height] = sht;
			ctl->top++; /* ����ͼ���������һ */
		}
		sheet_refreshsub(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize);
	}
	return;
}

/**
  * @brief  ˢ��ĳһ��ͼ������λ��
  * @param  ��
  * @retval None
  */
void sheet_refresh(struct SHTCTL *ctl, struct SHEET *sht, int bx0, int by0, int bx1, int by1)
{
	if (sht->height >= 0) { /* �⤷���ʾ�Фʤ顢�¤����¤����������ؤäƻ�����褭ֱ�� */
		sheet_refreshsub(ctl, sht->vx0 + bx0, sht->vy0 + by0, sht->vx0 + bx1, sht->vy0 + by1);
	}
	return;
}
/**
  * @brief  ͼ���ƶ�
  * @param  ��
  * @retval None
  */
void sheet_slide(struct SHTCTL *ctl, struct SHEET *sht, int vx0, int vy0)
{
	int old_vx0 = sht->vx0, old_vy0 = sht->vy0;
	sht->vx0 = vx0;
	sht->vy0 = vy0;
	if (sht->height >= 0) { /* ͼ��ĸ߶�Ϊ��ʾ��ͼ�� */
		sheet_refreshsub(ctl, old_vx0, old_vy0, old_vx0 + sht->bxsize, old_vy0 + sht->bysize);
		sheet_refreshsub(ctl, vx0, vy0, vx0 + sht->bxsize, vy0 + sht->bysize);
	}
	return;
}
/**
  * @brief  �ͷ�һ��ͼ��
  * @param  ͼ�������ָ��
  * @param  ĳһ��ͼ���ָ��
  * @retval None
  */
void sheet_free(struct SHTCTL *ctl, struct SHEET *sht)
{
	if (sht->height >= 0) {
		sheet_updown(ctl, sht, -1); /* ������ʹ�õ�ͼ����ȥ�� */
	}
	sht->flags = 0; /* δʹ�� */
	return;
}

void sheet_init(void)
{
	//��ʼ������Ľṹ��
	shtctl_init(ILI9341_MORE_PIXEL, ILI9341_LESS_PIXEL);
	//�������ṹ��
	Mouse_sht = sheet_alloc(&ctl);
	//�������ͼ��
	sheet_setbuf(Mouse_sht, Mouse_def.mouse, Mouse_def.Width, Mouse_def.High, 0x99);
	Mouse_sht->vx0 = 310;
	Mouse_sht->vy0 = 200;
	sheet_updown(&ctl, Mouse_sht, MAX_SHEETS);

}

