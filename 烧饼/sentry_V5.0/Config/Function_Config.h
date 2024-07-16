/**
  ******************************************************************************
  * Description : ���������ļ�
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#ifndef __FUNCTION_CONFIG_H
#define __FUNCTION_CONFIG_H



/********************************************************************
							���ܿ���  
**********************************************************************/

/**
  * @attention
  *
  *	
*/

//ִ�����ڼ���������
#define EXECUTION_COUNT_SWITCH			1
//����ģʽ����
#define STANDBY_SWITCH					1
//��̨Yaw���Ƿ�������ģʽ
#define YAW_DEBUGGING_MODE				0
//�Ƿ��Զ�Ѳ��
#define AUTOMATIC_INSPECTION_SWITCH		1
//�Ƿ���ң�������߱���
#define RC_DISCONNECT_SWITCH			0
//�Ƿ���������߱���
#define MOTOR_DISCONNECT_SWITCH			0
//��ͨģʽ�Ƿ�ʹ��Pitch��
#define NORMAL_PITCH_SWITCH				0



/********************************************************************
							ִ��Ƶ��  
**********************************************************************/

/**
  * @attention
  *
  *	
*/

//����ִ������(ms)
#define TASK_EXECUTION_CYCLE			2
//������ִ������(ms)
#define DETECT_EXECUTION_CYCLE			1000



/********************************************************************
							���̹��ܲ���  
**********************************************************************/

/**
  * @attention
  *
  *	
*/

//ң��������ƽ��������
#define RC_BASE_MOVE_SENS				8
//С����ƽ�Ʒ��򲹳�(6000/15)
#define GYRO_DIRECTION_COMPENSATION 	400
//ң����ģʽС�����ٶ�
#define RC_GYRO_MAX_SPEED				4500
//ң����С�����������ٲ���
#define RC_GYRO_START_STEP_SIZE			30
//ң����С����ֹͣ���ٲ���
#define RC_GYRO_STOP_STEP_SIZE			30
//ң����С����ֹͣ��С�ٶ�
#define RC_GYRO_STOP_MIN_SPEED			4000
//ң����С����ֹͣ��С�ǶȲ�
#define RC_GYRO_STOP_MIN_ANGLE			30



/********************************************************************
							��̨���ܲ���  
**********************************************************************/

/**
  * @attention
  *
  *	
*/

//ң������̨Yaw��ת��������
#define RC_YAW_TURN_SENS				0.0004
//ң����Pitch��������568
#define RC_PITCH_SENS					1200
//Pitch�����޽Ƕ�(6020�Ƕ�)6500
#define PITCH_UP_LIMIT					8000
//Pitch�����޽Ƕ�(6020�Ƕ�)5100
#define PITCH_DOWN_LIMIT				2000
//Pitch��ˮƽ�Ƕ�(6020�Ƕ�)
#define PITCH_INIT_ANGLE				6000
//Yaw��Ѳ��ģʽ������
#define YAW_PATROL_SEN					0.08
//Pitch��Ѳ��ģʽ������
#define PITCH_PATROL_SEN				2.5



/********************************************************************
							��ع��ܲ���  
**********************************************************************/

/**
  * @attention
  *
  *	
*/

//����
#define RATE_OF_FIRE					65
//��Ƶ
#define SHOOT_FREQUENCY					5000
//snailУ׼��С�г�
#define MIN_INTERVAL					20
//������1����
#define SHOOT1_SWITCH					1
//������2����
#define SHOOT2_SWITCH					1



/********************************************************************
						  �������ϵͳ���ȼ�����  
**********************************************************************/

/**
  * @attention
  *
  *	��ԽС���ȼ�Խ��
*/

//���ܳ�ʼ���������ȼ�
#define FUN_INIT_PRIORITY				0
//�������߳��������ȼ�
#define START_MAIN_PRIORITY				1
//��������ģʽ�������ȼ�
#define TURN_ON_STANDBY_PRIORITY		2
//���ݴ����������ȼ�
#define SAMPLING_PRIORITY				3
//��������������ȼ�
#define INSPECTION_PRIORITY				4
//��ƮУ׼�������ȼ�
#define IMU_OFFSET_PRIORITY				4
//��λ��ͨ���������ȼ�
#define PC_COMMUNICATE_PRIORITY			5
//�����������ȼ�
#define BASE_PRIORITY					5
//��̨�������ȼ�
#define GIMBAL_PRIORITY					5



/********************************************************************
							  �������  
**********************************************************************/

/**
  * @attention
  *
  *	�������Ƿ�ر����л�ĳ�����
*/

//����ܿ���
#define	MOTOR_SWITCH					1
//�����ĸ�3508����
#define	BASE_MOTOR_SWITCH				1
//YAW��6020����
#define	YAW_MOTOR_SWITCH				1
//PITCH��6020����
#define	PITCH_MOTOR_SWITCH				1
//�����̺�Ħ���ֿ���
#define SHOOT_MOTOR_SWITCH				1



#endif
