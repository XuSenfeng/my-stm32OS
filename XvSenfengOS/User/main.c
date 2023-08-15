#include "jiao_os.h"


int main()
{
//	int i;
	JIAO_OS_Init();


#if Jiao_Debug

	printf("**********************************\n\n");
	test();
#endif
	Draw_Mouse(160, 120);
	while(1){
		//检测触控屏按压事件
		XPT2046_TouchEvenHandler();
		__WFI();	//WFI指令进入睡眠

		
	}
	
}







