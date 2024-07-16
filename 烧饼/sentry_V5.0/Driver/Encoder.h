#ifndef __ENCODER_H
#define __ENCODER_H

//�󽮵���ش����ݽṹ��
typedef struct
{
	//��е�Ƕ�
	int16_t Angle;
	//ת��
	int16_t Speed;
	//����
	int16_t Current;
	//�¶�
	int16_t Temperature;
	
}DJI_Motor_Data_Struct;

//Yaw�����ݴ���ṹ��
typedef struct
{
	//Yaw����һ��ԭʼ����
	int16_t YAW_Last_Data;
	//Yaw����������תȦ��
	int16_t YAW_Rotation_Number;
	//Yaw��ԭʼ����չ��
	int32_t YAW_Expand_Data;
	//Yaw����ʵ����չ��
	float Yaw_Real_Expand_Data;
	//Yaw����ʵ��תȦ��
	int16_t Yaw_Real_Rotation_Number;
	//Yaw���������⴦���־λ
	uint8_t Yaw_Angle_Data_Special_Flag;
	
}Yaw_Motor_Data_Handle_Struct;


//����3508��2006�������ֵ
void Get_Base_Encode_Data(void);
//������̨3508��2006��6020�������ֵ
void Get_Gimbal_Encode_Data(void);

#endif
