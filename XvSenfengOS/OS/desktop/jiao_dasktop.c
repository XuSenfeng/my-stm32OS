#include "jiao_dasktop.h"




void boxfill8(unsigned char c, int x0, int y0, int x1, int y1)
{
	uint8_t R, G, B;
	uint16_t color;
	R=table_rgb[c*3];
	G=table_rgb[c*3+1];
	B=table_rgb[c*3+2];
	color = RGB888_2_RGB565(R, G, B);
	ILI9341_OpenWindow ( x0, y0, (x1-x0+1), (y1-y0+1) );
	ILI9341_FillColor ( (x1-x0+1)*(y1-y0+1) ,color);	
	return;
}

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


