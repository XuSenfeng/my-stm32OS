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
		//¼ì²â´¥¿ØÆÁ°´Ñ¹ÊÂ¼þ
		XPT2046_TouchEvenHandler();
		
		
	}
	
}







