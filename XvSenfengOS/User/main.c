#include "jiao_os.h"


int main()
{
//	int i;
	JIAO_OS_Init();


#if Jiao_Debug

	printf("**********************************\n\n");
	test();
#endif
#if USE_OF_MOUSE
	Draw_Mouse(160, 120);
#endif
	while(1){
		//��ⴥ������ѹ�¼�
		XPT2046_TouchEvenHandler();
		__WFI();	//WFIָ�����˯��

		
	}
	
}







