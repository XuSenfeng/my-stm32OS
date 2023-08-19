#include "jiao_sheet.h"


#define SHEET_USE		1
//图层控制结构体
struct SHTCTL ctl;
struct SHEET * Mouse_sht;
extern Mouse_Message_Def Mouse_def;

/**
  * @brief  初始化图层控制结构体
  * @param  屏幕的长和宽
  * @retval None
  */
void shtctl_init(int xsize, int ysize)
{
	int i;
	ctl.xsize = xsize;
	ctl.ysize = ysize;
	ctl.top = -1; /* 没有使用的图层 */
	for (i = 0; i < MAX_SHEETS; i++) {
		ctl.sheets0[i].flags = 0; /* 所有的图层标记为未使用 */
	}

}
/**
  * @brief  申请一个空的图层
  * @param  图层控制寄存器的指针
  * @retval None
  */
struct SHEET *sheet_alloc(struct SHTCTL *ctl)
{
	struct SHEET *sht;
	int i;
	//获得一个空闲的图层
	for (i = 0; i < MAX_SHEETS; i++) {
		if (ctl->sheets0[i].flags == 0) {
			sht = &ctl->sheets0[i];
			sht->flags = SHEET_USE; /* 标志位设置 */
			sht->height = -1; /* 表示使用中 */
			return sht;
		}
	}
	return 0;	/* 没有申请成功 */
}
/**
  * @brief  设置一个图层的属性
  * @param  图层的指
  * @param  保存图像的数组
  * @param  图像的长
  * @param  图像的宽
  * @param  透明的颜色
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
  * @brief  刷新某一块区域
  * @param  无
  * @retval None
  */
uint16_t temp_buf[32*32];

void sheet_refreshsub(struct SHTCTL *ctl, int vx0, int vy0, int vx1, int vy1)
{
	int h, bx, by, bx0, by0, bx1, by1;
	uint16_t *buf, width, high;
	struct SHEET *sht;

	/* refreh的范围超过了显示器的范围 */
	if (vx0 < 0) { vx0 = 0; }
	if (vy0 < 0) { vy0 = 0; }
	if (vx1 > ctl->xsize) { vx1 = ctl->xsize; }
	if (vy1 > ctl->ysize) { vy1 = ctl->ysize; }

	//设置一个临时的图层
	width =vx1-vx0;
	high = vy1-vy0;
	Get_Dasktop_Part(temp_buf, vx0, vy0, width, high);
	for (h = 0; h <= ctl->top; h++) {
		//遍历所有的图层
		sht = ctl->sheets[h];
		//获得图层信息
		buf = sht->buf;
		/* 获取相对位置的初始位置 */
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
				//修改临时图层
				if(buf[bx + by*sht->bxsize] != sht->col_inv){
					//如果不是透明颜色就进行更换
					printf("更换");
					temp_buf[bx + by*width] = buf[bx + by*sht->bxsize];
				}
			}
		}
	}
	printf("***********width = %d, high = %d**********\n", width, high);
	boxfill_buf(temp_buf, vx0, vy0, width, high );
		
}
	

//图层位置的变化
void sheet_updown(struct SHTCTL *ctl, struct SHEET *sht, int height)
{
	int h, old = sht->height; /* 保存改变之前的高度 */

	/* 对高度进行修正 */
	if (height > ctl->top + 1) {
		height = ctl->top + 1;
	}
	if (height < -1) {
		height = -1;
	}
	sht->height = height; /* 设置现在的高度 */

	/* 根据情况进行分析 */
	if (old > height) {	/* 比以前的图层低 */
		if (height >= 0) {
			/* 现在的图层依旧显示 */
			for (h = old; h > height; h--) {
				ctl->sheets[h] = ctl->sheets[h - 1];
				ctl->sheets[h]->height = h;
			}
			ctl->sheets[height] = sht;
		} else {	/* 之后不再显示显示 */
			if (ctl->top > old) {
				/* 之前的图层上面还有别的图层 */
				for (h = old; h < ctl->top; h++) {
					ctl->sheets[h] = ctl->sheets[h + 1];
					ctl->sheets[h]->height = h;
				}
			}
			ctl->top--; /* 高度下降 */
		}
		sheet_refreshsub(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize);
	} else if (old < height) {	/* 比之前的图层高 */
		if (old >= 0) {
			/* 之前的图层已经显示了 */
			for (h = old; h < height; h++) {
				ctl->sheets[h] = ctl->sheets[h + 1];
				ctl->sheets[h]->height = h;
			}
			ctl->sheets[height] = sht;
		} else {	/* 之前没有显示 */
			/* 首先把上面的图层上移 */
			for (h = ctl->top; h >= height; h--) {
				ctl->sheets[h + 1] = ctl->sheets[h];
				ctl->sheets[h + 1]->height = h + 1;
			}
			ctl->sheets[height] = sht;
			ctl->top++; /* 所有图层的数量加一 */
		}
		sheet_refreshsub(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize);
	}
	return;
}

/**
  * @brief  刷新某一个图层的相对位置
  * @param  无
  * @retval None
  */
void sheet_refresh(struct SHTCTL *ctl, struct SHEET *sht, int bx0, int by0, int bx1, int by1)
{
	if (sht->height >= 0) { /* もしも表示中なら、新しい下じきの情報に沿って画面を描き直す */
		sheet_refreshsub(ctl, sht->vx0 + bx0, sht->vy0 + by0, sht->vx0 + bx1, sht->vy0 + by1);
	}
	return;
}
/**
  * @brief  图层移动
  * @param  无
  * @retval None
  */
void sheet_slide(struct SHTCTL *ctl, struct SHEET *sht, int vx0, int vy0)
{
	int old_vx0 = sht->vx0, old_vy0 = sht->vy0;
	sht->vx0 = vx0;
	sht->vy0 = vy0;
	if (sht->height >= 0) { /* 图层的高度为显示的图层 */
		sheet_refreshsub(ctl, old_vx0, old_vy0, old_vx0 + sht->bxsize, old_vy0 + sht->bysize);
		sheet_refreshsub(ctl, vx0, vy0, vx0 + sht->bxsize, vy0 + sht->bysize);
	}
	return;
}
/**
  * @brief  释放一个图层
  * @param  图层控制器指针
  * @param  某一个图层的指针
  * @retval None
  */
void sheet_free(struct SHTCTL *ctl, struct SHEET *sht)
{
	if (sht->height >= 0) {
		sheet_updown(ctl, sht, -1); /* 从正在使用的图层中去处 */
	}
	sht->flags = 0; /* 未使用 */
	return;
}

void sheet_init(void)
{
	//初始化管理的结构体
	shtctl_init(ILI9341_MORE_PIXEL, ILI9341_LESS_PIXEL);
	//申请鼠标结构体
	Mouse_sht = sheet_alloc(&ctl);
	//设置鼠标图层
	sheet_setbuf(Mouse_sht, Mouse_def.mouse, Mouse_def.Width, Mouse_def.High, 0x99);
	Mouse_sht->vx0 = 310;
	Mouse_sht->vy0 = 200;
	sheet_updown(&ctl, Mouse_sht, MAX_SHEETS);

}

