/**
  ******************************************************************************
  * Description : ����һ����̿����м��Ĵ���
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- ȫ�ֱ��� -------------------- */

//�����˶������ṹ��
Kinematic_Solving_Struct Kinematic_Solving_t;



/* -------------------- ���ƶ��� -------------------- */

/**
  * @brief  �����ٶȻ�����
  * @param  �ĸ����ӵ�Ŀ���ٶ�
  * @retval None
*/
void Wheel_Speed_Action(int16_t Target1,int16_t Target2,int16_t Target3,int16_t Target4)
{
	//����PID�������
	Motor_Current_t.Base_Motor1_Driver_Current = Pos_PID(&Wheel1_Speed_t,Target1,Motor_Filiter_Data_t.Wheel_Motor1_Speed);
	Motor_Current_t.Base_Motor2_Driver_Current = Pos_PID(&Wheel2_Speed_t,Target2,Motor_Filiter_Data_t.Wheel_Motor2_Speed);
	Motor_Current_t.Base_Motor3_Driver_Current = Pos_PID(&Wheel3_Speed_t,Target3,Motor_Filiter_Data_t.Wheel_Motor3_Speed);
	Motor_Current_t.Base_Motor4_Driver_Current = Pos_PID(&Wheel4_Speed_t,Target4,Motor_Filiter_Data_t.Wheel_Motor4_Speed);
}


/**
  * @brief  ң����ģʽС��������
  * @param  None
  * @retval None
*/
void Remote_Gyro_Start(void)
{
	//ң��������ģʽ	
	if(Base_Control_t.Remote_Gyro_Target_Speed < RC_GYRO_MAX_SPEED)
	{
		//ƽ������                                            30					2
		Base_Control_t.Remote_Gyro_Target_Speed += RC_GYRO_START_STEP_SIZE * TASK_EXECUTION_CYCLE;		///���������ٶ�
		if(Base_Control_t.Remote_Gyro_Target_Speed > RC_GYRO_MAX_SPEED)									///��ֹ�ٶ����
		{
			Base_Control_t.Remote_Gyro_Target_Speed = RC_GYRO_MAX_SPEED;
		}
	}	
}


/**
  * @brief  ң����ģʽС����ֹͣ
  * @param  None
  * @retval None
*/
void Remote_Gyro_Stop(void)
{
	//ƽ��ֹͣ
	if(Base_Control_t.Remote_Gyro_Target_Speed > RC_GYRO_STOP_MIN_SPEED)
	{
		Base_Control_t.Remote_Gyro_Target_Speed -= RC_GYRO_STOP_STEP_SIZE*TASK_EXECUTION_CYCLE;
	}
	
	//����ǶȲ�
	float Angle_Difference = (float)Base_Control_t.Original_Forward_Direction/8191*360-(float)Motor_Filiter_Data_t.Yaw_Motor_Angle/8191*360;
	if(Angle_Difference>180){Angle_Difference=360-Angle_Difference;}
	if(Angle_Difference<-180){Angle_Difference=360+Angle_Difference;}
	if(Angle_Difference<0){Angle_Difference=-Angle_Difference;}
	
	//ֹͣ��ת
	if(Base_Control_t.Remote_Gyro_Target_Speed <= RC_GYRO_STOP_MIN_SPEED && Angle_Difference<RC_GYRO_STOP_MIN_ANGLE)
	{
		Base_Control_t.Remote_Gyro_Target_Speed = 0;
		//С������ֹͣ�������̸���
		Base_Control_t.Gyro_Rotating_Flag = 0;
	}
}


/**
  * @brief  ���̸�����̨����
  * @param  Ŀ����̨�Ƕ�
  * @retval None
*/
void Base_Follow_Gimbal_Action(int32_t Target)
{
	//����ʵʱ������̨
	Kinematic_Solving_t.Z_Speed = Pos_PID(&Base_Follow_Gimbal_t,Target,Motor_Filiter_Data_t.Yaw_Motor_Angle);
}



/* -------------------- �˶����� -------------------- */

