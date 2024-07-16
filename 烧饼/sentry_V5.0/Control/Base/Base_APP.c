/**
  ******************************************************************************
  * Description : 这是一坨底盘控制中间层的代码
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- 全局变量 -------------------- */

//底盘运动解析结构体
Kinematic_Solving_Struct Kinematic_Solving_t;



/* -------------------- 控制动作 -------------------- */

/**
  * @brief  轮子速度环动作
  * @param  四个轮子的目标速度
  * @retval None
*/
void Wheel_Speed_Action(int16_t Target1,int16_t Target2,int16_t Target3,int16_t Target4)
{
	//轮子PID计算输出
	Motor_Current_t.Base_Motor1_Driver_Current = Pos_PID(&Wheel1_Speed_t,Target1,Motor_Filiter_Data_t.Wheel_Motor1_Speed);
	Motor_Current_t.Base_Motor2_Driver_Current = Pos_PID(&Wheel2_Speed_t,Target2,Motor_Filiter_Data_t.Wheel_Motor2_Speed);
	Motor_Current_t.Base_Motor3_Driver_Current = Pos_PID(&Wheel3_Speed_t,Target3,Motor_Filiter_Data_t.Wheel_Motor3_Speed);
	Motor_Current_t.Base_Motor4_Driver_Current = Pos_PID(&Wheel4_Speed_t,Target4,Motor_Filiter_Data_t.Wheel_Motor4_Speed);
}


/**
  * @brief  遥控器模式小陀螺启动
  * @param  None
  * @retval None
*/
void Remote_Gyro_Start(void)
{
	//遥控器控制模式	
	if(Base_Control_t.Remote_Gyro_Target_Speed < RC_GYRO_MAX_SPEED)
	{
		//平滑启动                                            30					2
		Base_Control_t.Remote_Gyro_Target_Speed += RC_GYRO_START_STEP_SIZE * TASK_EXECUTION_CYCLE;		///步进增大速度
		if(Base_Control_t.Remote_Gyro_Target_Speed > RC_GYRO_MAX_SPEED)									///防止速度溢出
		{
			Base_Control_t.Remote_Gyro_Target_Speed = RC_GYRO_MAX_SPEED;
		}
	}	
}


/**
  * @brief  遥控器模式小陀螺停止
  * @param  None
  * @retval None
*/
void Remote_Gyro_Stop(void)
{
	//平滑停止
	if(Base_Control_t.Remote_Gyro_Target_Speed > RC_GYRO_STOP_MIN_SPEED)
	{
		Base_Control_t.Remote_Gyro_Target_Speed -= RC_GYRO_STOP_STEP_SIZE*TASK_EXECUTION_CYCLE;
	}
	
	//计算角度差
	float Angle_Difference = (float)Base_Control_t.Original_Forward_Direction/8191*360-(float)Motor_Filiter_Data_t.Yaw_Motor_Angle/8191*360;
	if(Angle_Difference>180){Angle_Difference=360-Angle_Difference;}
	if(Angle_Difference<-180){Angle_Difference=360+Angle_Difference;}
	if(Angle_Difference<0){Angle_Difference=-Angle_Difference;}
	
	//停止旋转
	if(Base_Control_t.Remote_Gyro_Target_Speed <= RC_GYRO_STOP_MIN_SPEED && Angle_Difference<RC_GYRO_STOP_MIN_ANGLE)
	{
		Base_Control_t.Remote_Gyro_Target_Speed = 0;
		//小陀螺已停止开启底盘跟随
		Base_Control_t.Gyro_Rotating_Flag = 0;
	}
}


/**
  * @brief  底盘跟随云台动作
  * @param  目标云台角度
  * @retval None
*/
void Base_Follow_Gimbal_Action(int32_t Target)
{
	//底盘实时跟随云台
	Kinematic_Solving_t.Z_Speed = Pos_PID(&Base_Follow_Gimbal_t,Target,Motor_Filiter_Data_t.Yaw_Motor_Angle);
}



/* -------------------- 运动解析 -------------------- */

