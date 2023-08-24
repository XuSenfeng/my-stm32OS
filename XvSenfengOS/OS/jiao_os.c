#include "jiao_os.h"
extern Mouse_Message_Def Mouse_def;

struct Event_Flog EventFlog;

/**
  * @brief  ��ʼ��Ӳ�����л�������
  * @param  ��
  * @retval None
  */
static void Hareware_Init(void)
{
	//��ʼ����ʱ��
	TOUCH_TIM_Init();
	//��ʼ������

	USART_Config();
	LED_GPIO_Config();
	printf("���\n");
	//��ʼ����Ļ
	ILI9341_Init();
	XPT2046_Init();
	printf("��ʼ��������, ͬʱ��ʼ��SPIFlash\n");
	//��FLASH���ȡУ����������FLASH�޲�������ʹ��ģʽ3����У��
	Calibrate_or_Get_TouchParaWithFlash(3,0);
	printf("��ʼ����Ļ,����������\n");
	//�����������Ļ
	//ILI9341_Clear(0, 0, ILI9341_LESS_PIXEL, ILI9341_MORE_PIXEL);
	//��ʼ������
	Key_GPIO_Config();
	//��������
	Draw_Dasktop();

}

/**
  * @brief  ��ʼ��ʹ�õ��Ĳ���
  * @param  ��
  * @retval None
  */
static void System_data_Init(void)
{
	//��ʼ����������
	Mouse_def.Width = MOUSE_WIDTH;
	Mouse_def.High = MOUSE_HIGH;
	init_mouse_cursor8(Mouse_def.mouse);
	//��ʼ��ͼ��
	sheet_init();
	//make_window8(uint16_t *buf, int xsize, int ysize, char *title)

	//��־λ������
	FIFO8_Init(&EventFlog.System_Flags, 16, EventFlog.system_flags_buf);
	//����ʱ�������
	Timer_init();
}


void JIAO_OS_Init(void)
{

	Hareware_Init();
	System_data_Init();
}






