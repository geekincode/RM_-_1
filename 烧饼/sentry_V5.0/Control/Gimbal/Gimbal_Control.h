#ifndef __GIMBAL_CONTROL_H
#define __GIMBAL_CONTROL_H


//��̨���ƽṹ��
typedef struct 
{
	//��̨Yaw�ᳯ���(IMU�Ƕ�)
	float GimBla_Yaw_Angle;
	//����Z���ٶ�ǰ������
	float Yaw_FeedForward_For_Zspeed;
	//Yaw��ǶȻ�������
	float Yaw_Angle_Result;
	//��̨Pitch�ᳯ���(IMU�Ƕ�)
	float GimBla_Pitch_Angle;
	//Pitch��ǶȻ�������
	float Pitch_Angle_Result;
	//Ѳ��ģʽPitch�᷽��
	uint8_t Patrol_Mode_Pitch_Direction;
	//Ħ���ֿ�����־λ
	uint8_t Friction_Start_Flag;
	
	
}Gimbal_Control_Struct;

//ң������̨�������
void Gimbal_thread_entry(void *parameter);

#endif
