#ifndef __MOTOR_H
#define __MOTOR_H

//������������ṹ��
typedef struct 
{
	//����3508��������
	int16_t Base_Motor1_Driver_Current;
	int16_t Base_Motor2_Driver_Current;
	int16_t Base_Motor3_Driver_Current;
	int16_t Base_Motor4_Driver_Current;
	//yaw��pitch��6020��������
	int16_t Yaw_Motor_Driver_Current;
	float Pitch_Motor_Driver_Current;
	//������2006��������
	int16_t Shoot_Motor1_Driver_Current;
	int16_t Shoot_Motor2_Driver_Current;
	
}Motor_Current_Struct;


//����3508���ID
#define BASE_MOTOR_ALL_ID			0x200
#define BASE_MOTOR1_ID				0x201
#define BASE_MOTOR2_ID				0x202
#define BASE_MOTOR3_ID				0x203
#define BASE_MOTOR4_ID				0x204

//��̨������2006���ID
#define GIMBAL_MOTOR_ALL_ID			0x200
#define SHOOT_LEFT_MOTOR_ID			0x201	//�󲦵���
#define SHOOT_RIGHT_MOTOR_ID		0x202	//�Ҳ�����

//Yaw���Pitch����6020ID
#define YAW_PITCH_MOTOR_ALL_ID		0x1FF
#define YAW_MOTOR_ID				0x205	//Yaw��
#define PITCH_MOTOR_ID				0x205	//Pitch��


//���̵������(CAN1����)
//����:���1234�ĵ���ֵ(-16384~16384)
void Base_MotorAll_SetSpeed(int16_t Motor1, int16_t Motor2, int16_t Motor3, int16_t Motor4);

//��̨2006�����������(CAN2����)
//2006����ĵ���ֵ(-10000~10000)
void Gimbal_MotorAll_SetSpeed(int16_t Motor_Left, int16_t Motor_Right);

//Yaw��6020����(CAN1����)
//����:Yaw��6020����ֵ(-30000~30000)
void Yaw_Motor_SetSpeed(int16_t Yaw_Motor);

//Pitch��6020����(CAN2����)
//����:Pitch��6020����ֵ(-30000~30000)
void Pitch_Motor_SetSpeed(int16_t Pitch_Right);

#endif
