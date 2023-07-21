#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_xpt2046_lcd.h"
void Hareware_Init(void);


int main()
{
	int i;
	Hareware_Init();
	while(1){
	XPT2046_TouchEvenHandler();
		
		for(i=0;i<10000;i++);
		
	}
	
}

void Hareware_Init(void)
{
	//初始化串口
	USART_Config();
	LED_GPIO_Config();
	printf("你好\n");
	//初始化屏幕
	ILI9341_Init();
	printf("初始化屏幕\n");
	ILI9341_Clear(0, 0, ILI9341_LESS_PIXEL, ILI9341_MORE_PIXEL);
	
	//从FLASH里获取校正参数，若FLASH无参数，则使用模式3进行校正
	Calibrate_or_Get_TouchParaWithFlash(3,0);
	XPT2046_Init();
	printf("初始化触摸屏\n");

}





