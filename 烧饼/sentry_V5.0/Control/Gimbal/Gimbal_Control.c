/**
  ******************************************************************************
  * Description : ����һ��ң������̨���ƵĴ���
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- ȫ�ֱ��� -------------------- */

//��̨���ƽṹ��
Gimbal_Control_Struct Gimbal_Control_t;
/*
//ң������̨Yaw��ת��������
#define RC_YAW_TURN_SENS				0.0004
//ң����Pitch��������
#define RC_PITCH_SENS					568
//Pitch�����޽Ƕ�(6020�Ƕ�)6500
#define PITCH_UP_LIMIT					7500
//Pitch�����޽Ƕ�(6020�Ƕ�)5100
#define PITCH_DOWN_LIMIT				4100
//Pitch��ˮƽ�Ƕ�(6020�Ƕ�)
#define PITCH_INIT_ANGLE				6000
//Yaw��Ѳ��ģʽ������
#define YAW_PATROL_SEN					0.08
//Pitch��Ѳ��ģʽ������
#define PITCH_PATROL_SEN				2.5

//����
#define RATE_OF_FIRE					65
//��Ƶ
#define SHOOT_FREQUENCY					5000
//snailУ׼��С�г�
#define MIN_INTERVAL					20
//������1����
#define SHOOT1_SWITCH					1
//������2����
#define SHOOT2_SWITCH					1
*/
float Shoot1_Frequency,Shoot2_Frequency;


/* -------------------- �������� -------------------- */

//ң������̨���ƺ�������
void Gimbal_Remote_Control(void);
//��λ����̨���ƺ�������
void Gimbal_PC_Control(void);


//int a = Judge.game_robot_status.shooter_id1_17mm_speed_limit;


/* -------------------- ��̨�����߼� -------------------- */

/**
  * @brief  ��̨�������
  * @param  None
  * @retval None
*/
void Gimbal_thread_entry(void *parameter)
{
	while(1)
	{
		/** @function ִ�����ڼ��� ----- */
		
		//ִ�����ڼ���
		#if(EXECUTION_COUNT_SWITCH==1)
			Execution_Count_t.Gimbal_Execution_Count++;
		#endif
		
		
		/** @function ������ģʽ ----- */
		
		//ң��������
		if(rc_data.rc.s2!=2)
		{
			Gimbal_Remote_Control();
		}
		
		//��λ������
		if(rc_data.rc.s2==2)
		{
			Gimbal_PC_Control();
		}
		
		
		/** @function ������� ----- */
		
		Yaw_Motor_SetSpeed(Motor_Current_t.Yaw_Motor_Driver_Current);
		//Pitch_Motor_SetSpeed(8000);
		Pitch_Motor_SetSpeed(Gimbal_Control_t.Pitch_Angle_Result*20);						//ֻ�ýǶȻ�PID
		//Pitch_Motor_SetSpeed(Motor_Current_t.Pitch_Motor_Driver_Current);
		Gimbal_MotorAll_SetSpeed(Motor_Current_t.Shoot_Motor1_Driver_Current,Motor_Current_t.Shoot_Motor2_Driver_Current);
		
		
		/** @function ������ʱ ----- */
		
		//ִ��Ƶ��
		rt_thread_mdelay(TASK_EXECUTION_CYCLE);
	}
}


