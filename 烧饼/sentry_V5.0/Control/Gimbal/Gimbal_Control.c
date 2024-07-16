/**
  ******************************************************************************
  * Description : 这是一坨遥控器云台控制的代码
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- 全局变量 -------------------- */

//云台控制结构体
Gimbal_Control_Struct Gimbal_Control_t;
/*
//遥控器云台Yaw轴转向灵敏度
#define RC_YAW_TURN_SENS				0.0004
//遥控器Pitch轴灵敏度
#define RC_PITCH_SENS					568
//Pitch轴上限角度(6020角度)6500
#define PITCH_UP_LIMIT					7500
//Pitch轴下限角度(6020角度)5100
#define PITCH_DOWN_LIMIT				4100
//Pitch轴水平角度(6020角度)
#define PITCH_INIT_ANGLE				6000
//Yaw轴巡检模式灵敏度
#define YAW_PATROL_SEN					0.08
//Pitch轴巡检模式灵敏度
#define PITCH_PATROL_SEN				2.5

//出速
#define RATE_OF_FIRE					65
//射频
#define SHOOT_FREQUENCY					5000
//snail校准最小行程
#define MIN_INTERVAL					20
//拨弹盘1开关
#define SHOOT1_SWITCH					1
//拨弹盘2开关
#define SHOOT2_SWITCH					1
*/
float Shoot1_Frequency,Shoot2_Frequency;


/* -------------------- 函数声明 -------------------- */

//遥控器云台控制函数声明
void Gimbal_Remote_Control(void);
//上位机云台控制函数声明
void Gimbal_PC_Control(void);


//int a = Judge.game_robot_status.shooter_id1_17mm_speed_limit;


/* -------------------- 云台控制逻辑 -------------------- */

/**
  * @brief  云台任务入口
  * @param  None
  * @retval None
*/
void Gimbal_thread_entry(void *parameter)
{
	while(1)
	{
		/** @function 执行周期计数 ----- */
		
		//执行周期计数
		#if(EXECUTION_COUNT_SWITCH==1)
			Execution_Count_t.Gimbal_Execution_Count++;
		#endif
		
		
		/** @function 检测控制模式 ----- */
		
		//遥控器控制
		if(rc_data.rc.s2!=2)
		{
			Gimbal_Remote_Control();
		}
		
		//上位机控制
		if(rc_data.rc.s2==2)
		{
			Gimbal_PC_Control();
		}
		
		
		/** @function 电机驱动 ----- */
		
		Yaw_Motor_SetSpeed(Motor_Current_t.Yaw_Motor_Driver_Current);
		//Pitch_Motor_SetSpeed(8000);
		Pitch_Motor_SetSpeed(Gimbal_Control_t.Pitch_Angle_Result*20);						//只用角度环PID
		//Pitch_Motor_SetSpeed(Motor_Current_t.Pitch_Motor_Driver_Current);
		Gimbal_MotorAll_SetSpeed(Motor_Current_t.Shoot_Motor1_Driver_Current,Motor_Current_t.Shoot_Motor2_Driver_Current);
		
		
		/** @function 任务延时 ----- */
		
		//执行频率
		rt_thread_mdelay(TASK_EXECUTION_CYCLE);
	}
}


