/**
  ******************************************************************************
  * Description : 这是一坨云台控制中间层的代码
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- 全局变量 -------------------- */




/* -------------------- 控制动作 -------------------- */

/**
  * @brief  云台Yaw轴动作
  * @param  目标角度(陀螺仪Yaw轴角度)
  * @retval None
*/
void Gimbal_Yaw_Action(float Target)
{
	//底盘Z轴速度前馈补偿
	Gimbal_Control_t.Yaw_FeedForward_For_Zspeed = (float)(Motor_Filiter_Data_t.Wheel_Motor1_Speed+Motor_Filiter_Data_t.Wheel_Motor2_Speed
									+Motor_Filiter_Data_t.Wheel_Motor3_Speed+Motor_Filiter_Data_t.Wheel_Motor4_Speed)/BASE_Z_SPEED_FEEDFORWARD;
	
	//角度环PID
	Gimbal_Control_t.Yaw_Angle_Result = Pos_PID(&Yaw_IMU_Angle_t,Target,INS.YawTotalAngle);
	
	//速度环PID
	#if (YAW_DEBUGGING_MODE==0)
		Motor_Current_t.Yaw_Motor_Driver_Current = 
			Pos_PID(&Yaw_Motor_Speed_t,Gimbal_Control_t.Yaw_Angle_Result+Gimbal_Control_t.Yaw_FeedForward_For_Zspeed,Motor_Filiter_Data_t.Yaw_Motor_Speed);
	#else
		Motor_Current_t.Yaw_Motor_Driver_Current = 
			Pos_PID(&Yaw_Motor_Speed_t,Gimbal_Control_t.Yaw_Angle_Result,Motor_Filiter_Data_t.Yaw_Motor_Speed);
	#endif
}


/**
  * @brief  云台Pitch轴动作
  * @param  目标角度(6020电机角度)
  * @retval None
*/
void Gimbal_Pitch_Action(float Target)
{
	//角度环PID
	Gimbal_Control_t.Pitch_Angle_Result = Pos_PID(&Pitch_Motor_Angle_t, Target, Motor_Filiter_Data_t.Pitch_Motor_Angle);
	//速度环PID
	///Motor_Current_t.Pitch_Motor_Driver_Current = Pos_PID(&Pitch_Motor_Speed_t, Gimbal_Control_t.Pitch_Angle_Result, Motor_Filiter_Data_t.Pitch_Motor_Speed);
	Gimbal_Control_t.Pitch_Angle_Result += Pos_PID(&Pitch_Motor_Speed_t, Gimbal_Control_t.Pitch_Angle_Result, 10*Motor_Filiter_Data_t.Pitch_Motor_Speed);
	//Motor_Current_t.Pitch_Motor_Driver_Current = Pos_PID(&Pitch_Motor_Speed_t,Gimbal_Control_t.Pitch_Angle_Result,PITCH_MOTOR_Data_t.Speed);
}


/**
  * @brief  拨弹速度环动作
  * @param  目标速度(-18000 -- 18000)
  * @retval None
*/
void Shoot_Speed_Action(float Target1,float Target2)
{
	//速度环PID
	Motor_Current_t.Shoot_Motor1_Driver_Current = Pos_PID(&Shoot1_Speed_t, Target1, Motor_Filiter_Data_t.Shoot_Motor1_Speed);
	Motor_Current_t.Shoot_Motor2_Driver_Current = Pos_PID(&Shoot2_Speed_t, Target2, Motor_Filiter_Data_t.Shoot_Motor2_Speed);
}
