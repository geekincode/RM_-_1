/**
  ******************************************************************************
  * Description : ����һ����յ���������ݵĴ���
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"


/* -------------------- ȫ�ֱ��� -------------------- */

//���̵������ֵ
DJI_Motor_Data_Struct BASE_MOTOR1_Data_t;				///3508��6020�����ʹ��DJI_Motor_Data_Struct�ṹ����ʵ����
DJI_Motor_Data_Struct BASE_MOTOR2_Data_t;
DJI_Motor_Data_Struct BASE_MOTOR3_Data_t;
DJI_Motor_Data_Struct BASE_MOTOR4_Data_t;

//���Ҳ����̵������ֵ
DJI_Motor_Data_Struct SHOOT_LEFT_MOTOR_Data_t;
DJI_Motor_Data_Struct SHOOT_RIGHT_MOTOR_Data_t;

//Yaw��Pitch��������ֵ
DJI_Motor_Data_Struct YAW_MOTOR_Data_t;
DJI_Motor_Data_Struct PITCH_MOTOR_Data_t;

//Yaw�����ݴ���ṹ��
Yaw_Motor_Data_Handle_Struct Yaw_Motor_Data_Handle_t;



/* -------------------- �������⴦�� -------------------- */

/**
  * @brief  Yaw��Ƕ��������⴦��
  * @param  None
  * @retval None
*/
void Yaw_Angle_Data_Special_Handling(void)
{
	//Yaw��Ƕ�ԭʼ����չ��
	if(YAW_MOTOR_Data_t.Angle - Yaw_Motor_Data_Handle_t.YAW_Last_Data > 4095)//8191->0
	{
		Yaw_Motor_Data_Handle_t.YAW_Rotation_Number--;
	}
	if(YAW_MOTOR_Data_t.Angle - Yaw_Motor_Data_Handle_t.YAW_Last_Data < -4095)//0->8191
	{
		Yaw_Motor_Data_Handle_t.YAW_Rotation_Number++;
	}
	Yaw_Motor_Data_Handle_t.YAW_Expand_Data = YAW_MOTOR_Data_t.Angle + Yaw_Motor_Data_Handle_t.YAW_Rotation_Number*8191;
	Yaw_Motor_Data_Handle_t.YAW_Last_Data = YAW_MOTOR_Data_t.Angle;
	
	//��̨��ʵ����
	Yaw_Motor_Data_Handle_t.Yaw_Real_Expand_Data = (float)Yaw_Motor_Data_Handle_t.YAW_Expand_Data/3*2;
	//��̨��ʵȦ��
	Yaw_Motor_Data_Handle_t.Yaw_Real_Rotation_Number = (int16_t)(Yaw_Motor_Data_Handle_t.Yaw_Real_Expand_Data/8191);
	
	//Yaw�Ƕ�3:2��ֵ
	YAW_MOTOR_Data_t.Angle = (int16_t)Yaw_Motor_Data_Handle_t.Yaw_Real_Expand_Data - Yaw_Motor_Data_Handle_t.Yaw_Real_Rotation_Number*8191;
	if(YAW_MOTOR_Data_t.Angle<0){YAW_MOTOR_Data_t.Angle=8191+YAW_MOTOR_Data_t.Angle;}
	
	//�������⴦��
	if(Yaw_Motor_Data_Handle_t.Yaw_Angle_Data_Special_Flag==1)//<120
	{
		YAW_MOTOR_Data_t.Angle = YAW_MOTOR_Data_t.Angle+2730;
		if(YAW_MOTOR_Data_t.Angle>8191)
		{
			YAW_MOTOR_Data_t.Angle = YAW_MOTOR_Data_t.Angle-8191;
		}
	}
	else if(Yaw_Motor_Data_Handle_t.Yaw_Angle_Data_Special_Flag==2)//>240
	{
		YAW_MOTOR_Data_t.Angle = YAW_MOTOR_Data_t.Angle-2730;
		if(YAW_MOTOR_Data_t.Angle<0)
		{
			YAW_MOTOR_Data_t.Angle = 8191+YAW_MOTOR_Data_t.Angle;
		}
	}
}



/* -------------------- ���յ���3508��Yaw��6020�������ֵ -------------------- */

