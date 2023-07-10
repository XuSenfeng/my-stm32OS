#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configUSE_16_BIT_TICKS		0
//任务名字长度
#define configMAX_TASK_NAME_LEN		            ( 16 )
//是否允许静态任务
#define configSUPPORT_STATIC_ALLOCATION           1
//最大任务数
#define configMAX_PRIORITIES		            ( 5 )

#define configKERNEL_INTERRUPT_PRIORITY 		255   /* 高四位有效，即等于0xff，或者是15 */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	191   /* 高四位有效，即等于0xb0，或者是11 */

#define xPortPendSVHandler   PendSV_Handler
#define xPortSysTickHandler  SysTick_Handler
#define vPortSVCHandler      SVC_Handler
#endif /* FREERTOS_CONFIG_H */
