#ifndef __SAMPLING_H
#define __SAMPLING_H

//电机滤波数据结构体
typedef struct
{
	//底盘3508速度滤波结果
	int16_t Wheel_Motor1_Speed;
	int16_t Wheel_Motor2_Speed;
	int16_t Wheel_Motor3_Speed;
	int16_t Wheel_Motor4_Speed;
	
	//Yaw轴Pitch轴6020速度滤波结果
	int16_t Yaw_Motor_Speed;
	int16_t Pitch_Motor_Speed;
	
	//Yaw轴Pitch轴6020角度滤波结果
	int32_t Yaw_Motor_Angle;
	int16_t Pitch_Motor_Angle;
	
	//拨弹盘2006速度滤波结果
	int16_t Shoot_Motor1_Speed;
	int16_t Shoot_Motor2_Speed;
	
}Motor_Filiter_Data_Struct;


//数据处理任务入口
void Sampling_thread_entry(void *parameter);

#endif
