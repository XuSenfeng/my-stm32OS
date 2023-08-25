#include "jiao_os.h"
extern struct Event_Flog EventFlog;


int main()
{
	int i;
	JIAO_OS_Init();

#if Jiao_Debug

	printf("**********************************\n\n");
	test();
	//Task_main();
#endif
#if USE_OF_MOUSE
	Draw_Mouse(160, 120);
#endif

	while(1){
		//检测触控屏按压事件
		if(FIFO8_Status(&EventFlog.System_Flags))
		{
			i = FIFO8_Get(&EventFlog.System_Flags);
			if(i==1 || i==2)
			{
				//处理的事件是触摸屏
				XPT2046_TouchEvenHandler(i);
			}else if(i>2 && i<7){
				//处理的事件是按键
				Key_TouchEventHandler(i);
			}else{
				//处理的事件是定时器
			Time_OutEventHandler(i);
			}
		}	

		__WFI();	//WFI指令进入睡眠

		
	}
	
}







