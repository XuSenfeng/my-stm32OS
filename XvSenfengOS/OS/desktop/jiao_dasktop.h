#ifndef __JIAO_DASKTOP_H__
#define __JIAO_DASKTOP_H__
#include "jiaoConfig.h"
#include "./lcd/bsp_ili9341_lcd.h"
#define MOUSE_WIDTH	16
#define MOUSE_HIGH	16

/*鼠标结构体*/
struct Mouse_Message
{
	//鼠标的大小
	uint8_t Width, High;
	//鼠标之前的位置
	uint16_t x_old;
	uint16_t y_old;
	//鼠标现在的位置
	uint16_t x;
	uint16_t y;
	//保存鼠标的位置的颜色信息, 用于鼠标移动之后的设置
	char  mouse[MOUSE_WIDTH*MOUSE_HIGH];
	uint16_t Old_color[MOUSE_WIDTH*MOUSE_HIGH];
	
};
typedef struct Mouse_Message Mouse_Message_Def;


void test(void);
void Draw_Dasktop(void);
void Draw_Mouse(uint16_t x, uint16_t y);
void init_mouse_cursor8(char *mouse);

#endif
