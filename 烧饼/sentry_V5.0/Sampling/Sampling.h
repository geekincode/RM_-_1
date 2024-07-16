#ifndef __SAMPLING_H
#define __SAMPLING_H

//����˲����ݽṹ��
typedef struct
{
	//����3508�ٶ��˲����
	int16_t Wheel_Motor1_Speed;
	int16_t Wheel_Motor2_Speed;
	int16_t Wheel_Motor3_Speed;
	int16_t Wheel_Motor4_Speed;
	
	//Yaw��Pitch��6020�ٶ��˲����
	int16_t Yaw_Motor_Speed;
	int16_t Pitch_Motor_Speed;
	
	//Yaw��Pitch��6020�Ƕ��˲����
	int32_t Yaw_Motor_Angle;
	int16_t Pitch_Motor_Angle;
	
	//������2006�ٶ��˲����
	int16_t Shoot_Motor1_Speed;
	int16_t Shoot_Motor2_Speed;
	
}Motor_Filiter_Data_Struct;


//���ݴ����������
void Sampling_thread_entry(void *parameter);

#endif
