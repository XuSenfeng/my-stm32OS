#include "jiao_os.h"
extern Mouse_Message_Def Mouse_def;
/**
  * @brief  ��ʼ��Ӳ�����л�������
  * @param  ��
  * @retval None
  */
static void Hareware_Init(void)
{
	//��ʼ������
	USART_Config();
	LED_GPIO_Config();
	printf("���\n");
	//��ʼ����Ļ
	ILI9341_Init();
	printf("��ʼ����Ļ,����������\n");
	ILI9341_Clear(0, 0, ILI9341_LESS_PIXEL, ILI9341_MORE_PIXEL);
	Draw_Dasktop();
	//��FLASH���ȡУ����������FLASH�޲�������ʹ��ģʽ3����У��
	Calibrate_or_Get_TouchParaWithFlash(3,0);
	XPT2046_Init();
	printf("��ʼ��������\n");
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
}

void JIAO_OS_Init(void)
{
	System_data_Init();
	Hareware_Init();

}






