#ifndef __MOTOR_H
#define __MOTOR_H

//电机驱动电流结构体
typedef struct 
{
	//轮子3508驱动电流
	int16_t Base_Motor1_Driver_Current;
	int16_t Base_Motor2_Driver_Current;
	int16_t Base_Motor3_Driver_Current;
	int16_t Base_Motor4_Driver_Current;
	//yaw轴pitch轴6020驱动电流
	int16_t Yaw_Motor_Driver_Current;
	float Pitch_Motor_Driver_Current;
	//拨弹盘2006驱动电流
	int16_t Shoot_Motor1_Driver_Current;
	int16_t Shoot_Motor2_Driver_Current;
	
}Motor_Current_Struct;


//底盘3508电机ID
#define BASE_MOTOR_ALL_ID			0x200
#define BASE_MOTOR1_ID				0x201
#define BASE_MOTOR2_ID				0x202
#define BASE_MOTOR3_ID				0x203
#define BASE_MOTOR4_ID				0x204

//云台拨弹盘2006电机ID
#define GIMBAL_MOTOR_ALL_ID			0x200
#define SHOOT_LEFT_MOTOR_ID			0x201	//左拨弹盘
#define SHOOT_RIGHT_MOTOR_ID		0x202	//右拨弹盘

//Yaw轴和Pitch轴电机6020ID
#define YAW_PITCH_MOTOR_ALL_ID		0x1FF
#define YAW_MOTOR_ID				0x205	//Yaw轴
#define PITCH_MOTOR_ID				0x205	//Pitch轴


//底盘电机驱动(CAN1总线)
//参数:电机1234的电流值(-16384~16384)
void Base_MotorAll_SetSpeed(int16_t Motor1, int16_t Motor2, int16_t Motor3, int16_t Motor4);

//云台2006拨弹电机驱动(CAN2总线)
//2006电机的电流值(-10000~10000)
void Gimbal_MotorAll_SetSpeed(int16_t Motor_Left, int16_t Motor_Right);

//Yaw轴6020驱动(CAN1总线)
//参数:Yaw轴6020电流值(-30000~30000)
void Yaw_Motor_SetSpeed(int16_t Yaw_Motor);

//Pitch轴6020驱动(CAN2总线)
//参数:Pitch轴6020电流值(-30000~30000)
void Pitch_Motor_SetSpeed(int16_t Pitch_Right);

#endif
