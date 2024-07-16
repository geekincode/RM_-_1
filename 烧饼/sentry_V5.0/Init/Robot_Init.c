/**
  ******************************************************************************
  * Description : ����һ������˳�ʼ���Ĵ���
  *  
  ******************************************************************************
  * @attention	
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"


/* -------------------- ȫ�ֱ��� -------------------- */

//���ܳ�ʼ���ṹ��
Robot_Init_Struct Robot_Init_t;



/* -------------------- ��ʼ�� -------------------- */


/**
  * @brief  �����ʼ��
  * @param  None
  * @retval None
*/
void Peripheral_Init(void)
{	
	//��ʱ����ʼ��
	Base_Timer_Init();
	
	//��ͨ�˲���ʼ��
	Low_Pass_Filter_Init();
	
	//PID��ʼ��
	PID_All_Init();	
	
	//CAN��ʼ��
	MyCAN_Init();	
	
	//ң������ʼ��
	Remote_Init();	
	
	//���ң��������
	while(Error_Detect_t.RC_Connected_Flag==0);//�ȴ�ң�����յ��������ݰ�
	Timer_Delay_ms(200);
	Remote_Data_Detect();
	
	//���̹������Ƴ�ʼ��
	//Power_Limit_Init();
	
	//���ڳ�ʼ��
	Serial_Init();
	
	//��ͨ�˲�����ʼ��
	Low_Pass_Filter_Init();

	//PWM��ʼ��
	PWM_Init();
	
	//�����ǳ�ʼ��
	IMU_Init();
}


/**
  * @brief  ���ܳ�ʼ���������
  * @param  None
  * @retval None
*/
void Fun_Init_thread_entry(void *parameter)
{
	//snial�г�У׼
	Snail_Init();
	
	//Pitch���ʼ�Ƕ�
	Gimbal_Control_t.GimBla_Pitch_Angle = PITCH_INIT_ANGLE;
	
	while(1)
	{
		if(rc_data.rc.s1==1)
		{
			//��ʼǰ���У׼
			Robot_Init_t.Yaw_Angle_Out_Start_Flag=1;
			//����ת��
			Gimbal_Control_t.GimBla_Yaw_Angle -= (float)(rc_data.rc.ch0)*RC_YAW_TURN_SENS*TASK_EXECUTION_CYCLE;
			
			//Yaw��PID����
			Robot_Init_t.Angle_Result = Pos_PID(&Yaw_IMU_Angle_t,Gimbal_Control_t.GimBla_Yaw_Angle,INS.YawTotalAngle);
			Robot_Init_t.Speed_Result = Pos_PID(&Yaw_Motor_Speed_t,Robot_Init_t.Angle_Result,YAW_MOTOR_Data_t.Speed);
			//���
			Yaw_Motor_SetSpeed(Robot_Init_t.Speed_Result);
			
			//Pitch��PID����
			Gimbal_Control_t.Pitch_Angle_Result = Pos_PID(&Pitch_Motor_Angle_t,Gimbal_Control_t.GimBla_Pitch_Angle,PITCH_MOTOR_Data_t.Angle);
			Motor_Current_t.Pitch_Motor_Driver_Current = Pos_PID(&Pitch_Motor_Speed_t,Gimbal_Control_t.Pitch_Angle_Result,PITCH_MOTOR_Data_t.Speed);
			//���
			Pitch_Motor_SetSpeed(Motor_Current_t.Pitch_Motor_Driver_Current);
		}
		
		//ǰ���У׼���
		if(rc_data.rc.s1==3 && Robot_Init_t.Yaw_Angle_Out_Start_Flag==1)
		{
			//ǰ��ǳ�ʼֵ
			Base_Control_t.Original_Forward_Direction = YAW_MOTOR_Data_t.Angle;
			
			if(Base_Control_t.Original_Forward_Direction<2730)//���⴦�����1
			{
				Yaw_Motor_Data_Handle_t.Yaw_Angle_Data_Special_Flag=1;
				Base_Control_t.Original_Forward_Direction = Base_Control_t.Original_Forward_Direction+2730;
			}
			if(Base_Control_t.Original_Forward_Direction>5460)//���⴦�����2
			{
				Yaw_Motor_Data_Handle_t.Yaw_Angle_Data_Special_Flag=2;
				Base_Control_t.Original_Forward_Direction = Base_Control_t.Original_Forward_Direction-2730; 
			}
			
			//�������߳�
			Start_Thread();
			
			//�˳�ǰ���У׼
			return;
		}
		
		//ϵͳ��ʱ
		rt_thread_mdelay(TASK_EXECUTION_CYCLE);
	}
}
