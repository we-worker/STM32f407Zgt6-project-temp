#ifndef __PID_H__
#define __PID_H__
#include "common.h"


typedef struct PID
{
	float kp;
	float ki;
	float kd;

	float error_acc;
	float lastError;

} PID;

int Follow_PID(PID *s_PID, float dac_out);

#endif


