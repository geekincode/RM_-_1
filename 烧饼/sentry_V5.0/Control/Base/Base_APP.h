#ifndef __BASE_APP_H
#define __BASE_APP_H


//�����˶�ѧ����ṹ��
typedef struct 
{
	//xy�ٶ�(����ڵ���)
	int16_t Base_X_Speed;
	int16_t Base_Y_Speed;
	
	//xy�ٶ�(�������̨)
	int16_t Gimbal_X_Speed;
	int16_t Gimbal_Y_Speed;
	
	//z�ٶ�
	int16_t Z_Speed;
	
	//������̨��ԽǶ�
	float Yaw_Df_Ag;			///�Ƕ�
	float Yaw_Df_Ag_pi;			///����
	
	//�������Ŀ���ٶ�
	int16_t Target1,Target2,Target3,Target4;
	
	//ƽ�ƽǶȲ���
	float Direction_Compensation;
	
}Kinematic_Solving_Struct;


//�ٶ�ת��(ǰ���󸺣������Ҹ�)
void Move_Speed_Transform(void);
//ң����ģʽС��������
void Remote_Gyro_Start(void);
//ң����ģʽС����ֹͣ
void Remote_Gyro_Stop(void);
//���̸�����̨����
void Base_Follow_Gimbal_Action(int32_t Target);
//�ı���̨������ԽǶ�
void Alter_Relative_Angles(void);

//�����ٶȻ�����
void Wheel_Speed_Action(int16_t Target1,int16_t Target2,int16_t Target3,int16_t Target4);


#endif
