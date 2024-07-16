#ifndef __GIMBAL_CONTROL_H
#define __GIMBAL_CONTROL_H


//云台控制结构体
typedef struct 
{
	//云台Yaw轴朝向角(IMU角度)
	float GimBla_Yaw_Angle;
	//底盘Z轴速度前馈补偿
	float Yaw_FeedForward_For_Zspeed;
	//Yaw轴角度环计算结果
	float Yaw_Angle_Result;
	//云台Pitch轴朝向角(IMU角度)
	float GimBla_Pitch_Angle;
	//Pitch轴角度环计算结果
	float Pitch_Angle_Result;
	//巡检模式Pitch轴方向
	uint8_t Patrol_Mode_Pitch_Direction;
	//摩擦轮开启标志位
	uint8_t Friction_Start_Flag;
	
	
}Gimbal_Control_Struct;

//遥控器云台任务入口
void Gimbal_thread_entry(void *parameter);

#endif