/**
  * @brief  遥控器云台控制
  * @param  None
  * @retval None
*/
void Gimbal_Remote_Control(void)
{
	/** @function Yaw轴角度控制 ----- */
		
	//Yaw轴旋转
	if(rc_data.rc.ch0!=0)
	{
		Gimbal_Control_t.GimBla_Yaw_Angle -= (float)(rc_data.rc.ch0)*RC_YAW_TURN_SENS*TASK_EXECUTION_CYCLE;
	}
	Gimbal_Yaw_Action(Gimbal_Control_t.GimBla_Yaw_Angle);
	
	
	/** @function Pitch轴角度控制 ----- */

	//Pitch轴旋转
	if(rc_data.rc.ch1!=0)
	{
		Gimbal_Control_t.GimBla_Pitch_Angle = PITCH_INIT_ANGLE + ((float)rc_data.rc.ch1/660*RC_PITCH_SENS);
		
		//机械限位
		if(Gimbal_Control_t.GimBla_Pitch_Angle>PITCH_UP_LIMIT) Gimbal_Control_t.GimBla_Pitch_Angle = PITCH_UP_LIMIT;
		if(Gimbal_Control_t.GimBla_Pitch_Angle<PITCH_DOWN_LIMIT) Gimbal_Control_t.GimBla_Pitch_Angle = PITCH_DOWN_LIMIT;
	}
	Gimbal_Pitch_Action(Gimbal_Control_t.GimBla_Pitch_Angle);
	#if(NORMAL_PITCH_SWITCH==0)
		Motor_Current_t.Pitch_Motor_Driver_Current=0;
	#endif
	
	/** @function 摩擦轮开关 ----- */
	
	//开摩擦轮
	if(rc_data.rc.s1==2)
	{
		Snail_Shoot(RATE_OF_FIRE);
		Gimbal_Control_t.Friction_Start_Flag=1;
	}
	//关摩擦轮
	else
	{
		Snail_Shoot(MIN_INTERVAL);
		Gimbal_Control_t.Friction_Start_Flag=0;
	}
	
	
	/** @function 连续开火 ----- */
	
	//计算射频
	//float Shoot1_Frequency,Shoot2_Frequency;
	#if (SHOOT1_SWITCH==1)
		Shoot1_Frequency = (float)rc_data.rc.pulley_wheel*SHOOT_FREQUENCY/660;
		//num = Judge_t.Chassis_power + Judge_t.Shooter_heat;
	#endif
	#if (SHOOT2_SWITCH==1)
		Shoot2_Frequency = (float)rc_data.rc.pulley_wheel*SHOOT_FREQUENCY/660;
	#endif
	
	//拨弹盘驱动
	if(rc_data.rc.pulley_wheel>=50)
	{
		Shoot_Speed_Action(-Shoot1_Frequency ,0);
	}
	//手动反转
	else if(rc_data.rc.pulley_wheel<=-50)
	{	
		Shoot_Speed_Action(0, -Shoot2_Frequency);
	}
	//拨弹盘停止
	else
	{
		Shoot_Speed_Action(0,0);
	}
}


/**
  * @brief  上位机云台控制
  * @param  None
  * @retval None
*/
void Gimbal_PC_Control(void)
{
	/** @function Yaw轴角度控制 ----- */
		
	//巡检模式Yaw轴旋转
	#if (AUTOMATIC_INSPECTION_SWITCH==1)
	if(Shoot_Flag==0)		
	{
		Gimbal_Control_t.GimBla_Yaw_Angle += YAW_PATROL_SEN*TASK_EXECUTION_CYCLE;
	}
	#endif
	Gimbal_Yaw_Action(Gimbal_Control_t.GimBla_Yaw_Angle);
	
	
	/** @function Pitch轴角度控制 ----- */

	//巡检模式Pitch轴旋转
	#if (AUTOMATIC_INSPECTION_SWITCH==1)
	if(Shoot_Flag==0)
	{
		if(Gimbal_Control_t.Patrol_Mode_Pitch_Direction==0) Gimbal_Control_t.GimBla_Pitch_Angle += PITCH_PATROL_SEN*TASK_EXECUTION_CYCLE;//向上
		if(Gimbal_Control_t.Patrol_Mode_Pitch_Direction==1) Gimbal_Control_t.GimBla_Pitch_Angle -= PITCH_PATROL_SEN*TASK_EXECUTION_CYCLE;//向下
		if(Gimbal_Control_t.GimBla_Pitch_Angle>PITCH_UP_LIMIT) Gimbal_Control_t.Patrol_Mode_Pitch_Direction=1;//方向变为向下
		if(Gimbal_Control_t.GimBla_Pitch_Angle<PITCH_DOWN_LIMIT) Gimbal_Control_t.Patrol_Mode_Pitch_Direction=0;//方向变为向上
	}
	#endif
	Gimbal_Pitch_Action(Gimbal_Control_t.GimBla_Pitch_Angle);
	
	
	/** @function 摩擦轮开关 ----- */
	
	//开摩擦轮
	if(rc_data.rc.s1==2)
	{
		Snail_Shoot(RATE_OF_FIRE);
		Gimbal_Control_t.Friction_Start_Flag=1;
	}
	//关摩擦轮
	else
	{
		Snail_Shoot(MIN_INTERVAL);
		Gimbal_Control_t.Friction_Start_Flag=0;
	}
	
	
	/** @function 火控 ----- */
	
	//计算射频
	float Shoot1_Frequency,Shoot2_Frequency;
	#if (SHOOT1_SWITCH==1)
		Shoot1_Frequency = SHOOT_FREQUENCY;
	#endif
	#if (SHOOT2_SWITCH==1)
		Shoot2_Frequency = SHOOT_FREQUENCY;
	#endif
	
	//拨弹盘驱动
	if(Gimbal_Control_t.Friction_Start_Flag==1 && Shoot_Flag==1)
	{
		Shoot_Speed_Action(-Shoot1_Frequency ,Shoot2_Frequency);
	}
	else
	{
		Shoot_Speed_Action(0,0);
	}
}
