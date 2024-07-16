#ifndef __JUDGE_TASK_H
#define __JUDGE_TASK_H

//����ϵͳ��Ϣ�ṹ��
typedef struct 
{
	//����ϵͳ���ձ�־λ
	uint8_t Judge_Receive_Flag;
	
	//���ܵ�Ħ����״̬,С����״̬,����״̬,���翪��,�Զ�����״̬
	uint8_t Friction;
	uint8_t Gyro;
	uint8_t AimBot;
	uint8_t Cap;
	uint8_t Auto;
	
	//���յĵ�������
	int16_t Debug_Data;
	
}Judge_Struct;

//����ϵͳ��Ϣ�ṹ��
extern Judge_Struct Judge_t;


//����ϵͳ��Ϣ�����������
void UI_Protract_thread_entry(void *parameter);


#endif
