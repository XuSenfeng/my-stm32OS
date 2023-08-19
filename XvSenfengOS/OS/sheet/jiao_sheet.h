#ifndef __JIAO_SHEET_H__
#define __JIAO_SHEET_H__

#define MAX_SHEETS		5
#include "stm32f10x.h"
struct SHEET {
	uint16_t *buf;
	int bxsize, bysize, vx0, vy0, col_inv, height, flags;
};
struct SHTCTL {
	int xsize, ysize, top;
	struct SHEET *sheets[MAX_SHEETS];
	struct SHEET sheets0[MAX_SHEETS];
};


#endif
