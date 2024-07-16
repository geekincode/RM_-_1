/**
  ******************************************************************************
  * Description : 参数配置文件
  *  
  ******************************************************************************
  * @attention	
  *
  ******************************************************************************
*/


#ifndef __FORMULA_CONFIG_H
#define __FORMULA_CONFIG_H



/********************************************************************
							一阶低通滤波配置  
**********************************************************************/


/* -------------------- 轮子速度环滤波 -------------------- */

//轮子速度环滤波参数
#define WHEEL_KL				    		0.1


/* -------------------- Yaw轴滤波 -------------------- */

//Yaw轴速度环滤波参数
#define YAW_SPEED_KL						0.1
//Yaw轴角度环滤波参数
#define YAW_ANGLE_KL						0.7


/* -------------------- Pitch轴滤波 -------------------- */

//Pitch轴速度环滤波参数
#define PITCH_SPEED_KL						0.1
//Pitch轴角度环滤波参数
#define PITCH_ANGLE_KL						0.7


/* -------------------- 摩擦轮速度环滤波 -------------------- */

//摩擦轮速度环滤波参数
#define FRICTION_SPEED_KL					0.1


/* -------------------- 拨弹盘滤波 -------------------- */

//拨弹盘速度环滤波参数
#define SHOOT_SPEED_KL						0.1
//拨弹盘角度环滤波参数
#define SHOOT_ANGLE_KL						0.8



/********************************************************************
							底盘3508参数配置  
**********************************************************************/


/* -------------------- 速度环PID参数 -------------------- */

//1号轮Kp补偿
#define WHEEL1_KP_COMPENSATE				0
//2号轮Kp补偿
#define WHEEL2_KP_COMPENSATE				0
//3号轮Kp补偿
#define WHEEL3_KP_COMPENSATE				0
//4号轮Kp补偿
#define WHEEL4_KP_COMPENSATE				0
//KP
#define	WHEEL_SPEED_KP						1.6
//KI
#define WHEEL_SPEED_KI						0.0001
//KD
#define WHEEL_SPEED_KD						0
//PID输出限幅
#define WHEEL_SPEED_PID_MAX					16384
//PID积分限幅
#define WHEEL_SPEED_MAX_IOUT				100
//死区
#define WHEEL_SPEED_BLIND					0
//不完全积分
#define WHEEL_SPEED_HSPID					1


/* -------------------- 底盘跟随云台PID参数 -------------------- */

//KP
#define	BASE_FOLLOW_GIMBAL_KP				4
//KI
#define BASE_FOLLOW_GIMBAL_KI				0
//KD
#define BASE_FOLLOW_GIMBAL_KD				70
//PID输出限幅
#define BASE_FOLLOW_GIMBAL_PID_MAX			7000
//PID积分限幅
#define BASE_FOLLOW_GIMBAL_MAX_IOUT			0
//死区
#define BASE_FOLLOW_GIMBAL_BLIND			0
//不完全积分
#define BASE_FOLLOW_GIMBAL_HSPID			1



/********************************************************************
						Yaw轴PID参数(IMU绝对角度)  
**********************************************************************/


/* -------------------- Yaw轴角度环PID参数 -------------------- */

//KP
#define	YAW_IMU_ANGLE_KP					2.5
//KI
#define YAW_IMU_ANGLE_KI					0.0
//KD
#define YAW_IMU_ANGLE_KD					100
//PID输出限幅
#define YAW_IMU_ANGLE_PID_MAX				2000
//PID积分限幅
#define YAW_IMU_ANGLE_MAX_IOUT				0
//死区
#define YAW_IMU_ANGLE_BLIND					0
//不完全积分
#define YAW_IMU_ANGLE_HSPID					1


/* -------------------- Yaw轴角度环前馈补偿参数 -------------------- */

//底盘Z轴速度前馈补偿系数(/)
#define	BASE_Z_SPEED_FEEDFORWARD			130


/* -------------------- Yaw轴速度环PID参数 -------------------- */

//KP
#define	YAW_MOTOR_SPEED_KP					200
//KI
#define YAW_MOTOR_SPEED_KI					0.001
//KD
#define YAW_MOTOR_SPEED_KD					0
//PID输出限幅
#define YAW_MOTOR_SPEED_PID_MAX				30000
//PID积分限幅
#define YAW_MOTOR_SPEED_MAX_IOUT			3000
//死区
#define YAW_MOTOR_SPEED_BLIND				0
//不完全积分
#define YAW_MOTOR_SPEED_HSPID				1



/********************************************************************
						Pitch轴PID参数(IMU绝对角度)  
**********************************************************************/


/* -------------------- Pitch轴6020角度环PID参数 -------------------- */

//KP	0.8
#define	PITCH_MOTOR_ANGLE_KP				0.8
//KI	0
#define PITCH_MOTOR_ANGLE_KI				0
//KD	0.5
#define PITCH_MOTOR_ANGLE_KD				0.5
//PID输出限幅
#define PITCH_MOTOR_ANGLE_PID_MAX			1000
//PID积分限幅
#define PITCH_MOTOR_ANGLE_MAX_IOUT			0
//死区
#define PITCH_MOTOR_ANGLE_BLIND				0
//不完全积分
#define PITCH_MOTOR_ANGLE_HSPID				1


/* -------------------- Pitch轴速度环PID参数 -------------------- */

//KP	80.0
#define	PITCH_MOTOR_SPEED_KP				0.7
//KI
#define PITCH_MOTOR_SPEED_KI				0
//KD
#define PITCH_MOTOR_SPEED_KD				0
//PID输出限幅
#define PITCH_MOTOR_SPEED_PID_MAX			30000
//PID积分限幅
#define PITCH_MOTOR_SPEED_MAX_IOUT			1000
//死区
#define PITCH_MOTOR_SPEED_BLIND				0
//不完全积分
#define PITCH_MOTOR_SPEED_HSPID				1



/********************************************************************
						   拨弹盘PID参数  
**********************************************************************/


/* -------------------- 速度环PID参数 -------------------- */

//KP
#define	SHOOT_SPEED_KP						2.7
//KI
#define SHOOT_SPEED_KI						0.00006
//KD
#define SHOOT_SPEED_KD						0
//PID输出限幅
#define SHOOT_SPEED_PID_MAX					16384
//PID积分限幅
#define SHOOT_SPEED_MAX_IOUT				100
//死区
#define SHOOT_SPEED_BLIND					0
//不完全积分
#define SHOOT_SPEED_HSPID					1



#endif
