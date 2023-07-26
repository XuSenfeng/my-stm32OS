#include "jiao_dasktop.h"

Mouse_Message_Def Mouse_def;

uint16_t table_rgb565[16] = {
	0x0000,	/*  0:�\ */
	0xf800,	/*  1:���� */
	0x07e0,	/*  2:�� */
	0xffe0,	/*  3:�� */
	0x001f,	/*  4:�� */
	0xf817,	/*  5:�� */
	0x07ff,	/*  6:ǳ��*/
	0xffff,	/*  7:�� */
	0xc638,	/*  8:�� */
	0x8000,	/*  9:���� */
	0x0420,	/* 10:���v */
	0x8420,	/* 11:����ɫ */
	0x0010,	/* 12:���� */
	0x8010,	/* 13:���� */
	0x0430,	/* 14:����ɫ */
	0x8430	/* 15:����ɫ */
};
//����һ�������λ���һ����
void boxfill8(unsigned char c, int x0, int y0, int x1, int y1)
{
	
	ILI9341_OpenWindow ( x0, y0, (x1-x0+1), (y1-y0+1) );
	ILI9341_FillColor ( (x1-x0+1)*(y1-y0+1) ,table_rgb565[c]);

	return;
}
//��������
void Draw_Dasktop(void)
{
	int xsize, ysize;
	xsize =320;
	ysize = 240;
	//ILI9341_GramScan(3);
	boxfill8(COL8_008484,  0,         0,          xsize, ysize - 29);
	boxfill8(COL8_C6C6C6,  0,         ysize - 28, xsize -  1, ysize - 28);
	boxfill8(COL8_FFFFFF,  0,         ysize - 27, xsize -  1, ysize - 27);
	boxfill8(COL8_C6C6C6,  0,         ysize - 26, xsize -  1, ysize -  1);

	boxfill8(COL8_FFFFFF,  3,         ysize - 24, 59,         ysize - 24);
	boxfill8(COL8_FFFFFF,  2,         ysize - 24,  2,         ysize -  4);
	boxfill8(COL8_848484,  3,         ysize -  4, 59,         ysize -  4);
	boxfill8(COL8_848484, 59,         ysize - 23, 59,         ysize -  5);
	boxfill8(COL8_000000,  2,         ysize -  3, 59,         ysize -  3);
	boxfill8(COL8_000000, 60,         ysize - 24, 60,         ysize -  3);

	boxfill8(COL8_848484, xsize - 273, ysize - 24, xsize -  4, ysize - 24);
	boxfill8(COL8_848484, xsize - 47, ysize - 23, xsize - 47, ysize -  4);
	boxfill8(COL8_FFFFFF, xsize - 47, ysize -  3, xsize -  4, ysize -  3);
	boxfill8(COL8_FFFFFF, xsize -  3, ysize - 24, xsize -  3, ysize -  3);
	//boxfill8(COL8_000000, 20, 30, 30, 50);

}

#if Jiao_Debug

void test(void)
{
	uint16_t data[20];
	uint8_t i, j;

	for(i=0; i<16; i++)
	{

		
			boxfill8(i, i, 10, i, 10);
		
	}
	ILI9341_Read_Datas(data, 0,10, 16, 1);

	for(i=0; i<4; i++)
	{
		for(j=0;j<4;j++)
		{
			printf("cl%d= 0x%x\t", (i*4+j), data[i*4+j]);
		}
		printf("\n");
	}
	ILI9341_DispString_EN_CH(0, 220, "jhy������");
	ILI9341_DisplayStringEx(0, 120, 20, 40, "jhy������", 0);
}

void init_mouse_cursor8(char *mouse, char bc)
/* ��ʼ�����ṹ�� */
{
	static char cursor[16][16] = {
		"**************..",
		"*OOOOOOOOOOO*...",
		"*OOOOOOOOOO*....",
		"*OOOOOOOOO*.....",
		"*OOOOOOOO*......",
		"*OOOOOOO*.......",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOO**OOO*.....",
		"*OOO*..*OOO*....",
		"*OO*....*OOO*...",
		"*O*......*OOO*..",
		"**........*OOO*.",
		"*..........*OOO*",
		"............*OO*",
		".............***"
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == '*') {
				mouse[y * 16 + x] = COL8_000000;
			}
			if (cursor[y][x] == 'O') {
				mouse[y * 16 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.') {
				mouse[y * 16 + x] = bc;
			}
		}
	}
	return;
}



void Draw_Mouse(uint16_t x, uint16_t y)
{
	char  mouse[16*16];
	Mouse_def.x_old = Mouse_def.x;
	Mouse_def.y_old = Mouse_def.y;
	Mouse_def.x = x;
	Mouse_def.y = y;
	init_mouse_cursor8(mouse, COL8_008484);
	putblock8_8(16, 16, mouse);
}
#endif
