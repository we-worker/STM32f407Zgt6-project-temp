#include "schedule.h"
//轮询任务调度算法。

volatile uint64_t systime_ms = 0;


/********************************************************************************
* Routine: Loop_1000Hz
* Description: 
* Param: NULL
* Return: NULL
* Notes: 
**********************************************************************************/
static void Loop_1000Hz(void)
{
	
}

/********************************************************************************
* Routine: Loop_500Hz
* Description: 
* Param: NULL
* Return: NULL
* Notes: 
**********************************************************************************/
static void Loop_500Hz(void)
{
//	if(Mode_Select==Mode1){
//		Cricket.Task1();
//	}else	if(Mode_Select==Mode2){
//		Cricket.Task2();
//	}else	if(Mode_Select==Mode3){
//		Cricket.Task3();
//	}else	if(Mode_Select==Mode4){
//		Cricket.Task4();
//	}else	if(Mode_Select==Mode5){
//		Cricket.Task5();
//	}else	if(Mode_Select==Mode6){
//		Cricket.Task6();
//	}
}

/********************************************************************************
* Routine: Loop_333Hz
* Description: 
* Param: NULL
* Return: NULL
* Notes: 
**********************************************************************************/
static void Loop_333Hz(void)
{

}


/********************************************************************************
* Routine: Loop_200Hz
* Description: 
* Param: NULL
* Return: NULL
* Notes: 
**********************************************************************************/
static void Loop_200Hz(void)
{
	
}

/********************************************************************************
* Routine: Loop_100Hz
* Description: 
* Param: NULL
* Return: NULL
* Notes: 
**********************************************************************************/
static void Loop_100Hz(void)
{
//	enc1 = __HAL_TIM_GET_COUNTER(&htim3);
//	UIcount1=enc1%9;
//	UIcount2=enc1%20;
//	UIcount3=UIcount2-10;
//	htim3.Instance->CNT=0;
//	htim4.Instance->CNT=0;
}

/********************************************************************************
* Routine: Loop_50Hz
* Description: 
* Param: NULL
* Return: NULL
* Notes: 
**********************************************************************************/
static void Loop_50Hz(void)
{
	
}

/********************************************************************************
* Routine: Loop_20Hz
* Description: 
* Param: NULL
* Return: NULL
* Notes: 
**********************************************************************************/
static void Loop_20Hz(void)
{
	
}

/********************************************************************************
* Routine: Loop_5Hz
* Description: 
* Param: NULL
* Return: NULL
* Notes: 
**********************************************************************************/
static void Loop_5Hz(void)
{
	
}

/********************************************************************************
* Routine: Loop_2Hz
* Description: 
* Param: NULL
* Return: NULL
* Notes: 
**********************************************************************************/
static void Loop_2Hz(void)
{
	
}

/********************************************************************************
* Routine: Loop_1Hz
* Description: 
* Param: NULL
* Return: NULL
* Notes: 
**********************************************************************************/
static void Loop_1Hz(void)
{
	static uint8_t i=0;
	if(i==0){
		Led.Light(1,-1,-1);
		i = 1;
	}else{
		Led.Light(0,-1,-1);
		i = 0;
	}
}

//系统任务配置，创建不同执行频率的“线程”
static sched_task_t sched_tasks[] = 
{
	{Loop_1000Hz,1000,  0, 0, 0},
	{Loop_500Hz , 500,  0, 0, 0},
	{Loop_333Hz , 333,  0, 0, 0},
	{Loop_200Hz , 200,  0, 0, 0},
	{Loop_100Hz , 100,  0, 0, 0},
	{Loop_50Hz  ,  50,  0, 0, 0},
	{Loop_20Hz  ,  20,  0, 0, 0},
	{Loop_5Hz   ,   5,  0, 0, 0},
	{Loop_2Hz   ,   2,  0, 0, 0},
	{Loop_1Hz   ,   1,  0, 0, 0},
};
//根据数组长度，判断线程数量
#define TASK_NUM (sizeof(sched_tasks)/sizeof(sched_task_t))

/********************************************************************************
* Routine: Scheduler_Init
* Description: 
* Param: void
* Return: void
* Notes: 
**********************************************************************************/
void Scheduler_Init(void)
{
	uint8_t index = 0;
	//初始化任务表
	for(index=0;index < TASK_NUM;index++)
	{
		//计算每个任务的延时周期数
		sched_tasks[index].interval_ticks = (uint16_t)(1000.0f/sched_tasks[index].rate_hz);
		//最短周期为1
		if(sched_tasks[index].interval_ticks < 1)
		{
			sched_tasks[index].interval_ticks = 1;
		}
	}
}

/********************************************************************************
* Routine: Scheduler_Run
* Description: 
* Param: NULL
* Return: NULL
* Notes: 
**********************************************************************************/
void Scheduler_Run(void)
{
	systime_ms++;
	uint8_t index = 0;
	//循环判断所有线程，是否应该执行
	
	for(index=0;index < TASK_NUM;index++)
	{
		//进行判断，如果当前时间减去上一次执行的时间，大于等于该线程的执行周期，则执行线程
		if(systime_ms - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
		{
			//更新线程的执行时间，用于下一次判断
			sched_tasks[index].last_run = systime_ms;
			//执行线程函数，使用的是函数指针
			sched_tasks[index].task_func();
		}
	}
}

/********************************************************************************
* Routine: GetSchedulCunt
* Description: 
* Param: void
* Return: uint64_t
* Notes: 
**********************************************************************************/
uint64_t GetSchedulCunt(void)
{
	return systime_ms;
}




