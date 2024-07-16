/**
  ******************************************************************************
  * Description : ����һ���������Ĵ���
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- ȫ�ֱ��� -------------------- */

//������ṹ��
Error_Detect_Struct Error_Detect_t;



/* -------------------- �������������ں��� -------------------- */

/**
  * @brief  �������������ں���
  * @param  None
  * @retval None
*/
void Inspection_thread_entry(void *parameter)
{
	while(1)
	{
		//ִ�����ڼ���
		#if(EXECUTION_COUNT_SWITCH==1)
			Execution_Count_t.Detect_Execution_Count++;
		#endif
	
		//������
		if(Error_Detect_t.RC_Connected_Flag==0) Error_Detect_t.RC_Disconnected_Flag=1;
		else Error_Detect_t.RC_Disconnected_Flag=0;
		if(Error_Detect_t.BASE_MOTOR1_Connected_Flag==0) Error_Detect_t.BASE_MOTOR1_Disconnected_Flag=1;
		else Error_Detect_t.BASE_MOTOR1_Disconnected_Flag=0;
		if(Error_Detect_t.BASE_MOTOR2_Connected_Flag==0) Error_Detect_t.BASE_MOTOR2_Disconnected_Flag=1;
		else Error_Detect_t.BASE_MOTOR2_Disconnected_Flag=0;
		if(Error_Detect_t.BASE_MOTOR3_Connected_Flag==0) Error_Detect_t.BASE_MOTOR3_Disconnected_Flag=1;
		else Error_Detect_t.BASE_MOTOR3_Disconnected_Flag=0;
		if(Error_Detect_t.BASE_MOTOR4_Connected_Flag==0) Error_Detect_t.BASE_MOTOR4_Disconnected_Flag=1;
		else Error_Detect_t.BASE_MOTOR4_Disconnected_Flag=0;
		if(Error_Detect_t.YAW_MOTOR_Connected_Flag==0) Error_Detect_t.YAW_MOTOR_Disconnected_Flag=1;
		else Error_Detect_t.YAW_MOTOR_Disconnected_Flag=0;
		if(Error_Detect_t.PITCH_MOTOR_Connected_Flag==0) Error_Detect_t.PITCH_MOTOR_Disconnected_Flag=1;
		else Error_Detect_t.PITCH_MOTOR_Disconnected_Flag=0;
		if(Error_Detect_t.SHOOT_LEFT_MOTOR_Connected_Flag==0) Error_Detect_t.SHOOT_LEFT_MOTOR_Disconnected_Flag=1;
		else Error_Detect_t.SHOOT_LEFT_MOTOR_Disconnected_Flag=0;
		if(Error_Detect_t.Judge_Connected_Flag==0) Error_Detect_t.Judge_Disconnected_Flag=1;
		else Error_Detect_t.Judge_Disconnected_Flag=0;
		
		//���ñ�־λ
		Error_Detect_t.RC_Connected_Flag=0;
		Error_Detect_t.BASE_MOTOR1_Connected_Flag=0;
		Error_Detect_t.BASE_MOTOR2_Connected_Flag=0;
		Error_Detect_t.BASE_MOTOR3_Connected_Flag=0;
		Error_Detect_t.BASE_MOTOR4_Connected_Flag=0;
		Error_Detect_t.YAW_MOTOR_Connected_Flag=0;
		Error_Detect_t.PITCH_MOTOR_Connected_Flag=0;
		Error_Detect_t.SHOOT_LEFT_MOTOR_Connected_Flag=0;
		Error_Detect_t.SHOOT_RIGHT_MOTOR_Connected_Flag=0;
		Error_Detect_t.Judge_Connected_Flag=0;
		
		//������߱���
		#if (MOTOR_DISCONNECT_SWITCH==1)
			if( Error_Detect_t.BASE_MOTOR1_Disconnected_Flag==1 ||
				Error_Detect_t.BASE_MOTOR2_Disconnected_Flag==1 ||
				Error_Detect_t.BASE_MOTOR3_Disconnected_Flag==1 ||
				Error_Detect_t.BASE_MOTOR4_Disconnected_Flag==1 ||
				Error_Detect_t.YAW_MOTOR_Disconnected_Flag==1 ||
				Error_Detect_t.PITCH_MOTOR_Disconnected_Flag==1 ||
				Error_Detect_t.SHOOT_LEFT_MOTOR_Disconnected_Flag==1 ||
				Error_Detect_t.SHOOT_RIGHT_MOTOR_Disconnected_Flag==1 )
			{
				//��ѭ������־λ
				Error_Detect_t.Motor_Cycle_Detection_Flag=1;
			}
			else
			{
				//����ѭ������־λ
				Error_Detect_t.Motor_Cycle_Detection_Flag=0;
			}
		#endif
		//ң�������߱���	
		#if (RC_DISCONNECT_SWITCH==1)
			if(Error_Detect_t.RC_Disconnected_Flag==1)
			{
				//��ѭ������־λ
				Error_Detect_t.RC_Cycle_Detection_Flag=1;
			}
			else
			{
				//����ѭ������־λ
				Error_Detect_t.RC_Cycle_Detection_Flag=0;
			}
		#endif
		
		//������ִ������
		if(Error_Detect_t.RC_Cycle_Detection_Flag==0 && Error_Detect_t.Motor_Cycle_Detection_Flag==0) rt_thread_mdelay(DETECT_EXECUTION_CYCLE);
	}
}


/**
  * @brief  ���ң��������
  * @param  None
  * @retval None
*/
void Remote_Data_Detect(void)
{
	//ң�������ݴ�����
	if( rc_data.rc.ch0>660 || rc_data.rc.ch0<-660 ||
		rc_data.rc.ch0>660 || rc_data.rc.ch0<-660 ||
		rc_data.rc.ch0>660 || rc_data.rc.ch0<-660 ||
		rc_data.rc.ch0>660 || rc_data.rc.ch0<-660 )
	{
		//�ô����־λ
		Error_Detect_t.RC_Data_Error_Flag = 1;
		
		while ( rc_data.rc.ch0>660 || rc_data.rc.ch0<-660 ||
				rc_data.rc.ch0>660 || rc_data.rc.ch0<-660 ||
				rc_data.rc.ch0>660 || rc_data.rc.ch0<-660 ||
				rc_data.rc.ch0>660 || rc_data.rc.ch0<-660 )
		{
			//���³�ʼ��ң����
			Remote_Init();
			//�ȴ�ң�����յ��������ݰ�
			Error_Detect_t.RC_Connected_Flag=0;
			while(Error_Detect_t.RC_Connected_Flag==0);
			Timer_Delay_ms(200);
		}
		
		//��������־λ
		Error_Detect_t.RC_Data_Error_Flag = 0;
		
	}
}