/**
  * @brief  ң������̨����
  * @param  None
  * @retval None
*/
void Gimbal_Remote_Control(void)
{
	/** @function Yaw��Ƕȿ��� ----- */
		
	//Yaw����ת
	if(rc_data.rc.ch0!=0)
	{
		Gimbal_Control_t.GimBla_Yaw_Angle -= (float)(rc_data.rc.ch0)*RC_YAW_TURN_SENS*TASK_EXECUTION_CYCLE;
	}
	Gimbal_Yaw_Action(Gimbal_Control_t.GimBla_Yaw_Angle);
	
	
	/** @function Pitch��Ƕȿ��� ----- */

	//Pitch����ת
	if(rc_data.rc.ch1!=0)
	{
		Gimbal_Control_t.GimBla_Pitch_Angle = PITCH_INIT_ANGLE + ((float)rc_data.rc.ch1/660*RC_PITCH_SENS);
		
		//��е��λ
		if(Gimbal_Control_t.GimBla_Pitch_Angle>PITCH_UP_LIMIT) Gimbal_Control_t.GimBla_Pitch_Angle = PITCH_UP_LIMIT;
		if(Gimbal_Control_t.GimBla_Pitch_Angle<PITCH_DOWN_LIMIT) Gimbal_Control_t.GimBla_Pitch_Angle = PITCH_DOWN_LIMIT;
	}
	Gimbal_Pitch_Action(Gimbal_Control_t.GimBla_Pitch_Angle);
	#if(NORMAL_PITCH_SWITCH==0)
		Motor_Current_t.Pitch_Motor_Driver_Current=0;
	#endif
	
	/** @function Ħ���ֿ��� ----- */
	
	//��Ħ����
	if(rc_data.rc.s1==2)
	{
		Snail_Shoot(RATE_OF_FIRE);
		Gimbal_Control_t.Friction_Start_Flag=1;
	}
	//��Ħ����
	else
	{
		Snail_Shoot(MIN_INTERVAL);
		Gimbal_Control_t.Friction_Start_Flag=0;
	}
	
	
	/** @function �������� ----- */
	
	//������Ƶ
	//float Shoot1_Frequency,Shoot2_Frequency;
	#if (SHOOT1_SWITCH==1)
		Shoot1_Frequency = (float)rc_data.rc.pulley_wheel*SHOOT_FREQUENCY/660;
		//num = Judge_t.Chassis_power + Judge_t.Shooter_heat;
	#endif
	#if (SHOOT2_SWITCH==1)
		Shoot2_Frequency = (float)rc_data.rc.pulley_wheel*SHOOT_FREQUENCY/660;
	#endif
	
	//����������
	if(rc_data.rc.pulley_wheel>=50)
	{
		Shoot_Speed_Action(-Shoot1_Frequency ,0);
	}
	//�ֶ���ת
	else if(rc_data.rc.pulley_wheel<=-50)
	{	
		Shoot_Speed_Action(0, -Shoot2_Frequency);
	}
	//������ֹͣ
	else
	{
		Shoot_Speed_Action(0,0);
	}
}


/**
  * @brief  ��λ����̨����
  * @param  None
  * @retval None
*/
void Gimbal_PC_Control(void)
{
	/** @function Yaw��Ƕȿ��� ----- */
		
	//Ѳ��ģʽYaw����ת
	#if (AUTOMATIC_INSPECTION_SWITCH==1)
	if(Shoot_Flag==0)		
	{
		Gimbal_Control_t.GimBla_Yaw_Angle += YAW_PATROL_SEN*TASK_EXECUTION_CYCLE;
	}
	#endif
	Gimbal_Yaw_Action(Gimbal_Control_t.GimBla_Yaw_Angle);
	
	
	/** @function Pitch��Ƕȿ��� ----- */

	//Ѳ��ģʽPitch����ת
	#if (AUTOMATIC_INSPECTION_SWITCH==1)
	if(Shoot_Flag==0)
	{
		if(Gimbal_Control_t.Patrol_Mode_Pitch_Direction==0) Gimbal_Control_t.GimBla_Pitch_Angle += PITCH_PATROL_SEN*TASK_EXECUTION_CYCLE;//����
		if(Gimbal_Control_t.Patrol_Mode_Pitch_Direction==1) Gimbal_Control_t.GimBla_Pitch_Angle -= PITCH_PATROL_SEN*TASK_EXECUTION_CYCLE;//����
		if(Gimbal_Control_t.GimBla_Pitch_Angle>PITCH_UP_LIMIT) Gimbal_Control_t.Patrol_Mode_Pitch_Direction=1;//�����Ϊ����
		if(Gimbal_Control_t.GimBla_Pitch_Angle<PITCH_DOWN_LIMIT) Gimbal_Control_t.Patrol_Mode_Pitch_Direction=0;//�����Ϊ����
	}
	#endif
	Gimbal_Pitch_Action(Gimbal_Control_t.GimBla_Pitch_Angle);
	
	
	/** @function Ħ���ֿ��� ----- */
	
	//��Ħ����
	if(rc_data.rc.s1==2)
	{
		Snail_Shoot(RATE_OF_FIRE);
		Gimbal_Control_t.Friction_Start_Flag=1;
	}
	//��Ħ����
	else
	{
		Snail_Shoot(MIN_INTERVAL);
		Gimbal_Control_t.Friction_Start_Flag=0;
	}
	
	
	/** @function ��� ----- */
	
	//������Ƶ
	float Shoot1_Frequency,Shoot2_Frequency;
	#if (SHOOT1_SWITCH==1)
		Shoot1_Frequency = SHOOT_FREQUENCY;
	#endif
	#if (SHOOT2_SWITCH==1)
		Shoot2_Frequency = SHOOT_FREQUENCY;
	#endif
	
	//����������
	if(Gimbal_Control_t.Friction_Start_Flag==1 && Shoot_Flag==1)
	{
		Shoot_Speed_Action(-Shoot1_Frequency ,Shoot2_Frequency);
	}
	else
	{
		Shoot_Speed_Action(0,0);
	}
}
