/****************************************
*����ļ����ڽ���ϵͳ����ز����Լ���ʼ��	*
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
#include "./Key/bsp_key.h"
/*���ڼ�¼�����ж��в������¼�*/
struct Event_Flog{
	uint8_t Key1_num;
	uint8_t Key2_num;
	uint8_t Touch_num;

};



void JIAO_OS_Init(void);

#endif
