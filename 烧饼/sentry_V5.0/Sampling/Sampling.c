/**
  ******************************************************************************
  * Description : ����һ�����ݴ���Ĵ���
  *  
  ******************************************************************************
  * @attention	
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"


/* -------------------- ȫ�ֱ��� -------------------- */

//����˲����ݽṹ��
Motor_Filiter_Data_Struct Motor_Filiter_Data_t;


/* -------------------- ���ݴ������� -------------------- */

/**
  * @brief  ���ݴ����������
  * @param  None
  * @retval None
*/
void Sampling_thread_entry(void *parameter)
{
	while(1)
	{
		//ִ�����ڼ���
		#if(EXECUTION_COUNT_SWITCH==1)
			Execution_Count_t.Sampling_Execution_Count++;
		#endif
		
		//��������̬�������
		IMU_Updata();
		
		//��ͨ�˲���																			///��ÿһ������ķ���ֵ���е�ͨ�˲���ɸ����Ƶ�ź�
		Motor_Filiter_Data_t.Wheel_Motor1_Speed = Low_Pass_Filter(&Wheel1_Low_Pass_t, BASE_MOTOR1_Data_t.Speed);//�����ٶȻ���ͨ�˲�
		Motor_Filiter_Data_t.Wheel_Motor2_Speed = Low_Pass_Filter(&Wheel2_Low_Pass_t, BASE_MOTOR2_Data_t.Speed);
		Motor_Filiter_Data_t.Wheel_Motor3_Speed = Low_Pass_Filter(&Wheel3_Low_Pass_t, BASE_MOTOR3_Data_t.Speed);
		Motor_Filiter_Data_t.Wheel_Motor4_Speed = Low_Pass_Filter(&Wheel4_Low_Pass_t, BASE_MOTOR4_Data_t.Speed);
		Motor_Filiter_Data_t.Yaw_Motor_Speed = Low_Pass_Filter(&Yaw_Speed_Low_Pass_t, YAW_MOTOR_Data_t.Speed);//Yaw��Pitch���ٶȻ���ͨ�˲�
		Motor_Filiter_Data_t.Pitch_Motor_Speed = Low_Pass_Filter(&Pitch_Speed_Low_Pass_t, PITCH_MOTOR_Data_t.Speed);
		Motor_Filiter_Data_t.Yaw_Motor_Angle = Low_Pass_Filter(&Yaw_Angle_Low_Pass_t, YAW_MOTOR_Data_t.Angle);//Yaw��Pitch��ǶȻ���ͨ�˲�
		Motor_Filiter_Data_t.Pitch_Motor_Angle = Low_Pass_Filter(&Pitch_Angle_Low_Pass_t, PITCH_MOTOR_Data_t.Angle);
		Motor_Filiter_Data_t.Shoot_Motor1_Speed = Low_Pass_Filter(&Friction1_Speed_Low_Pass_t, SHOOT_LEFT_MOTOR_Data_t.Speed);//�������ٶȻ���ͨ�˲�
		Motor_Filiter_Data_t.Shoot_Motor2_Speed = Low_Pass_Filter(&Friction2_Speed_Low_Pass_t, SHOOT_RIGHT_MOTOR_Data_t.Speed);
		
		//ִ��Ƶ��
		rt_thread_mdelay(TASK_EXECUTION_CYCLE);
	}
}
