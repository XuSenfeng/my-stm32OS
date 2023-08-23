#include "jiao_sheet.h"


#define SHEET_USE		1
//夕蚊陣崙潤更悶
static struct SHTCTL ctl;
struct SHEET * Mouse_sht;
extern Mouse_Message_Def Mouse_def;

/**
  * @brief  兜兵晒夕蚊陣崙潤更悶
  * @param  徳鳥議海才錐
  * @retval None
  */
void shtctl_init(int xsize, int ysize)
{
	int i;
	ctl.xsize = xsize;
	ctl.ysize = ysize;
	ctl.top = -1; /* 短嗤聞喘議夕蚊 */
	for (i = 0; i < MAX_SHEETS; i++) {
		ctl.sheets0[i].flags = 0; /* 侭嗤議夕蚊炎芝葎隆聞喘 */
	}

}
/**
  * @brief  賦萩匯倖腎議夕蚊
  * @param  夕蚊陣崙篠贋匂議峺寞
  * @retval None
  */
struct SHEET *sheet_alloc(struct SHTCTL *ctl)
{
	struct SHEET *sht;
	int i;
	//資誼匯倖腎椀議夕蚊
	for (i = 0; i < MAX_SHEETS; i++) {
		if (ctl->sheets0[i].flags == 0) {
			sht = &ctl->sheets0[i];
			sht->flags = SHEET_USE; /* 炎崗了譜崔 */
			sht->height = -1; /* 燕幣聞喘嶄 */
			return sht;
		}
	}
	return 0;	/* 短嗤賦萩撹孔 */
}
/**
  * @brief  譜崔匯倖夕蚊議奉來
  * @param  夕蚊議峺
  * @param  隠贋夕�餤鎚�怏
  * @param  夕�餤蝶�
  * @param  夕�餤朕�
  * @param  邑苧議冲弼
  * @retval None
  */
void sheet_setbuf(struct SHEET *sht, uint8_t *buf, int xsize, int ysize, int col_inv)
{
	sht->buf = buf;
	sht->bxsize = xsize;
	sht->bysize = ysize;
	sht->col_inv = col_inv;
	return;
}
/**
  * @brief  泡仟蝶匯翠曝囃
  * @param  涙
  * @retval None
  */
	//uint16_t temp_buf[16*16];
void sheet_refreshsub(int vx0, int vy0, int vx1, int vy1)
{
	int h, bx, by, bx0, by0, bx1, by1, vy, vx;
	uint16_t width, high, c;
	struct SHEET *sht;
	uint16_t *temp_buf;
	uint8_t  *buf;
	/* refreh議袈律階狛阻�塋焼�議袈律 */
	if (vx0 < 0) { vx0 = 0; }
	if (vy0 < 0) { vy0 = 0; }
	if (vx1 > ctl.xsize) { vx1 = ctl.xsize; }
	if (vy1 > ctl.ysize) { vy1 = ctl.ysize; }

	//譜崔匯倖匝扮議夕蚊
	width =vx1-vx0;
	high = vy1-vy0;
	temp_buf = malloc(width * high *2);
	if(temp_buf != NULL)
	{
	Get_Dasktop_Part(temp_buf, vx0, vy0, width, high);
	for (h = 0; h <= ctl.top; h++) {
		//演煽侭嗤議夕蚊
		sht = ctl.sheets[h];
		//資誼夕蚊佚連
		buf = sht->buf;
		/* 資函匝扮夕蚊厚仟了崔�犇墫斃鴫禝栂志� */
		bx0 = vx0 - sht->vx0;
		by0 = vy0 - sht->vy0;
		bx1 = vx1 - sht->vx0;
		by1 = vy1 - sht->vy0;	
		printf("bx0 = %d %d", bx0, vx0);
		//麻竃栖�犇堽志段埜妻�議扮昨氏竃�嵶別�
		if (bx0 < 0) { bx0 = 0;}
		if (by0 < 0) { by0 = 0;}
		if (bx1 > sht->bxsize) { bx1 = sht->bxsize; }
		if (by1 > sht->bysize) { by1 = sht->bysize; }
		//遍枠頁匝扮夕蚊壓恣円

		//匝扮夕蚊壓恣貧叔
		for (by = by0; by < by1; by++) {
			vy = sht->vy0 + by;
			for (bx = bx0; bx < bx1; bx++) {
				vx = sht->vx0 + bx;
				c = table_rgb565[buf[by * sht->bxsize + bx]];
				if (c != sht->col_inv) {
					temp_buf[(vy-vy0) * width + vx-vx0] = c;
				}
			}
		}
		
	}
	printf("***********width = %d, high = %d**********\n", width, high);
	printf("***********vx0 = %d, vy0 = %d %d %d**********\n\n", vx0, vy0, vy1, vx1);

	boxfill_buf(temp_buf, vx0, vy0, width, high );
	free(temp_buf);
}
	else
		printf("賦萩払移\n");
}
	

