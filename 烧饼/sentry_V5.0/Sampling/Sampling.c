/**
  ******************************************************************************
  * Description : 这是一坨数据处理的代码
  *  
  ******************************************************************************
  * @attention	
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"


/* -------------------- 全局变量 -------------------- */

//电机滤波数据结构体
Motor_Filiter_Data_Struct Motor_Filiter_Data_t;


/* -------------------- 数据处理任务 -------------------- */

/**
  * @brief  数据处理任务入口
  * @param  None
  * @retval None
*/
void Sampling_thread_entry(void *parameter)
{
	while(1)
	{
		//执行周期计数
		#if(EXECUTION_COUNT_SWITCH==1)
			Execution_Count_t.Sampling_Execution_Count++;
		#endif
		
		//陀螺仪姿态解算更新
		IMU_Updata();
		
		//低通滤波器																			///对每一个电机的返回值进行低通滤波，筛除高频信号
		Motor_Filiter_Data_t.Wheel_Motor1_Speed = Low_Pass_Filter(&Wheel1_Low_Pass_t, BASE_MOTOR1_Data_t.Speed);//轮子速度环低通滤波
		Motor_Filiter_Data_t.Wheel_Motor2_Speed = Low_Pass_Filter(&Wheel2_Low_Pass_t, BASE_MOTOR2_Data_t.Speed);
		Motor_Filiter_Data_t.Wheel_Motor3_Speed = Low_Pass_Filter(&Wheel3_Low_Pass_t, BASE_MOTOR3_Data_t.Speed);
		Motor_Filiter_Data_t.Wheel_Motor4_Speed = Low_Pass_Filter(&Wheel4_Low_Pass_t, BASE_MOTOR4_Data_t.Speed);
		Motor_Filiter_Data_t.Yaw_Motor_Speed = Low_Pass_Filter(&Yaw_Speed_Low_Pass_t, YAW_MOTOR_Data_t.Speed);//Yaw轴Pitch轴速度环低通滤波
		Motor_Filiter_Data_t.Pitch_Motor_Speed = Low_Pass_Filter(&Pitch_Speed_Low_Pass_t, PITCH_MOTOR_Data_t.Speed);
		Motor_Filiter_Data_t.Yaw_Motor_Angle = Low_Pass_Filter(&Yaw_Angle_Low_Pass_t, YAW_MOTOR_Data_t.Angle);//Yaw轴Pitch轴角度环低通滤波
		Motor_Filiter_Data_t.Pitch_Motor_Angle = Low_Pass_Filter(&Pitch_Angle_Low_Pass_t, PITCH_MOTOR_Data_t.Angle);
		Motor_Filiter_Data_t.Shoot_Motor1_Speed = Low_Pass_Filter(&Friction1_Speed_Low_Pass_t, SHOOT_LEFT_MOTOR_Data_t.Speed);//拨弹盘速度环低通滤波
		Motor_Filiter_Data_t.Shoot_Motor2_Speed = Low_Pass_Filter(&Friction2_Speed_Low_Pass_t, SHOOT_RIGHT_MOTOR_Data_t.Speed);
		
		//执行频率
		rt_thread_mdelay(TASK_EXECUTION_CYCLE);
	}
}
