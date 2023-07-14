#include "pid.h"
#include "dac.h"
#include "adc.h"
#include "lcd.h"

extern u16 dac1val;
extern u8 dac1StopFlag;
extern float dac1_Vref;

//pid的参数初始化
PID dac_pid={
	.kp=100,
	.ki=1,
	.kd=50,
	.error_acc=0,
	.lastError=0

};


//电压调整pid
int Follow_PID(PID *s_PID,float adc_data1,float dac_out)
{
	float iError=0;
	int output = 0;
	iError = adc_data1-dac_out; //误差值计算
	s_PID->error_acc += iError;		   //积分
	output = s_PID->kp * iError + s_PID->ki * s_PID->error_acc * 0.5f + s_PID->kd * iError - s_PID->lastError;

	s_PID->lastError = iError;	// error值存储

	return (output);
}

void Auto_Justment(void){
	
	//dac1val=dac1val+Follow_PID(&dac_pid);
	if (dac1val > 4000)//限定最大输出电压
		dac1val=4000;
	
	DAC_SetChannel1Data(DAC_Align_12b_R, dac1val); //设置DAC值
	
	u16 dac_data = DAC_GetDataOutputValue(DAC_Channel_1); //读取前面设置DAC的值
	
	char display_str[30];
	float dac_out = (float)dac_data * (dac1_Vref / 4096);			//得到DAC电压值
	sprintf((char *)display_str, "DAC VOL:%.4f", dac_out); //浮点型数据  e-01  就是除于10      /10  //%06e
	LCD_DisplayString(10, 50, 24, display_str);			//实际电压数值

}



