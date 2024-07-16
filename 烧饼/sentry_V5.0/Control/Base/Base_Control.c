/**
  ******************************************************************************
  * Description : ����һ��ң�������̿��ƵĴ���
  *  
  ******************************************************************************
  * @attention	
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- ȫ�ֱ��� -------------------- */

//���̿��ƽṹ��
Base_Control_Struct Base_Control_t;

//�����־λ�ṹ��
KeyMouse_Flag KM_Flag_t;



/* -------------------- �������� -------------------- */

//ң�������̿��ƺ�������
void Base_Remote_Control(void);
//��λ�����̿��ƺ�������
void Base_PC_Control(void);



/* -------------------- ���̿����߼� -------------------- */

/**
  * @brief  �����������
  * @param  None
  * @retval None
*/
void Base_thread_entry(void *parameter)
{
	while(1)
	{
		/** @function ִ�����ڼ��� ----- */

		#if(EXECUTION_COUNT_SWITCH==1)
			Execution_Count_t.Base_Execution_Count++;
		#endif
		
		
		/** @function ������ģʽ ----- */
		
		#if (STANDBY_SWITCH==1)							//���ϽǵĲ���1��2��3��λ
		if(rc_data.rc.s1==1)
		{
			Turn_On_Standby_Mode();
		}
		#endif
		
		/** @function ������ģʽ ----- */
		
		//ң��������
		if(rc_data.rc.s2!=2)
		{
			//ң���������߼�
			///Kinematic_Solving_t.Z_Speed = 0;
			Base_Remote_Control();
			///Kinematic_Solving_t.Z_Speed = 0;
		}
		
		//��λ������
		if(rc_data.rc.s2==2)
		{
			//��λ�������߼�
			Base_PC_Control();
		}
		
		/** @function �˶�ѧ���� ----- */

		Move_Speed_Transform();
		
		
		/** @function ������� ----- */
		
		Base_MotorAll_SetSpeed(Motor_Current_t.Base_Motor1_Driver_Current,Motor_Current_t.Base_Motor2_Driver_Current,
								Motor_Current_t.Base_Motor3_Driver_Current,Motor_Current_t.Base_Motor4_Driver_Current);
		
		
		/** @function ������ʱ ----- */
		
		//ִ��Ƶ��
		rt_thread_mdelay(TASK_EXECUTION_CYCLE);
	}
}


/**
  * @brief  ң�������̿���(xǰ���󸺣�y�����Ҹ�)
  * @param  None
  * @retval None
*/
void Base_Remote_Control(void)
{
	/** @function ǰ���ƶ�ҡ�� ----- */

	Kinematic_Solving_t.Base_X_Speed = rc_data.rc.ch3*RC_BASE_MOVE_SENS;
	
	/** @function �����ƶ�ҡ�� ----- */

	Kinematic_Solving_t.Base_Y_Speed = rc_data.rc.ch2*RC_BASE_MOVE_SENS;
		
	/** @function С������ת ----- */
	
	if(rc_data.rc.s2==1)
	{
		//С��������
		Remote_Gyro_Start();
		
		//С������ת
		Kinematic_Solving_t.Z_Speed = Base_Control_t.Remote_Gyro_Target_Speed;
		
		//С���ݿ�ʼ��ת
		Base_Control_t.Gyro_Rotating_Flag=1;
	}
	//С����ֹͣ
	else if(Base_Control_t.Remote_Gyro_Target_Speed>0)
	{
		//С����ֹͣ
		Remote_Gyro_Stop();
		
		//С������ת
		Kinematic_Solving_t.Z_Speed = Base_Control_t.Remote_Gyro_Target_Speed;
	}
	
	/** @function ���̸�����̨ ----- */
	
	if(rc_data.rc.s2!=1 && Base_Control_t.Gyro_Rotating_Flag==0)
	{
		Base_Follow_Gimbal_Action(Base_Control_t.Original_Forward_Direction);
	}
}


/**
  * @brief  ��λ�����̿���
  * @param  None
  * @retval None
*/
void Base_PC_Control(void)
{
	/** @function С������ת ----- */
	
	//С��������
	Remote_Gyro_Start();
	
	//С������ת
	Kinematic_Solving_t.Z_Speed = Base_Control_t.Remote_Gyro_Target_Speed;
	
	//С���ݿ�ʼ��ת
	Base_Control_t.Gyro_Rotating_Flag=1;
}
