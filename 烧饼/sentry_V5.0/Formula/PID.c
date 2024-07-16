/**
  ******************************************************************************
  * Description : ����һ��PID��ʽ�Ĵ���
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- ȫ�ֱ��� -------------------- */

//�����ٶȻ��ṹ��
PID_Struct Wheel1_Speed_t;
PID_Struct Wheel2_Speed_t;
PID_Struct Wheel3_Speed_t;
PID_Struct Wheel4_Speed_t;
//���̸�����̨�ṹ��
PID_Struct Base_Follow_Gimbal_t;
//Yaw��Ƕ��ٶȻ��ṹ��(IMU���ԽǶ�)
PID_Struct Yaw_Motor_Speed_t;
PID_Struct Yaw_IMU_Angle_t;
//Pitch��Ƕ��ٶȻ��ṹ��(IMU���ԽǶ�)
PID_Struct Pitch_Motor_Speed_t;
PID_Struct Pitch_Motor_Angle_t;
//������PID�ṹ��
PID_Struct Shoot1_Speed_t;
PID_Struct Shoot2_Speed_t;



/* -------------------- PID��ʼ�������� -------------------- */

/**
  * @brief  PID��ʼ��
  * @param  PID�ṹ�������PID����������޷��������޷�������ȫ΢�֣�����
  * @retval None
*/
void PID_Init(PID_Struct *pid,float kp,float ki,float kd,float Max_out,float Max_Iout,float blind_spot,float HSPID_kl)
{
	//PID������ֵ
	pid->Kp = kp;
	pid->Ki = ki;
	pid->Kd = kd;
	
	//�޷���ֵ
	pid->Max_out = Max_out;
	pid->Min_out = -Max_out;
	pid->Max_Iout = Max_Iout;
	pid->Min_Iout = -Max_Iout;
	
	//����ȫ΢���˲�����ʼ��
	pid->HSPID_Struct.KL = HSPID_kl;
}

/**
  * @brief  PID����
  * @param  PID�ṹ�����
  * @retval None
*/
void PID_clear(PID_Struct *pid)
{
	//��������
	pid->Bias[0] = pid->Bias[1] = pid->Bias[2] = 0;
	pid->Iout = pid->Out_Put = 0;
}


/* -------------------- PID��ʽ -------------------- */

/**
  * @brief  λ��ʽPID
  * @param  PID�ṹ�������Ŀ��ֵ����ǰֵ
  * @retval PID���
*/
float Pos_PID (PID_Struct *pid,float Target,float Now)
{
	//���ֵ
	pid->Bias[2] = pid->Bias[1];
	pid->Bias[1] = pid->Bias[0];
	pid->Bias[0] = Target-Now;
	
	//PID����
	pid->Pout = pid->Kp * pid->Bias[0];
	pid->Iout += pid->Ki * pid->Bias[0];
	pid->Dout = pid->Kd * (pid->Bias[0]-pid->Bias[1]);
	
	//����ȫ΢��
	pid->Dout = Low_Pass_Filter(&pid->HSPID_Struct,pid->Dout);
	
	//�����޷�
	if(pid->Iout > pid->Max_Iout){pid->Iout = pid->Max_Iout;}
	if(pid->Iout < pid->Min_Iout){pid->Iout = pid->Min_Iout;}
	
	//�������ֵ
	pid->Out_Put = pid->Pout + pid->Iout + pid->Dout;
	
	//��������
	if(fabs(pid->Bias[0]) < pid->Blind_Spot){pid->Out_Put = 0;}
	
	//����޷�
	if(pid->Out_Put > pid->Max_out){pid->Out_Put = pid->Max_out;}
	if(pid->Out_Put < pid->Min_out){pid->Out_Put = pid->Min_out;}
	
	//�������ֵ
	return (float)pid->Out_Put;
}


/* -------------------- PID������ʼ��ֵ -------------------- */