/**
  * @brief  ���շ���ֵ(�Ƕ�=ԭʼ����/8191*360)
  * @param  None
  * @retval None
*/
void Get_Base_Encode_Data(void)
{
	switch(MyCAN1_Rece_Data.StdId)
	{
		case BASE_MOTOR1_ID:							/// ��������1��3508���
			BASE_MOTOR1_Data_t.Angle=(MyCAN1_Rece_Data.Data[0]<<8)+MyCAN1_Rece_Data.Data[1];//��еת�ӽǶ�
			BASE_MOTOR1_Data_t.Speed=(MyCAN1_Rece_Data.Data[2]<<8)+MyCAN1_Rece_Data.Data[3];//ת��
			BASE_MOTOR1_Data_t.Current=(MyCAN1_Rece_Data.Data[4]<<8)+MyCAN1_Rece_Data.Data[5];//����
			BASE_MOTOR1_Data_t.Temperature=MyCAN1_Rece_Data.Data[6];//����¶�
			Error_Detect_t.BASE_MOTOR1_Connected_Flag=1;//�ñ�־λ
			break;
		
		case BASE_MOTOR2_ID:							/// ��������2��3508���
			BASE_MOTOR2_Data_t.Angle=(MyCAN1_Rece_Data.Data[0]<<8)+MyCAN1_Rece_Data.Data[1];//��еת�ӽǶ�
			BASE_MOTOR2_Data_t.Speed=(MyCAN1_Rece_Data.Data[2]<<8)+MyCAN1_Rece_Data.Data[3];//ת��
			BASE_MOTOR2_Data_t.Current=(MyCAN1_Rece_Data.Data[4]<<8)+MyCAN1_Rece_Data.Data[5];//����
			BASE_MOTOR2_Data_t.Temperature=MyCAN1_Rece_Data.Data[6];//����¶�
			Error_Detect_t.BASE_MOTOR2_Connected_Flag=1;//�ñ�־λ
			break;
		
		case BASE_MOTOR3_ID:							/// ��������3��3508���
			BASE_MOTOR3_Data_t.Angle=(MyCAN1_Rece_Data.Data[0]<<8)+MyCAN1_Rece_Data.Data[1];//��еת�ӽǶ�
			BASE_MOTOR3_Data_t.Speed=(MyCAN1_Rece_Data.Data[2]<<8)+MyCAN1_Rece_Data.Data[3];//ת��
			BASE_MOTOR3_Data_t.Current=(MyCAN1_Rece_Data.Data[4]<<8)+MyCAN1_Rece_Data.Data[5];//����
			BASE_MOTOR3_Data_t.Temperature=MyCAN1_Rece_Data.Data[6];//����¶�
			Error_Detect_t.BASE_MOTOR3_Connected_Flag=1;//�ñ�־λ
			break;
		
		case BASE_MOTOR4_ID:							/// ��������4��3508���
			BASE_MOTOR4_Data_t.Angle=(MyCAN1_Rece_Data.Data[0]<<8)+MyCAN1_Rece_Data.Data[1];//��еת�ӽǶ�
			BASE_MOTOR4_Data_t.Speed=(MyCAN1_Rece_Data.Data[2]<<8)+MyCAN1_Rece_Data.Data[3];//ת��
			BASE_MOTOR4_Data_t.Current=(MyCAN1_Rece_Data.Data[4]<<8)+MyCAN1_Rece_Data.Data[5];//����
			BASE_MOTOR4_Data_t.Temperature=MyCAN1_Rece_Data.Data[6];//����¶�
			Error_Detect_t.BASE_MOTOR4_Connected_Flag=1;//�ñ�־λ
			break;
		
		case YAW_MOTOR_ID:								///Yaw��С������ת��6020���
			YAW_MOTOR_Data_t.Angle=(MyCAN1_Rece_Data.Data[0]<<8)+MyCAN1_Rece_Data.Data[1];//��еת�ӽǶ�
			YAW_MOTOR_Data_t.Speed=(MyCAN1_Rece_Data.Data[2]<<8)+MyCAN1_Rece_Data.Data[3];//ת��
			YAW_MOTOR_Data_t.Current=(MyCAN1_Rece_Data.Data[4]<<8)+MyCAN1_Rece_Data.Data[5];//����
			YAW_MOTOR_Data_t.Temperature=MyCAN1_Rece_Data.Data[6];//����¶�
			//Yaw��Ƕ��������⴦��
			Yaw_Angle_Data_Special_Handling();
			Error_Detect_t.YAW_MOTOR_Connected_Flag=1;//�ñ�־λ
		/*
		case ROBOT_STATE_ID:
			Judge_t.Chassis_power=(float)((MyCAN1_Rece_Data.Data[0]<<8)+MyCAN1_Rece_Data.Data[1])/100;//���̹���
			Judge_t.Chassis_volt=(MyCAN1_Rece_Data.Data[2]<<8)+MyCAN1_Rece_Data.Data[3];//���̵�ѹ
			Judge_t.Chassis_power_buffer=(MyCAN1_Rece_Data.Data[4]<<8)+MyCAN1_Rece_Data.Data[5];//������������
			Judge_t.Shooter_heat=(MyCAN1_Rece_Data.Data[6]<<8)+MyCAN1_Rece_Data.Data[7];//ǹ������
			Error_Detect_t.Judge_Connected_Flag=1;//�ñ�־λ
			break;
		
		case ROBOT_RULES_ID:
			Judge_t.Chassis_power_limit=(MyCAN1_Rece_Data.Data[0]<<8)+MyCAN1_Rece_Data.Data[1];//���̹�������
			Judge_t.Shooter_speed_limit=(MyCAN1_Rece_Data.Data[2]<<8)+MyCAN1_Rece_Data.Data[3];//ǹ����������
			Judge_t.Shooter_cooling_limit=(MyCAN1_Rece_Data.Data[4]<<8)+MyCAN1_Rece_Data.Data[5];//ǹ����������
			Judge_t.Shooter_cooling_rate=(MyCAN1_Rece_Data.Data[6]<<8)+MyCAN1_Rece_Data.Data[7];//ǹ������ÿ����ȴֵ
			Error_Detect_t.Judge_Connected_Flag=1;//�ñ�־λ
			break;*/

	}
}


