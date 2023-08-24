#include "jiao_os.h"
extern Mouse_Message_Def Mouse_def;

struct Event_Flog EventFlog;

/**
  * @brief  初始化硬件进行绘制桌面
  * @param  无
  * @retval None
  */
static void Hareware_Init(void)
{
	//初始化定时器
	TOUCH_TIM_Init();
	//初始化串口

	USART_Config();
	LED_GPIO_Config();
	printf("你好\n");
	//初始化屏幕
	ILI9341_Init();
	XPT2046_Init();
	printf("初始化触摸屏, 同时初始化SPIFlash\n");
	//从FLASH里获取校正参数，若FLASH无参数，则使用模式3进行校正
	Calibrate_or_Get_TouchParaWithFlash(3,0);
	printf("初始化屏幕,绘制桌面中\n");
	//在这里清空屏幕
	//ILI9341_Clear(0, 0, ILI9341_LESS_PIXEL, ILI9341_MORE_PIXEL);
	//初始化按键
	Key_GPIO_Config();
	//绘制桌面
	Draw_Dasktop();

}

/**
  * @brief  初始化使用到的参数
  * @param  无
  * @retval None
  */
static void System_data_Init(void)
{
	//初始化鼠标的数据
	Mouse_def.Width = MOUSE_WIDTH;
	Mouse_def.High = MOUSE_HIGH;
	init_mouse_cursor8(Mouse_def.mouse);
	//初始化图层
	sheet_init();
	//make_window8(uint16_t *buf, int xsize, int ysize, char *title)

	//标志位的设置
	FIFO8_Init(&EventFlog.System_Flags, 16, EventFlog.system_flags_buf);
	//设置时间计数器
	Timer_init();
}


void JIAO_OS_Init(void)
{

	Hareware_Init();
	System_data_Init();
}






