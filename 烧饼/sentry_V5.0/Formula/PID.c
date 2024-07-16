/**
  ******************************************************************************
  * Description : 这是一坨PID公式的代码
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- 全局变量 -------------------- */

//轮子速度环结构体
PID_Struct Wheel1_Speed_t;
PID_Struct Wheel2_Speed_t;
PID_Struct Wheel3_Speed_t;
PID_Struct Wheel4_Speed_t;
//底盘跟随云台结构体
PID_Struct Base_Follow_Gimbal_t;
//Yaw轴角度速度环结构体(IMU绝对角度)
PID_Struct Yaw_Motor_Speed_t;
PID_Struct Yaw_IMU_Angle_t;
//Pitch轴角度速度环结构体(IMU绝对角度)
PID_Struct Pitch_Motor_Speed_t;
PID_Struct Pitch_Motor_Angle_t;
//拨弹盘PID结构体
PID_Struct Shoot1_Speed_t;
PID_Struct Shoot2_Speed_t;



/* -------------------- PID初始化和清零 -------------------- */

/**
  * @brief  PID初始化
  * @param  PID结构体变量，PID参数，输出限幅，积分限幅，不完全微分，死区
  * @retval None
*/
void PID_Init(PID_Struct *pid,float kp,float ki,float kd,float Max_out,float Max_Iout,float blind_spot,float HSPID_kl)
{
	//PID参数赋值
	pid->Kp = kp;
	pid->Ki = ki;
	pid->Kd = kd;
	
	//限幅赋值
	pid->Max_out = Max_out;
	pid->Min_out = -Max_out;
	pid->Max_Iout = Max_Iout;
	pid->Min_Iout = -Max_Iout;
	
	//不完全微分滤波器初始化
	pid->HSPID_Struct.KL = HSPID_kl;
}

/**
  * @brief  PID清零
  * @param  PID结构体变量
  * @retval None
*/
void PID_clear(PID_Struct *pid)
{
	//变量清零
	pid->Bias[0] = pid->Bias[1] = pid->Bias[2] = 0;
	pid->Iout = pid->Out_Put = 0;
}


/* -------------------- PID公式 -------------------- */

/**
  * @brief  位置式PID
  * @param  PID结构体变量，目标值，当前值
  * @retval PID输出
*/
float Pos_PID (PID_Struct *pid,float Target,float Now)
{
	//误差值
	pid->Bias[2] = pid->Bias[1];
	pid->Bias[1] = pid->Bias[0];
	pid->Bias[0] = Target-Now;
	
	//PID计算
	pid->Pout = pid->Kp * pid->Bias[0];
	pid->Iout += pid->Ki * pid->Bias[0];
	pid->Dout = pid->Kd * (pid->Bias[0]-pid->Bias[1]);
	
	//不完全微分
	pid->Dout = Low_Pass_Filter(&pid->HSPID_Struct,pid->Dout);
	
	//积分限幅
	if(pid->Iout > pid->Max_Iout){pid->Iout = pid->Max_Iout;}
	if(pid->Iout < pid->Min_Iout){pid->Iout = pid->Min_Iout;}
	
	//计算输出值
	pid->Out_Put = pid->Pout + pid->Iout + pid->Dout;
	
	//死区保护
	if(fabs(pid->Bias[0]) < pid->Blind_Spot){pid->Out_Put = 0;}
	
	//输出限幅
	if(pid->Out_Put > pid->Max_out){pid->Out_Put = pid->Max_out;}
	if(pid->Out_Put < pid->Min_out){pid->Out_Put = pid->Min_out;}
	
	//返回输出值
	return (float)pid->Out_Put;
}


/* -------------------- PID参数初始赋值 -------------------- */