//夕蚊了崔議延晒
void sheet_updown(struct SHTCTL *ctl, struct SHEET *sht, int height)
{
	int h, old = sht->height; /* 隠贋個延岻念議互業 */

	/* 斤互業序佩俐屎 */
	if (height > ctl->top + 1) {
		height = ctl->top + 1;
	}
	if (height < -1) {
		height = -1;
	}
	sht->height = height; /* 譜崔�嶬攀銚澡� */

	/* 功象秤趨序佩蛍裂 */
	if (old > height) {	/* 曳參念議夕蚊詰 */
		if (height >= 0) {
			/* �嶬攀塚鴫穢西貧塋� */
			for (h = old; h > height; h--) {
				ctl->sheets[h] = ctl->sheets[h - 1];
				ctl->sheets[h]->height = h;
			}
			ctl->sheets[height] = sht;
		} else {	/* 岻朔音壅�塋章塋� */
			if (ctl->top > old) {
				/* 岻念議夕蚊貧中珊嗤艶議夕蚊 */
				for (h = old; h < ctl->top; h++) {
					ctl->sheets[h] = ctl->sheets[h + 1];
					ctl->sheets[h]->height = h;
				}
			}
			ctl->top--; /* 互業和週 */
		}
		sheet_refreshsub(sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize);
	} else if (old < height) {	/* 曳岻念議夕蚊互 */
		if (old >= 0) {
			/* 岻念議夕蚊厮将�塋樵� */
			for (h = old; h < height; h++) {
				ctl->sheets[h] = ctl->sheets[h + 1];
				ctl->sheets[h]->height = h;
			}
			ctl->sheets[height] = sht;
		} else {	/* 岻念短嗤�塋� */
			/* 遍枠委貧中議夕蚊貧卞 */
			for (h = ctl->top; h >= height; h--) {
				ctl->sheets[h + 1] = ctl->sheets[h];
				ctl->sheets[h + 1]->height = h + 1;
			}
			ctl->sheets[height] = sht;
			ctl->top++; /* 侭嗤夕蚊議方楚紗匯 */
		}
		sheet_refreshsub(sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize);
	}
	return;
}

/**
  * @brief  泡仟蝶匯倖夕蚊議�犇堽志�
  * @param  涙
  * @retval None
  */
void sheet_refresh(struct SHTCTL *ctl, struct SHEET *sht, int bx0, int by0, int bx1, int by1)
{
	if (sht->height >= 0) { /* もしも燕幣嶄なら、仟しい和じきの秤�鵑朴悗辰道�中を宙き岷す */
		sheet_refreshsub(sht->vx0 + bx0, sht->vy0 + by0, sht->vx0 + bx1, sht->vy0 + by1);
	}
	return;
}
/**
  * @brief  夕蚊卞強
  * @param  涙
  * @retval None
  */
void sheet_slide(struct SHEET *sht, int vx0, int vy0)
{
	int old_vx0 = sht->vx0, old_vy0 = sht->vy0;
	sht->vx0 = vx0;
	sht->vy0 = vy0;
	if (sht->height >= 0) { /* 夕蚊議互業葎�塋承塚鴫� */
		printf("圻栖了崔嶷仟紙崙\n");
		sheet_refreshsub(old_vx0, old_vy0, old_vx0 + sht->bxsize, old_vy0 + sht->bysize);
		sheet_refreshsub(vx0, vy0, vx0 + sht->bxsize, vy0 + sht->bysize);
	}
	return;
}
/**
  * @brief  瞥慧匯倖夕蚊
  * @param  夕蚊陣崙匂峺寞
  * @param  蝶匯倖夕蚊議峺寞
  * @retval None
  */
void sheet_free(struct SHTCTL *ctl, struct SHEET *sht)
{
	if (sht->height >= 0) {
		sheet_updown(ctl, sht, -1); /* 貫屎壓聞喘議夕蚊嶄肇侃 */
	}
	sht->flags = 0; /* 隆聞喘 */
	return;
}

void sheet_init(void)
{
	//兜兵晒砿尖議潤更悶
	shtctl_init(ILI9341_MORE_PIXEL, ILI9341_LESS_PIXEL);
	//賦萩報炎潤更悶
	Mouse_sht = sheet_alloc(&ctl);
	//譜崔報炎夕蚊
	sheet_setbuf(Mouse_sht, Mouse_def.mouse, Mouse_def.Width, Mouse_def.High, 	0x9999);
	Mouse_sht->vx0 = 310;
	Mouse_sht->vy0 = 200;
	sheet_updown(&ctl, Mouse_sht, MAX_SHEETS);

}

