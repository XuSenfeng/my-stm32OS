#ifndef __JIAO_DASKTOP_H__
#define __JIAO_DASKTOP_H__
#include "jiaoConfig.h"
#include "./lcd/bsp_ili9341_lcd.h"

struct Mouse_Message
{
	uint8_t Width, High;
	uint16_t x_old;
	uint16_t y_old;
	uint16_t x;
	uint16_t y;
	char  mouse[16*16];
	uint16_t Old_color[16*16];
	
};
typedef struct Mouse_Message Mouse_Message_Def;


void test(void);
void Draw_Dasktop(void);
void Draw_Mouse(uint16_t x, uint16_t y);

#endif
