/**
  ******************************************************************************
  * Description : ����һ���˲����Ĵ���
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- һ�׵�ͨ�˲��ṹ�� -------------------- */

//���̵���˲��ṹ��
LowPass_Struct Wheel1_Low_Pass_t;
LowPass_Struct Wheel2_Low_Pass_t;
LowPass_Struct Wheel3_Low_Pass_t;
LowPass_Struct Wheel4_Low_Pass_t;
//Yaw��Pitch�����ٶ��˲��ṹ��
LowPass_Struct Yaw_Speed_Low_Pass_t;
LowPass_Struct Pitch_Speed_Low_Pass_t;
//Yaw��Pitch�����Ƕ��˲��ṹ��
LowPass_Struct Yaw_Angle_Low_Pass_t;
LowPass_Struct Pitch_Angle_Low_Pass_t;
//Ħ���ֵ���ٶ��˲��ṹ��
LowPass_Struct Friction1_Speed_Low_Pass_t;
LowPass_Struct Friction2_Speed_Low_Pass_t;
//�����̵���ٶ��˲��ṹ��
LowPass_Struct Shoot_Speed_Low_Pass_t;
//�����̵���Ƕ��˲��ṹ��
LowPass_Struct Shoot_Angle_Low_Pass_t;



/* -------------------- һ�׵�ͨ�˲� -------------------- */

/**
  * @brief  һ�׵�ͨ�˲���ʽ
  * @param  �˲��ṹ��filter �����β�������num
  * @retval �˲����
*/
float Low_Pass_Filter(LowPass_Struct *filter,float num)
{
	//�˲���ʽ
	filter->OutPut = filter->KL * num + (1 - filter->KL) * filter->Last_num; 				///Y(n)=kX(n)+(1-k)Y(n-1)
	
	//��ֵ�ϴβ���ֵ
	filter->Last_num = filter->OutPut;

	//���
	return filter->OutPut;
}

/**
  * @brief  һ�׵�ͨ�˲���ʼ��
  * @param  None
  * @retval None
*/
void Low_Pass_Filter_Init(void)
{
	Wheel1_Low_Pass_t.KL = Wheel2_Low_Pass_t.KL = Wheel3_Low_Pass_t.KL = Wheel4_Low_Pass_t.KL = WHEEL_KL;
	Yaw_Speed_Low_Pass_t.KL = YAW_SPEED_KL;
	Pitch_Speed_Low_Pass_t.KL = PITCH_SPEED_KL;
	Yaw_Angle_Low_Pass_t.KL = YAW_ANGLE_KL;
	Pitch_Angle_Low_Pass_t.KL = PITCH_ANGLE_KL;
	Friction1_Speed_Low_Pass_t.KL = Friction2_Speed_Low_Pass_t.KL = FRICTION_SPEED_KL;
	Shoot_Speed_Low_Pass_t.KL = SHOOT_SPEED_KL;
	Shoot_Angle_Low_Pass_t.KL = SHOOT_ANGLE_KL;
}

/**
  * @brief  һ�׵�ͨ�˲�����
  * @param  �˲��ṹ��
  * @retval None
*/
void Low_Pass_Filter_Clear(LowPass_Struct *filter)
{
	filter->Last_num=0;
}
