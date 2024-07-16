#ifndef __PID_H
#define __PID_H

//PID结构体
typedef struct 
{
	float Bias[3];	//当前误差，上次误差，上上次误差
	
	float Pout;		//P输出
	float Iout;		//I输出
	float Dout;		//D输出

	float Max_Iout;	//积分限幅
	float Min_Iout;
	float Max_out;	//输出限幅
	float Min_out;
	
	float Out_Put;	//输出

	float Kp;		//PID参数
	float Ki;
	float Kd;
	
	//不完全微分低通滤波器
	LowPass_Struct HSPID_Struct;
	
	//死区
	float Blind_Spot;
	
}PID_Struct;


//PID清零
void PID_clear(PID_Struct *pid);
//PID初始化
void PID_Init(PID_Struct *pid,float kp,float ki,float kd,float Max_out,float Max_Iout,float HSPID_kl,float blind_spot);
//位置式PID
float Pos_PID (PID_Struct *pid,float Target,float Now);
//增量式PID
float Inc_PID(PID_Struct *pid,float Target,float Now);
//上电初始化时赋值
void PID_All_Init(void);

#endif