/**
  * @brief  �ϵ��ʼ��ʱ��ֵ
  * @param  None
  * @retval None
*/
void PID_All_Init(void)
{
	//��������PID��ʼ��
	PID_Init(&Wheel1_Speed_t,WHEEL_SPEED_KP+WHEEL1_KP_COMPENSATE,WHEEL_SPEED_KI,WHEEL_SPEED_KD,WHEEL_SPEED_PID_MAX,WHEEL_SPEED_MAX_IOUT,WHEEL_SPEED_BLIND,WHEEL_SPEED_HSPID);
	PID_Init(&Wheel2_Speed_t,WHEEL_SPEED_KP+WHEEL2_KP_COMPENSATE,WHEEL_SPEED_KI,WHEEL_SPEED_KD,WHEEL_SPEED_PID_MAX,WHEEL_SPEED_MAX_IOUT,WHEEL_SPEED_BLIND,WHEEL_SPEED_HSPID);
	PID_Init(&Wheel3_Speed_t,WHEEL_SPEED_KP+WHEEL3_KP_COMPENSATE,WHEEL_SPEED_KI,WHEEL_SPEED_KD,WHEEL_SPEED_PID_MAX,WHEEL_SPEED_MAX_IOUT,WHEEL_SPEED_BLIND,WHEEL_SPEED_HSPID);
	PID_Init(&Wheel4_Speed_t,WHEEL_SPEED_KP+WHEEL4_KP_COMPENSATE,WHEEL_SPEED_KI,WHEEL_SPEED_KD,WHEEL_SPEED_PID_MAX,WHEEL_SPEED_MAX_IOUT,WHEEL_SPEED_BLIND,WHEEL_SPEED_HSPID);
	//���̸�����̨��ʼ��
	PID_Init(&Base_Follow_Gimbal_t,BASE_FOLLOW_GIMBAL_KP,BASE_FOLLOW_GIMBAL_KI,BASE_FOLLOW_GIMBAL_KD,BASE_FOLLOW_GIMBAL_PID_MAX,BASE_FOLLOW_GIMBAL_MAX_IOUT,BASE_FOLLOW_GIMBAL_BLIND,BASE_FOLLOW_GIMBAL_HSPID);
	//Yaw�ٶȽǶȳ�ʼ��(IMU���ԽǶ�)
	PID_Init(&Yaw_Motor_Speed_t,YAW_MOTOR_SPEED_KP,YAW_MOTOR_SPEED_KI,YAW_MOTOR_SPEED_KD,YAW_MOTOR_SPEED_PID_MAX,YAW_MOTOR_SPEED_MAX_IOUT,YAW_MOTOR_SPEED_BLIND,YAW_MOTOR_SPEED_HSPID);
	PID_Init(&Yaw_IMU_Angle_t,YAW_IMU_ANGLE_KP,YAW_IMU_ANGLE_KI,YAW_IMU_ANGLE_KD,YAW_IMU_ANGLE_PID_MAX,YAW_IMU_ANGLE_MAX_IOUT,YAW_IMU_ANGLE_BLIND,YAW_IMU_ANGLE_HSPID);
	//Pitch�ٶȽǶȳ�ʼ��(6020�Ƕ�)
	PID_Init(&Pitch_Motor_Speed_t,PITCH_MOTOR_SPEED_KP,PITCH_MOTOR_SPEED_KI,PITCH_MOTOR_SPEED_KD,PITCH_MOTOR_SPEED_PID_MAX,PITCH_MOTOR_SPEED_MAX_IOUT,PITCH_MOTOR_SPEED_BLIND,PITCH_MOTOR_SPEED_HSPID);
	PID_Init(&Pitch_Motor_Angle_t,PITCH_MOTOR_ANGLE_KP,PITCH_MOTOR_ANGLE_KI,PITCH_MOTOR_ANGLE_KD,PITCH_MOTOR_ANGLE_PID_MAX,PITCH_MOTOR_ANGLE_MAX_IOUT,PITCH_MOTOR_ANGLE_BLIND,PITCH_MOTOR_ANGLE_HSPID);
	//�������ٶȻ���ʼ��
	PID_Init(&Shoot1_Speed_t,SHOOT_SPEED_KP,SHOOT_SPEED_KI,SHOOT_SPEED_KD,SHOOT_SPEED_PID_MAX,SHOOT_SPEED_MAX_IOUT,SHOOT_SPEED_BLIND,SHOOT_SPEED_HSPID);
	PID_Init(&Shoot2_Speed_t,SHOOT_SPEED_KP,SHOOT_SPEED_KI,SHOOT_SPEED_KD,SHOOT_SPEED_PID_MAX,SHOOT_SPEED_MAX_IOUT,SHOOT_SPEED_BLIND,SHOOT_SPEED_HSPID);
}
