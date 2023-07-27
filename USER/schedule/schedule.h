#ifndef _SCHEDULE_H
#define _SCHEDULE_H


#include "main.h"

typedef struct
{
	void(*task_func)(void);
	float rate_hz;
	uint16_t interval_ticks;
	uint32_t last_run;
	uint32_t run_time;
}sched_task_t;

void Scheduler_Init(void);
void Scheduler_Run(void);
uint64_t GetSchedulCunt(void);


#endif
