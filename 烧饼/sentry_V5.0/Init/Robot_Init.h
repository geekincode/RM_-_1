#ifndef __ROBOT_INIT_H
#define __ROBOT_INIT_H


//���ܳ�ʼ���ṹ��
typedef struct 
{
	//��ʼǰ���У׼��־λ
	uint8_t Yaw_Angle_Out_Start_Flag;
	//ǰ���У׼�ǶȻ����
	float Angle_Result;
	//ǰ���У׼�ٶȻ����
	float Speed_Result;
	
}Robot_Init_Struct;


//�����ʼ��
void Peripheral_Init(void);
//���ܳ�ʼ���������
void Fun_Init_thread_entry(void *parameter);

#endif
