#include "jiao_os.h"


int main()
{
	int i;
	JIAO_OS_Init();
#if Jiao_Debug

	printf("**********************************\n\n");
	test();
#endif
	Draw_Mouse(160, 120);
	while(1){
		//��ⴥ������ѹ�¼�
		XPT2046_TouchEvenHandler();
		
		
	}
	
}







