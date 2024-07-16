#ifndef __TASK_CONTROL_H
#define __TASK_CONTROL_H

//ִ�����ڼ��ṹ��
typedef struct
{
	//���ݴ�������ִ�д���
	uint32_t Sampling_Execution_Count;
	//�������ִ�д���
	uint32_t Detect_Execution_Count; 
	//��λ������ִ�д���
	uint32_t PC_Execution_Count; 	
	//��������ִ�д���
	uint32_t Base_Execution_Count; 
	//��̨����ִ�д���
	uint32_t Gimbal_Execution_Count; 
	
}Execution_Count_Struct;


//���ܳ�ʼ��
void Function_Init(void);
//�̳߳�ʼ������
void Task_Init(void);
//�������߳�
void Start_Thread(void);
//��������ģʽ
void Turn_On_Standby_Mode(void);
//�����������ģʽ
void Complete_Machine_Inspection(void);

#endif
