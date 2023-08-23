#include "jiao_os.h"


int main()
{
//	int i;
	JIAO_OS_Init();

#if Jiao_Debug

	printf("**********************************\n\n");
	//test();
	Task_main();
#endif
#if USE_OF_MOUSE
	Draw_Mouse(160, 120);
#endif
	while(1){
		//检测触控屏按压事件
		XPT2046_TouchEvenHandler();
		Key_TouchEventHandler();
		Time_OutEventHandler();
		__WFI();	//WFI指令进入睡眠

		
	}
	
}







