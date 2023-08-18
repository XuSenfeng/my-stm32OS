#ifndef __JIAO_DASKTOP_H__
#define __JIAO_DASKTOP_H__
#include "jiaoConfig.h"
#include "./lcd/bsp_ili9341_lcd.h"
#define MOUSE_WIDTH	16
#define MOUSE_HIGH	16

/*���ṹ��*/
struct Mouse_Message
{
	//���Ĵ�С
	uint8_t Width, High;
	//���֮ǰ��λ��
	uint16_t x_old;
	uint16_t y_old;
	//������ڵ�λ��
	uint16_t x;
	uint16_t y;
	//��������λ�õ���ɫ��Ϣ, ��������ƶ�֮�������
	char  mouse[MOUSE_WIDTH*MOUSE_HIGH];
	uint16_t Old_color[MOUSE_WIDTH*MOUSE_HIGH];
	
};
typedef struct Mouse_Message Mouse_Message_Def;


void test(void);
void Draw_Dasktop(void);
void Draw_Mouse(uint16_t x, uint16_t y);
void init_mouse_cursor8(char *mouse);

#endif
