#include "jiao_dasktop.h"

//保存原本的颜色
extern uint16_t Old_Color[20*40];
//初始化一个鼠标为全局变量
Mouse_Message_Def Mouse_def;
//这里是操作系统使用的颜色的RGB565格式
uint16_t table_rgb565[16] = {
	0x0000,	/*  0:黒 */
	0xf800,	/*  1:亮红 */
	0x07e0,	/*  2:绿 */
	0xffe0,	/*  3:黄 */
	0x001f,	/*  4:蓝 */
	0xf817,	/*  5:紫 */
	0x07ff,	/*  6:浅蓝*/
	0xffff,	/*  7:白 */
	0xc638,	/*  8:灰 */
	0x8000,	/*  9:暗红 */
	0x0420,	/* 10:暗緑 */
	0x8420,	/* 11:暗黄色 */
	0x0010,	/* 12:暗青 */
	0x8010,	/* 13:暗紫 */
	0x0430,	/* 14:暗蓝色 */
	0x8430	/* 15:暗灰色 */
};

/**
  * @brief  绘制一个长方形或者一条线
  * @param  设置颜的
  * @param  起始的位置x
  * @param  起始的位置y
  * @param  结束的位置x
  * @param  结束的位置y
  * @retval None
  */
void boxfill8(unsigned char c, int x0, int y0, int x1, int y1)
{
	
	ILI9341_OpenWindow ( x0, y0, (x1-x0+1), (y1-y0+1) );
	ILI9341_FillColor ( (x1-x0+1)*(y1-y0+1) ,table_rgb565[c]);

	return;
}

/**
  * @brief  进行桌面图像的保存
  * @param  起始的位置x
  * @param  起始的位置y
  * @param  结束的位置x
  * @param  结束的位置y
  * @retval None
  */
void flish_boxfill8(int x0, int y0, int x1, int y1)
{
	
	uint16_t Color_Data[ILI9341_MORE_PIXEL], high, width, i;
	high = y1-y0+1;
	width = x1-x0+1;
	for(i=0;i<high;i++)
	{
		//读取最初始时候的颜色
		ILI9341_Read_Datas(Color_Data, x0, y0+i, width, 1);
		//这里由于每一个颜色是两个位, 所以计算的时候需要乘以2
		SPI_FLASH_BufferWrite((uint8_t *)Color_Data, (DASKTOP_SHEET_ADDR + (x0 + (y0+i) * ILI9341_MORE_PIXEL)*2), width*2);
		printf("写入%d\n", i);
	}
	
	
	


	return;
}

/**
  * @brief  绘制桌面, 初始化鼠标
  * @param  无
  * @retval None
  */
void Draw_Dasktop(void)
{
	//桌面的长宽, 以及Flash里面是否有桌面的图层存在的标志
	int xsize, ysize;
	uint32_t i;
	uint8_t Dasktop_Flash_flag;
	xsize =ILI9341_MORE_PIXEL;
	ysize = ILI9341_LESS_PIXEL;
	//读取Flash里面图层是否保存的初始化信息
	SPI_FLASH_BufferRead(&Dasktop_Flash_flag,DASKTOP_SHEET_FLAG_ADDR,1);
	//ILI9341_GramScan(3);
	//初始化桌面
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
	
	
	if((Dasktop_Flash_flag==0x32) & (~RELOAD_FLASH_SHEET))
	{
		printf("已经初始化桌面的图层\n");
	}else
	{
		Dasktop_Flash_flag=0x32;
		printf("没有初始化桌面的图层, 即将进行初始化, result = %#x\n", Dasktop_Flash_flag);
		SPI_FLASH_SectorErase(DASKTOP_SHEET_ADDR-256);
		SPI_FLASH_BufferWrite(&Dasktop_Flash_flag,DASKTOP_SHEET_FLAG_ADDR,1);
		for(i=0;i<1024;i++)
		{	
			//对相应的位置进行清除
			SPI_FLASH_SectorErase(DASKTOP_SHEET_ADDR + i*SPI_FLASH_PageSize);
			printf("擦除%d\n", i);

		}
		flish_boxfill8(0, 0, ILI9341_MORE_PIXEL, ILI9341_LESS_PIXEL);
		
	}
}

/**
  * @brief  初始化鼠标的图形
  * @param  保存初始化内容的结构体
  * @param  绘制的时候背景的颜色
  * @retval None
  */
void init_mouse_cursor8(char *mouse)
/* 初始化鼠标结构体 */
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
				mouse[y * 16 + x] = COL8_008484;
			}
		}
	}
	return;
}



/**
  * @brief  绘制鼠标,同时更新鼠标位置结构体
  * @param  绘制的位置x
  * @param  绘制的位置y
  * @retval None
  */
void Draw_Mouse(uint16_t x, uint16_t y)
{
	if(y>(ILI9341_LESS_PIXEL-1))
		y=(ILI9341_LESS_PIXEL-1);
	if(x>(ILI9341_MORE_PIXEL-1))
		x=(ILI9341_MORE_PIXEL-1);
	Mouse_def.x_old = Mouse_def.x;
	Mouse_def.y_old = Mouse_def.y;
	Mouse_def.x = x;
	Mouse_def.y = y;
	putblock8_8(x, y, Mouse_def.Width, Mouse_def.High, Mouse_def.mouse);
}

#if Jiao_Debug
/**
  * @brief  测试函数可以修改这个宏进行设置是否进行测试
  * @param  无
  * @retval None
  */
void test(void)
{
	uint16_t data[20];
	uint16_t i, j;
	uint16_t Color_Data[ILI9341_MORE_PIXEL];
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
	//显示文字
	ILI9341_DispString_EN_CH(0, 220, "jhy焦浩洋");
	ILI9341_DisplayStringEx(0, 120, 20, 40, "jhy焦浩洋", 0);
	//测试,读取Flash里面的图形, 进行重新绘制, 检测写入是否成功
	
	for(i=0;i<0xffff;i++)
		for(j=0;j<0xff;j++);
	for(i=0;i<ILI9341_LESS_PIXEL;i++)
	{
		
		SPI_FLASH_BufferRead((uint8_t *)Color_Data, (DASKTOP_SHEET_ADDR + i*ILI9341_MORE_PIXEL*2), (ILI9341_MORE_PIXEL*2));
		
		ILI9341_OpenWindow ( 0, i, ILI9341_MORE_PIXEL, 1 );
		ILI9341_Write_Cmd ( CMD_SetPixel );	
		
		for ( j = 0; j < ILI9341_MORE_PIXEL; j ++ )
			ILI9341_Write_Data ( Color_Data[j] );
		printf("重新绘制%d\n", i);
	}
	
	
}
#endif
