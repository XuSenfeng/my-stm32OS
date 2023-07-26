#ifndef __JIAO_DASKTOP_H__
#define __JIAO_DASKTOP_H__
#include "jiaoConfig.h"
#include "./lcd/bsp_ili9341_lcd.h"

struct Mouse_Message
{
	uint16_t x_old;
	uint16_t y_old;
	uint16_t x;
	uint16_t y;
	
};
typedef struct Mouse_Message Mouse_Message_Def;


void test(void);
void Draw_Dasktop(void);
void Draw_Mouse(uint16_t x, uint16_t y);

#endif
