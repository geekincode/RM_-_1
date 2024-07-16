#ifndef __DETECT_H
#define __DETECT_H


//������ṹ��
typedef struct 
{
	//ң�������ݴ����־λ
	uint8_t RC_Data_Error_Flag;
	//ң�������ӱ�־λ
	uint8_t RC_Connected_Flag;
	//����ϵͳ���ӱ�־λ
	uint8_t Judge_Connected_Flag;
	
	//������ӱ�־λ
	uint8_t BASE_MOTOR1_Connected_Flag;	
	uint8_t BASE_MOTOR2_Connected_Flag;
	uint8_t BASE_MOTOR3_Connected_Flag;
	uint8_t BASE_MOTOR4_Connected_Flag;
	uint8_t SHOOT_LEFT_MOTOR_Connected_Flag;
	uint8_t SHOOT_RIGHT_MOTOR_Connected_Flag;
	uint8_t YAW_MOTOR_Connected_Flag;
	uint8_t PITCH_MOTOR_Connected_Flag;
	
	//���ߴ����־λ
	uint8_t RC_Disconnected_Flag;
	uint8_t BASE_MOTOR1_Disconnected_Flag;	
	uint8_t BASE_MOTOR2_Disconnected_Flag;
	uint8_t BASE_MOTOR3_Disconnected_Flag;
	uint8_t BASE_MOTOR4_Disconnected_Flag;
	uint8_t SHOOT_LEFT_MOTOR_Disconnected_Flag;
	uint8_t SHOOT_RIGHT_MOTOR_Disconnected_Flag;
	uint8_t YAW_MOTOR_Disconnected_Flag;
	uint8_t PITCH_MOTOR_Disconnected_Flag;
	uint8_t Judge_Disconnected_Flag;
	
	//ѭ��������ݱ�־λ
	uint8_t RC_Cycle_Detection_Flag;
	uint8_t Motor_Cycle_Detection_Flag;
	
}Error_Detect_Struct;


//�������������ں���
void Inspection_thread_entry(void *parameter);
//���ң��������
void Remote_Data_Detect(void);

#endif
