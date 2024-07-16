#ifndef __ROBOT_INIT_H
#define __ROBOT_INIT_H


//功能初始化结构体
typedef struct 
{
	//开始前向角校准标志位
	uint8_t Yaw_Angle_Out_Start_Flag;
	//前向角校准角度环结果
	float Angle_Result;
	//前向角校准速度环结果
	float Speed_Result;
	
}Robot_Init_Struct;


//外设初始化
void Peripheral_Init(void);
//功能初始化任务入口
void Fun_Init_thread_entry(void *parameter);

#endif