/**
  * @brief  �ٶ�ת��(xǰ���󸺣�y�����Ҹ�)->(yǰ���󸺣�x������)
  * @param  None
  * @retval None
*/
void Move_Speed_Transform(void)
{
	//ƽ�ƽǶȲ���
	Kinematic_Solving_t.Direction_Compensation = 
			(float)(Motor_Filiter_Data_t.Wheel_Motor1_Speed + Motor_Filiter_Data_t.Wheel_Motor2_Speed
				+ Motor_Filiter_Data_t.Wheel_Motor3_Speed + Motor_Filiter_Data_t.Wheel_Motor4_Speed)/4/GYRO_DIRECTION_COMPENSATION;
	
	//������̨��ԽǶ�
	Kinematic_Solving_t.Yaw_Df_Ag = (float)Base_Control_t.Original_Forward_Direction/8191*360
				- (float)Motor_Filiter_Data_t.Yaw_Motor_Angle/8191*360 - Kinematic_Solving_t.Direction_Compensation;		///8191ΪС������תһ�ܵ�����ֵ
	if(Kinematic_Solving_t.Yaw_Df_Ag<0)		{Kinematic_Solving_t.Yaw_Df_Ag = 360+Kinematic_Solving_t.Yaw_Df_Ag;}
	
	//�Ƕ�ת����
	Kinematic_Solving_t.Yaw_Df_Ag_pi=Kinematic_Solving_t.Yaw_Df_Ag/360*2*PI;
	
	//��¼С���ݷ�תģʽ
	///if(rc_data.rc.ch1>400) Kinematic_Solving_t.Z_Speed = -2000;
	
	//����ת��
	Kinematic_Solving_t.Gimbal_X_Speed = Kinematic_Solving_t.Base_X_Speed*cos(Kinematic_Solving_t.Yaw_Df_Ag_pi)
											-Kinematic_Solving_t.Base_Y_Speed*sin(Kinematic_Solving_t.Yaw_Df_Ag_pi);
	Kinematic_Solving_t.Gimbal_Y_Speed = Kinematic_Solving_t.Base_X_Speed*sin(Kinematic_Solving_t.Yaw_Df_Ag_pi)
											+Kinematic_Solving_t.Base_Y_Speed*cos(Kinematic_Solving_t.Yaw_Df_Ag_pi);
	
	//�ٶ�ת��
	#if (CHASSIS_TYPE==0) //ȫ����
		Kinematic_Solving_t.Target1 = Kinematic_Solving_t.Gimbal_X_Speed - Kinematic_Solving_t.Gimbal_Y_Speed + Kinematic_Solving_t.Z_Speed;
		Kinematic_Solving_t.Target2 = Kinematic_Solving_t.Gimbal_X_Speed + Kinematic_Solving_t.Gimbal_Y_Speed + Kinematic_Solving_t.Z_Speed;
		Kinematic_Solving_t.Target3 =-Kinematic_Solving_t.Gimbal_X_Speed + Kinematic_Solving_t.Gimbal_Y_Speed + Kinematic_Solving_t.Z_Speed;
		Kinematic_Solving_t.Target4 =-Kinematic_Solving_t.Gimbal_X_Speed - Kinematic_Solving_t.Gimbal_Y_Speed + Kinematic_Solving_t.Z_Speed;
	#else //����
		Kinematic_Solving_t.Target1 = Kinematic_Solving_t.Gimbal_X_Speed-Kinematic_Solving_t.Gimbal_Y_Speed+Kinematic_Solving_t.Z_Speed;
		Kinematic_Solving_t.Target2 = Kinematic_Solving_t.Gimbal_X_Speed+Kinematic_Solving_t.Gimbal_Y_Speed+Kinematic_Solving_t.Z_Speed;
		Kinematic_Solving_t.Target3 =-Kinematic_Solving_t.Gimbal_X_Speed+Kinematic_Solving_t.Gimbal_Y_Speed+Kinematic_Solving_t.Z_Speed;
		Kinematic_Solving_t.Target4 =-Kinematic_Solving_t.Gimbal_X_Speed-Kinematic_Solving_t.Gimbal_Y_Speed+Kinematic_Solving_t.Z_Speed;
	#endif
	
	//�˶�����
	Wheel_Speed_Action(Kinematic_Solving_t.Target1, Kinematic_Solving_t.Target2, Kinematic_Solving_t.Target3, Kinematic_Solving_t.Target4);
}