/**
  * @brief  速度转换(x前正后负，y左正右负)->(y前正后负，x左负右正)
  * @param  None
  * @retval None
*/
void Move_Speed_Transform(void)
{
	//平移角度补偿
	Kinematic_Solving_t.Direction_Compensation = 
			(float)(Motor_Filiter_Data_t.Wheel_Motor1_Speed + Motor_Filiter_Data_t.Wheel_Motor2_Speed
				+ Motor_Filiter_Data_t.Wheel_Motor3_Speed + Motor_Filiter_Data_t.Wheel_Motor4_Speed)/4/GYRO_DIRECTION_COMPENSATION;
	
	//底盘云台相对角度
	Kinematic_Solving_t.Yaw_Df_Ag = (float)Base_Control_t.Original_Forward_Direction/8191*360
				- (float)Motor_Filiter_Data_t.Yaw_Motor_Angle/8191*360 - Kinematic_Solving_t.Direction_Compensation;		///8191为小陀螺旋转一周的数据值
	if(Kinematic_Solving_t.Yaw_Df_Ag<0)		{Kinematic_Solving_t.Yaw_Df_Ag = 360+Kinematic_Solving_t.Yaw_Df_Ag;}
	
	//角度转弧度
	Kinematic_Solving_t.Yaw_Df_Ag_pi=Kinematic_Solving_t.Yaw_Df_Ag/360*2*PI;
	
	//检录小陀螺反转模式
	///if(rc_data.rc.ch1>400) Kinematic_Solving_t.Z_Speed = -2000;
	
	//坐标转换
	Kinematic_Solving_t.Gimbal_X_Speed = Kinematic_Solving_t.Base_X_Speed*cos(Kinematic_Solving_t.Yaw_Df_Ag_pi)
											-Kinematic_Solving_t.Base_Y_Speed*sin(Kinematic_Solving_t.Yaw_Df_Ag_pi);
	Kinematic_Solving_t.Gimbal_Y_Speed = Kinematic_Solving_t.Base_X_Speed*sin(Kinematic_Solving_t.Yaw_Df_Ag_pi)
											+Kinematic_Solving_t.Base_Y_Speed*cos(Kinematic_Solving_t.Yaw_Df_Ag_pi);
	
	//速度转换
	#if (CHASSIS_TYPE==0) //全向轮
		Kinematic_Solving_t.Target1 = Kinematic_Solving_t.Gimbal_X_Speed - Kinematic_Solving_t.Gimbal_Y_Speed + Kinematic_Solving_t.Z_Speed;
		Kinematic_Solving_t.Target2 = Kinematic_Solving_t.Gimbal_X_Speed + Kinematic_Solving_t.Gimbal_Y_Speed + Kinematic_Solving_t.Z_Speed;
		Kinematic_Solving_t.Target3 =-Kinematic_Solving_t.Gimbal_X_Speed + Kinematic_Solving_t.Gimbal_Y_Speed + Kinematic_Solving_t.Z_Speed;
		Kinematic_Solving_t.Target4 =-Kinematic_Solving_t.Gimbal_X_Speed - Kinematic_Solving_t.Gimbal_Y_Speed + Kinematic_Solving_t.Z_Speed;
	#else //麦轮
		Kinematic_Solving_t.Target1 = Kinematic_Solving_t.Gimbal_X_Speed-Kinematic_Solving_t.Gimbal_Y_Speed+Kinematic_Solving_t.Z_Speed;
		Kinematic_Solving_t.Target2 = Kinematic_Solving_t.Gimbal_X_Speed+Kinematic_Solving_t.Gimbal_Y_Speed+Kinematic_Solving_t.Z_Speed;
		Kinematic_Solving_t.Target3 =-Kinematic_Solving_t.Gimbal_X_Speed+Kinematic_Solving_t.Gimbal_Y_Speed+Kinematic_Solving_t.Z_Speed;
		Kinematic_Solving_t.Target4 =-Kinematic_Solving_t.Gimbal_X_Speed-Kinematic_Solving_t.Gimbal_Y_Speed+Kinematic_Solving_t.Z_Speed;
	#endif
	
	//运动解算
	Wheel_Speed_Action(Kinematic_Solving_t.Target1, Kinematic_Solving_t.Target2, Kinematic_Solving_t.Target3, Kinematic_Solving_t.Target4);
}
