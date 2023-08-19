/****************************************
*这个文件用于进行系统的相关操作以及初始化	*
*										*
*										*
*										*
*										*
*****************************************/

#ifndef __JIAO_OS_H__
#define __JIAO_OS_H__

#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_xpt2046_lcd.h"
#include "jiao_dasktop.h"
#include "bsp_base_tim.h"
#include "jiao_sheet.h"
void JIAO_OS_Init(void);

#endif
