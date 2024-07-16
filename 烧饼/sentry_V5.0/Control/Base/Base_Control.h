#ifndef __BASE_CONTROL_H
#define __BASE_CONTROL_H


//���̿��ƽṹ��
typedef struct 
{
	//����ǰ������(���yaw��6020չ��ǰ������)
	int16_t Original_Forward_Direction;
	//ң����ģʽС����ģʽ�ٶ�
	int16_t Remote_Gyro_Target_Speed;
	//С������ת��־λ
	uint8_t Gyro_Rotating_Flag;
	
}Base_Control_Struct;

//�����־λ�ṹ��
typedef struct
{
	uint8_t Z_Flag;	 				//����Z����־λ
	uint8_t Push_R_Flag; 			//����Ҽ���־λ
	uint8_t Shift_Flag; 			//����Shift����־λ
	uint8_t E_Flag; 				//����E����־λ
	uint8_t F_Flag; 				//����F����־λ
	uint8_t C_Flag; 				//����C����־λ
	
	uint8_t Key_Friction_Flag;		//����Ħ���ֱ�־λ
	uint8_t Key_AimBot_Flag; 		//��������ģʽ��־λ
	uint8_t Key_Gyro_Flag; 			//����С����ģʽ��־λ
	uint8_t Key_Auto_Shot_Flag; 	//���������Զ�����ģʽ��־λ
	uint8_t Key_Turn_Flag; 			//������̨��תģʽ��־λ
	uint8_t Key_Cap_Flag; 			//���̳���ģʽ��־λ
	
}KeyMouse_Flag;

//ң���������������
void Base_thread_entry(void *parameter);


#endif