/**
  * @brief  上电初始化时赋值
  * @param  None
  * @retval None
*/
void PID_All_Init(void)
{
	//底盘轮子PID初始化
	PID_Init(&Wheel1_Speed_t,WHEEL_SPEED_KP+WHEEL1_KP_COMPENSATE,WHEEL_SPEED_KI,WHEEL_SPEED_KD,WHEEL_SPEED_PID_MAX,WHEEL_SPEED_MAX_IOUT,WHEEL_SPEED_BLIND,WHEEL_SPEED_HSPID);
	PID_Init(&Wheel2_Speed_t,WHEEL_SPEED_KP+WHEEL2_KP_COMPENSATE,WHEEL_SPEED_KI,WHEEL_SPEED_KD,WHEEL_SPEED_PID_MAX,WHEEL_SPEED_MAX_IOUT,WHEEL_SPEED_BLIND,WHEEL_SPEED_HSPID);
	PID_Init(&Wheel3_Speed_t,WHEEL_SPEED_KP+WHEEL3_KP_COMPENSATE,WHEEL_SPEED_KI,WHEEL_SPEED_KD,WHEEL_SPEED_PID_MAX,WHEEL_SPEED_MAX_IOUT,WHEEL_SPEED_BLIND,WHEEL_SPEED_HSPID);
	PID_Init(&Wheel4_Speed_t,WHEEL_SPEED_KP+WHEEL4_KP_COMPENSATE,WHEEL_SPEED_KI,WHEEL_SPEED_KD,WHEEL_SPEED_PID_MAX,WHEEL_SPEED_MAX_IOUT,WHEEL_SPEED_BLIND,WHEEL_SPEED_HSPID);
	//底盘跟随云台初始化
	PID_Init(&Base_Follow_Gimbal_t,BASE_FOLLOW_GIMBAL_KP,BASE_FOLLOW_GIMBAL_KI,BASE_FOLLOW_GIMBAL_KD,BASE_FOLLOW_GIMBAL_PID_MAX,BASE_FOLLOW_GIMBAL_MAX_IOUT,BASE_FOLLOW_GIMBAL_BLIND,BASE_FOLLOW_GIMBAL_HSPID);
	//Yaw速度角度初始化(IMU绝对角度)
	PID_Init(&Yaw_Motor_Speed_t,YAW_MOTOR_SPEED_KP,YAW_MOTOR_SPEED_KI,YAW_MOTOR_SPEED_KD,YAW_MOTOR_SPEED_PID_MAX,YAW_MOTOR_SPEED_MAX_IOUT,YAW_MOTOR_SPEED_BLIND,YAW_MOTOR_SPEED_HSPID);
	PID_Init(&Yaw_IMU_Angle_t,YAW_IMU_ANGLE_KP,YAW_IMU_ANGLE_KI,YAW_IMU_ANGLE_KD,YAW_IMU_ANGLE_PID_MAX,YAW_IMU_ANGLE_MAX_IOUT,YAW_IMU_ANGLE_BLIND,YAW_IMU_ANGLE_HSPID);
	//Pitch速度角度初始化(6020角度)
	PID_Init(&Pitch_Motor_Speed_t,PITCH_MOTOR_SPEED_KP,PITCH_MOTOR_SPEED_KI,PITCH_MOTOR_SPEED_KD,PITCH_MOTOR_SPEED_PID_MAX,PITCH_MOTOR_SPEED_MAX_IOUT,PITCH_MOTOR_SPEED_BLIND,PITCH_MOTOR_SPEED_HSPID);
	PID_Init(&Pitch_Motor_Angle_t,PITCH_MOTOR_ANGLE_KP,PITCH_MOTOR_ANGLE_KI,PITCH_MOTOR_ANGLE_KD,PITCH_MOTOR_ANGLE_PID_MAX,PITCH_MOTOR_ANGLE_MAX_IOUT,PITCH_MOTOR_ANGLE_BLIND,PITCH_MOTOR_ANGLE_HSPID);
	//拨弹盘速度环初始化
	PID_Init(&Shoot1_Speed_t,SHOOT_SPEED_KP,SHOOT_SPEED_KI,SHOOT_SPEED_KD,SHOOT_SPEED_PID_MAX,SHOOT_SPEED_MAX_IOUT,SHOOT_SPEED_BLIND,SHOOT_SPEED_HSPID);
	PID_Init(&Shoot2_Speed_t,SHOOT_SPEED_KP,SHOOT_SPEED_KI,SHOOT_SPEED_KD,SHOOT_SPEED_PID_MAX,SHOOT_SPEED_MAX_IOUT,SHOOT_SPEED_BLIND,SHOOT_SPEED_HSPID);
}
