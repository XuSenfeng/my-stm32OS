#include "jiao_os.h"
extern Mouse_Message_Def Mouse_def;
/**
  * @brief  初始化硬件进行绘制桌面
  * @param  无
  * @retval None
  */
static void Hareware_Init(void)
{
	//初始化串口
	USART_Config();
	LED_GPIO_Config();
	printf("你好\n");
	//初始化屏幕
	ILI9341_Init();
	printf("初始化屏幕,绘制桌面中\n");
	ILI9341_Clear(0, 0, ILI9341_LESS_PIXEL, ILI9341_MORE_PIXEL);
	Draw_Dasktop();
	//从FLASH里获取校正参数，若FLASH无参数，则使用模式3进行校正
	Calibrate_or_Get_TouchParaWithFlash(3,0);
	XPT2046_Init();
	printf("初始化触摸屏\n");
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
}

void JIAO_OS_Init(void)
{
	System_data_Init();
	Hareware_Init();

}






