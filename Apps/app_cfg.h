
#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT

// 任务TaskStart的栈及优先级分配
#define TASK_START_STK_SIZE 1024
#define TaskStartPrio 		10 //清分任务
// 任务Task1的栈及优先级分配
#define TASK1_STK_SIZE 		1024
#define Task1Prio 			11 //停止找零任务
// 任务Task2的栈及优先级分配
#define TASK2_STK_SIZE 		1024
#define Task2Prio 			8 //串口任务
// 任务Task4的栈及优先级分配
#define TASK4_STK_SIZE 		4096
#define Task4Prio 			9 //找零任务
// 任务Task3的栈及优先级分配
#define TASK3_STK_SIZE 		4096
#define Task3Prio 			12
// 定时器优先级配置
#define  OS_TASK_TMR_PRIO   (OS_LOWEST_PRIO - 2)

#endif