/* -------------------- ������̨3508��2006��Pitch��6020�������ֵ -------------------- */

/**
  * @brief  ���շ���ֵ(�Ƕ�=ԭʼ����/8191*360)
  * @param  None
  * @retval None
*/
void Get_Gimbal_Encode_Data(void)
{
	switch(MyCAN2_Rece_Data.StdId)
	{		
		case SHOOT_LEFT_MOTOR_ID:							///��Ħ���ֵ��
			SHOOT_LEFT_MOTOR_Data_t.Angle=(MyCAN2_Rece_Data.Data[0]<<8)+MyCAN2_Rece_Data.Data[1];//��еת�ӽǶ�
			SHOOT_LEFT_MOTOR_Data_t.Speed=(MyCAN2_Rece_Data.Data[2]<<8)+MyCAN2_Rece_Data.Data[3];//ת��
			SHOOT_LEFT_MOTOR_Data_t.Current=(MyCAN2_Rece_Data.Data[4]<<8)+MyCAN2_Rece_Data.Data[5];//����
			SHOOT_LEFT_MOTOR_Data_t.Temperature=MyCAN2_Rece_Data.Data[6];//����¶�
			Error_Detect_t.SHOOT_LEFT_MOTOR_Connected_Flag=1;//�ñ�־λ
			break;
		
		case SHOOT_RIGHT_MOTOR_ID:							///��Ħ���ֵ��
			SHOOT_RIGHT_MOTOR_Data_t.Angle=(MyCAN2_Rece_Data.Data[0]<<8)+MyCAN2_Rece_Data.Data[1];//��еת�ӽǶ�
			SHOOT_RIGHT_MOTOR_Data_t.Speed=(MyCAN2_Rece_Data.Data[2]<<8)+MyCAN2_Rece_Data.Data[3];//ת��
			SHOOT_RIGHT_MOTOR_Data_t.Current=(MyCAN2_Rece_Data.Data[4]<<8)+MyCAN2_Rece_Data.Data[5];//����
			SHOOT_RIGHT_MOTOR_Data_t.Temperature=MyCAN2_Rece_Data.Data[6];//����¶�
			Error_Detect_t.SHOOT_RIGHT_MOTOR_Connected_Flag=1;//�ñ�־λ
			break;
		
		case PITCH_MOTOR_ID:								///Pitch�������ƶ�6020���
			PITCH_MOTOR_Data_t.Angle=(MyCAN2_Rece_Data.Data[0]<<8)+MyCAN2_Rece_Data.Data[1];//��еת�ӽǶ�
			PITCH_MOTOR_Data_t.Speed=(MyCAN2_Rece_Data.Data[2]<<8)+MyCAN2_Rece_Data.Data[3];//ת��
			PITCH_MOTOR_Data_t.Current=(MyCAN2_Rece_Data.Data[4]<<8)+MyCAN2_Rece_Data.Data[5];//����
			PITCH_MOTOR_Data_t.Temperature=MyCAN2_Rece_Data.Data[6];//����¶�
			Error_Detect_t.PITCH_MOTOR_Connected_Flag=1;//�ñ�־λ
			break;
	}
}
