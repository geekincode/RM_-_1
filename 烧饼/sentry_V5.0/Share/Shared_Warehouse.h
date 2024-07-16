/**
  ******************************************************************************
  * Description : ȫ�ֲֿ�
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#ifndef __SHARED_WAREHOUSE_H
#define __SHARED_WAREHOUSE_H


/* -------------------- ͷ�ļ� -------------------- */

#include "stm32f4xx.h"
#include <rtthread.h>           				
#include <stdio.h>                  
#include <stdarg.h>    
#include "arm_math.h"				// DSP
#include "Function_Config.h"		// ģʽ��������ļ�
#include "Formula_Config.h"			// ��ʽ��������ļ�
#include "MyCAN.h"					// CAN���ͺͽ���
#include "Motor.h"					// �������
#include "Encoder.h"				// ����ش�����
#include "Function_Config.h"		// ģʽ��������ļ�
#include "Formula_Config.h"			// ��ʽ��������ļ�
#include "PWM.h"					// PWM����
#include "Task_Control.h"			// rtthread�������
#include "Robot_Init.h"				// �����˳�ʼ��
#include "Usart.h"					// ��λ�����ڳ�ʼ��
#include "Base_Timer.h"				// ��ʱ����ʼ��
#include "remote.h"					// ң��������
#include "Filter.h"					// �˲�����ʽ
#include "PID.h"					// PID��ʽ
#include "Sampling.h"				// ���ݴ���
#include "Base_Control.h"			// ���̿���
#include "Base_APP.h"				// ���̿���
#include "Gimbal_Control.h"			// ��̨����
#include "Gimbal_APP.h"				// ��̨����
#include "Detect.h"					// �������
#include "PC_Communicate.h"			// ��λ��ͨ��
#include "AiMBot_Config.h"			// ��������
#include "Power_Limit.h"			// ���̹�������
#include "Power_Limit_Config.h"		// ���̹������������ļ�
#include "Judge.h"					// ����ϵͳ����
#include "IMU.h"					// ��������̬����
#include "PWM.h"					// snail����
#include "Judge_crc.h"
#include "Judge_Task.h"



/* -------------------- ȫ�ֱ����ֿ� -------------------- */

/*  CAN12���յ�ԭʼ���ݰ�  */
extern CanRxMsg MyCAN1_Rece_Data;
extern CanRxMsg MyCAN2_Rece_Data;

/*  ң������������  */
extern RC_Data_Struct rc_data;

/*  ִ�����ڼ��ṹ��  */
extern Execution_Count_Struct Execution_Count_t;

/*  һ�׵�ͨ�˲��ṹ��  */
extern LowPass_Struct Wheel1_Low_Pass_t;						//���̵���˲��ṹ��
extern LowPass_Struct Wheel2_Low_Pass_t;
extern LowPass_Struct Wheel3_Low_Pass_t;
extern LowPass_Struct Wheel4_Low_Pass_t;
extern LowPass_Struct Yaw_Speed_Low_Pass_t;						//Yaw��Pitch�����˲��ṹ��
extern LowPass_Struct Pitch_Speed_Low_Pass_t;
extern LowPass_Struct Yaw_Angle_Low_Pass_t;						//Yaw��Pitch�����Ƕ��˲��ṹ��
extern LowPass_Struct Pitch_Angle_Low_Pass_t;
extern LowPass_Struct Friction1_Speed_Low_Pass_t;				//Ħ���ֵ���ٶ��˲��ṹ��
extern LowPass_Struct Friction2_Speed_Low_Pass_t;
extern LowPass_Struct Shoot_Speed_Low_Pass_t;					//�����̵���ٶ��˲��ṹ��
extern LowPass_Struct Shoot_Angle_Low_Pass_t;					//�����̵���Ƕ��˲��ṹ��

/*  �������ԭʼ����  */
extern DJI_Motor_Data_Struct BASE_MOTOR1_Data_t;				//���̵������ֵ
extern DJI_Motor_Data_Struct BASE_MOTOR2_Data_t;
extern DJI_Motor_Data_Struct BASE_MOTOR3_Data_t;
extern DJI_Motor_Data_Struct BASE_MOTOR4_Data_t;
extern DJI_Motor_Data_Struct SHOOT_LEFT_MOTOR_Data_t;			//������Ħ���ֵ������ֵ
extern DJI_Motor_Data_Struct SHOOT_RIGHT_MOTOR_Data_t;
extern DJI_Motor_Data_Struct YAW_MOTOR_Data_t;					//Yaw��Pitch��������ֵ
extern DJI_Motor_Data_Struct PITCH_MOTOR_Data_t;	

/*  PID�ṹ��  */
extern PID_Struct Wheel1_Speed_t;								//���ӽṹ��
extern PID_Struct Wheel2_Speed_t;
extern PID_Struct Wheel3_Speed_t;
extern PID_Struct Wheel4_Speed_t;
extern PID_Struct Base_Follow_Gimbal_t;							//���̸�����̨�ṹ��
extern PID_Struct Yaw_Motor_Speed_t;							//Yaw��Ƕ��ٶȻ��ṹ��(IMU���ԽǶ�)
extern PID_Struct Yaw_IMU_Angle_t;
extern PID_Struct Pitch_Motor_Speed_t;							//Pitch��Ƕ��ٶȻ��ṹ��(IMU���ԽǶ�)
extern PID_Struct Pitch_Motor_Angle_t;
extern PID_Struct Shoot1_Speed_t;								//������PID�ṹ��
extern PID_Struct Shoot2_Speed_t;

/*  ���ݴ���  */
extern Motor_Filiter_Data_Struct Motor_Filiter_Data_t;			//����˲����ݽṹ��
extern Yaw_Motor_Data_Handle_Struct Yaw_Motor_Data_Handle_t;	//Yaw�����ݴ���ṹ��

/*  �����������  */
extern Motor_Current_Struct Motor_Current_t;

/*  ���ƽṹ��  */
extern Kinematic_Solving_Struct Kinematic_Solving_t;			//�����˶������ṹ��
extern Base_Control_Struct Base_Control_t;						//���̿��ƽṹ��
extern Gimbal_Control_Struct Gimbal_Control_t;					//��̨���ƽṹ��

/*  �������  */
extern uint8_t Shoot_Flag;										//�����־λ

/*  ������  */
extern Error_Detect_Struct Error_Detect_t;						//������ṹ��


//�����־λ�ṹ��
extern KeyMouse_Flag KM_Flag_t;

//DMA����ԭʼ����
//extern uint8_t judge_buf[2][220] = {0};
//����ϵͳ���ݽṹ��
extern judge_t Judge;
extern judge_get_data_t Judge_Data;




#endif
