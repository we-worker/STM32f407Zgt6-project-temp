#ifndef _AD9959_Outset_H
#define _AD9959_Outset_H

#define uchar unsigned char
#define uint unsigned int 

//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
// extern void AllChannl_init();
extern void AD9959_Setwavefrequency(double f);
extern void AD9959_enablechanne00(void);
extern void AD9959_enablechannel0(void);
extern void AD9959_enablechannel1(void);
extern void AD9959_enablechannel2(void);
extern void AD9959_enablechannel3(void);
extern void AD9959_Setwavephase(double f,int p);
extern void AD9959_Setwaveamplitute(double f,int a);
extern void AD9959_SetFremodulation2(double f1,double f2);
extern void AD9959_SetPhamodulation2(double f,int p1,int p2);
extern void AD9959_SetAM2(double f);
extern void AD9959_SetFremodulation4(double f1,double f2,double f3,double f4);
extern void AD9959_SetPhamodulation4(double f,int p1,int p2,int p3,int p4);
extern void AD9959_SetAM4(double f);
extern void AD9959_SetFremodulation42(double f1,double f2,double f3,double f4);
extern void AD9959_SetPhamodulation42(double f,int p1,int p2,int p3,int p4);
extern void AD9959_SetAM42(double f);
extern void AD9959_SetFremodulation80(double f1,double f2,double f3,double f4,double f5,double f6,double f7,double f8);
extern void AD9959_SetPhamodulation80(double f,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8);
extern void AD9959_SetAM80(double f);
extern void AD9959_SetFremodulation81(double f1,double f2,double f3,double f4,double f5,double f6,double f7,double f8);
extern void AD9959_SetPhamodulation81(double f,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8);
extern void AD9959_SetAM81(double f);
extern void AD9959_SetFremodulation82(double f1,double f2,double f3,double f4,double f5,double f6,double f7,double f8);
extern void AD9959_SetPhamodulation82(double f,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8);
extern void AD9959_SetAM82(double f);
extern void AD9959_SetFremodulation83(double f1,double f2,double f3,double f4,double f5,double f6,double f7,double f8);
extern void AD9959_SetPhamodulation83(double f,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8);
extern void AD9959_SetAM83(double f);
extern void AD9959_SetFremodulation160(double f1,double f2,double f3,double f4,double f5,double f6,double f7,double f8,double f9,double f10,double f11,double f12,double f13,double f14,double f15,double f16);
extern void AD9959_SetPhamodulation160(double f,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8,int p9,int p10,int p11,int p12,int p13,int p14,int p15,int p16);
extern void AD9959_SetAM160(double f);
extern void AD9959_SetFremodulation161(double f1,double f2,double f3,double f4,double f5,double f6,double f7,double f8,double f9,double f10,double f11,double f12,double f13,double f14,double f15,double f16);
extern void AD9959_SetPhamodulation161(double f,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8,int p9,int p10,int p11,int p12,int p13,int p14,int p15,int p16);
extern void AD9959_SetAM161(double f);
extern void AD9959_SetFremodulation162(double f1,double f2,double f3,double f4,double f5,double f6,double f7,double f8,double f9,double f10,double f11,double f12,double f13,double f14,double f15,double f16);
extern void AD9959_SetPhamodulation162(double f,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8,int p9,int p10,int p11,int p12,int p13,int p14,int p15,int p16);
extern void AD9959_SetAM162(double f);
extern void AD9959_SetFremodulation163(double f1,double f2,double f3,double f4,double f5,double f6,double f7,double f8,double f9,double f10,double f11,double f12,double f13,double f14,double f15,double f16);
extern void AD9959_SetPhamodulation163(double f,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8,int p9,int p10,int p11,int p12,int p13,int p14,int p15,int p16);
extern void AD9959_SetAM163(double f);
extern void AD9959_Frequency_Sweep(double f1,double f2,double a1,double a2);
extern void AD9959_Phase_Sweep(int p1,int p2,int a1,int a2,double f);	
extern void AD9959_Amplitute_Sweep(int a,int a1,int a2,double f);
#endif

