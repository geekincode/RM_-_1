/**
  ******************************************************************************
  * Description : ���������ļ�
  *  
  ******************************************************************************
  * @attention	
  *
  ******************************************************************************
*/


#ifndef __FORMULA_CONFIG_H
#define __FORMULA_CONFIG_H



/********************************************************************
							һ�׵�ͨ�˲�����  
**********************************************************************/


/* -------------------- �����ٶȻ��˲� -------------------- */

//�����ٶȻ��˲�����
#define WHEEL_KL				    		0.1


/* -------------------- Yaw���˲� -------------------- */

//Yaw���ٶȻ��˲�����
#define YAW_SPEED_KL						0.1
//Yaw��ǶȻ��˲�����
#define YAW_ANGLE_KL						0.7


/* -------------------- Pitch���˲� -------------------- */

//Pitch���ٶȻ��˲�����
#define PITCH_SPEED_KL						0.1
//Pitch��ǶȻ��˲�����
#define PITCH_ANGLE_KL						0.7


/* -------------------- Ħ�����ٶȻ��˲� -------------------- */

//Ħ�����ٶȻ��˲�����
#define FRICTION_SPEED_KL					0.1


/* -------------------- �������˲� -------------------- */

//�������ٶȻ��˲�����
#define SHOOT_SPEED_KL						0.1
//�����̽ǶȻ��˲�����
#define SHOOT_ANGLE_KL						0.8



/********************************************************************
							����3508��������  
**********************************************************************/


/* -------------------- �ٶȻ�PID���� -------------------- */

//1����Kp����
#define WHEEL1_KP_COMPENSATE				0
//2����Kp����
#define WHEEL2_KP_COMPENSATE				0
//3����Kp����
#define WHEEL3_KP_COMPENSATE				0
//4����Kp����
#define WHEEL4_KP_COMPENSATE				0
//KP
#define	WHEEL_SPEED_KP						1.6
//KI
#define WHEEL_SPEED_KI						0.0001
//KD
#define WHEEL_SPEED_KD						0
//PID����޷�
#define WHEEL_SPEED_PID_MAX					16384
//PID�����޷�
#define WHEEL_SPEED_MAX_IOUT				100
//����
#define WHEEL_SPEED_BLIND					0
//����ȫ����
#define WHEEL_SPEED_HSPID					1


/* -------------------- ���̸�����̨PID���� -------------------- */

//KP
#define	BASE_FOLLOW_GIMBAL_KP				4
//KI
#define BASE_FOLLOW_GIMBAL_KI				0
//KD
#define BASE_FOLLOW_GIMBAL_KD				70
//PID����޷�
#define BASE_FOLLOW_GIMBAL_PID_MAX			7000
//PID�����޷�
#define BASE_FOLLOW_GIMBAL_MAX_IOUT			0
//����
#define BASE_FOLLOW_GIMBAL_BLIND			0
//����ȫ����
#define BASE_FOLLOW_GIMBAL_HSPID			1



/********************************************************************
						Yaw��PID����(IMU���ԽǶ�)  
**********************************************************************/


/* -------------------- Yaw��ǶȻ�PID���� -------------------- */

//KP
#define	YAW_IMU_ANGLE_KP					2.5
//KI
#define YAW_IMU_ANGLE_KI					0.0
//KD
#define YAW_IMU_ANGLE_KD					100
//PID����޷�
#define YAW_IMU_ANGLE_PID_MAX				2000
//PID�����޷�
#define YAW_IMU_ANGLE_MAX_IOUT				0
//����
#define YAW_IMU_ANGLE_BLIND					0
//����ȫ����
#define YAW_IMU_ANGLE_HSPID					1


/* -------------------- Yaw��ǶȻ�ǰ���������� -------------------- */

//����Z���ٶ�ǰ������ϵ��(/)
#define	BASE_Z_SPEED_FEEDFORWARD			130


/* -------------------- Yaw���ٶȻ�PID���� -------------------- */

//KP
#define	YAW_MOTOR_SPEED_KP					200
//KI
#define YAW_MOTOR_SPEED_KI					0.001
//KD
#define YAW_MOTOR_SPEED_KD					0
//PID����޷�
#define YAW_MOTOR_SPEED_PID_MAX				30000
//PID�����޷�
#define YAW_MOTOR_SPEED_MAX_IOUT			3000
//����
#define YAW_MOTOR_SPEED_BLIND				0
//����ȫ����
#define YAW_MOTOR_SPEED_HSPID				1



/********************************************************************
						Pitch��PID����(IMU���ԽǶ�)  
**********************************************************************/


/* -------------------- Pitch��6020�ǶȻ�PID���� -------------------- */

//KP	0.8
#define	PITCH_MOTOR_ANGLE_KP				0.8
//KI	0
#define PITCH_MOTOR_ANGLE_KI				0
//KD	0.5
#define PITCH_MOTOR_ANGLE_KD				0.5
//PID����޷�
#define PITCH_MOTOR_ANGLE_PID_MAX			1000
//PID�����޷�
#define PITCH_MOTOR_ANGLE_MAX_IOUT			0
//����
#define PITCH_MOTOR_ANGLE_BLIND				0
//����ȫ����
#define PITCH_MOTOR_ANGLE_HSPID				1


/* -------------------- Pitch���ٶȻ�PID���� -------------------- */

//KP	80.0
#define	PITCH_MOTOR_SPEED_KP				0.7
//KI
#define PITCH_MOTOR_SPEED_KI				0
//KD
#define PITCH_MOTOR_SPEED_KD				0
//PID����޷�
#define PITCH_MOTOR_SPEED_PID_MAX			30000
//PID�����޷�
#define PITCH_MOTOR_SPEED_MAX_IOUT			1000
//����
#define PITCH_MOTOR_SPEED_BLIND				0
//����ȫ����
#define PITCH_MOTOR_SPEED_HSPID				1



/********************************************************************
						   ������PID����  
**********************************************************************/


/* -------------------- �ٶȻ�PID���� -------------------- */

//KP
#define	SHOOT_SPEED_KP						2.7
//KI
#define SHOOT_SPEED_KI						0.00006
//KD
#define SHOOT_SPEED_KD						0
//PID����޷�
#define SHOOT_SPEED_PID_MAX					16384
//PID�����޷�
#define SHOOT_SPEED_MAX_IOUT				100
//����
#define SHOOT_SPEED_BLIND					0
//����ȫ����
#define SHOOT_SPEED_HSPID					1



#endif
