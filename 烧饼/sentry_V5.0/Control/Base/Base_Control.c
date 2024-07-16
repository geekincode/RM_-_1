/**
  ******************************************************************************
  * Description : 这是一坨遥控器底盘控制的代码
  *  
  ******************************************************************************
  * @attention	
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- 全局变量 -------------------- */

//底盘控制结构体
Base_Control_Struct Base_Control_t;

//键鼠标志位结构体
KeyMouse_Flag KM_Flag_t;



/* -------------------- 函数声明 -------------------- */

//遥控器底盘控制函数声明
void Base_Remote_Control(void);
//上位机底盘控制函数声明
void Base_PC_Control(void);



/* -------------------- 底盘控制逻辑 -------------------- */

/**
  * @brief  底盘任务入口
  * @param  None
  * @retval None
*/
void Base_thread_entry(void *parameter)
{
	while(1)
	{
		/** @function 执行周期计数 ----- */

		#if(EXECUTION_COUNT_SWITCH==1)
			Execution_Count_t.Base_Execution_Count++;
		#endif
		
		
		/** @function 检测待机模式 ----- */
		
		#if (STANDBY_SWITCH==1)							//左上角的拨杆1，2，3挡位
		if(rc_data.rc.s1==1)
		{
			Turn_On_Standby_Mode();
		}
		#endif
		
		/** @function 检测控制模式 ----- */
		
		//遥控器控制
		if(rc_data.rc.s2!=2)
		{
			//遥控器控制逻辑
			///Kinematic_Solving_t.Z_Speed = 0;
			Base_Remote_Control();
			///Kinematic_Solving_t.Z_Speed = 0;
		}
		
		//上位机控制
		if(rc_data.rc.s2==2)
		{
			//上位机控制逻辑
			Base_PC_Control();
		}
		
		/** @function 运动学解析 ----- */

		Move_Speed_Transform();
		
		
		/** @function 电机驱动 ----- */
		
		Base_MotorAll_SetSpeed(Motor_Current_t.Base_Motor1_Driver_Current,Motor_Current_t.Base_Motor2_Driver_Current,
								Motor_Current_t.Base_Motor3_Driver_Current,Motor_Current_t.Base_Motor4_Driver_Current);
		
		
		/** @function 任务延时 ----- */
		
		//执行频率
		rt_thread_mdelay(TASK_EXECUTION_CYCLE);
	}
}


/**
  * @brief  遥控器底盘控制(x前正后负，y左正右负)
  * @param  None
  * @retval None
*/
void Base_Remote_Control(void)
{
	/** @function 前后移动摇杆 ----- */

	Kinematic_Solving_t.Base_X_Speed = rc_data.rc.ch3*RC_BASE_MOVE_SENS;
	
	/** @function 左右移动摇杆 ----- */

	Kinematic_Solving_t.Base_Y_Speed = rc_data.rc.ch2*RC_BASE_MOVE_SENS;
		
	/** @function 小陀螺旋转 ----- */
	
	if(rc_data.rc.s2==1)
	{
		//小陀螺启动
		Remote_Gyro_Start();
		
		//小陀螺旋转
		Kinematic_Solving_t.Z_Speed = Base_Control_t.Remote_Gyro_Target_Speed;
		
		//小陀螺开始旋转
		Base_Control_t.Gyro_Rotating_Flag=1;
	}
	//小陀螺停止
	else if(Base_Control_t.Remote_Gyro_Target_Speed>0)
	{
		//小陀螺停止
		Remote_Gyro_Stop();
		
		//小陀螺旋转
		Kinematic_Solving_t.Z_Speed = Base_Control_t.Remote_Gyro_Target_Speed;
	}
	
	/** @function 底盘跟随云台 ----- */
	
	if(rc_data.rc.s2!=1 && Base_Control_t.Gyro_Rotating_Flag==0)
	{
		Base_Follow_Gimbal_Action(Base_Control_t.Original_Forward_Direction);
	}
}


/**
  * @brief  上位机底盘控制
  * @param  None
  * @retval None
*/
void Base_PC_Control(void)
{
	/** @function 小陀螺旋转 ----- */
	
	//小陀螺启动
	Remote_Gyro_Start();
	
	//小陀螺旋转
	Kinematic_Solving_t.Z_Speed = Base_Control_t.Remote_Gyro_Target_Speed;
	
	//小陀螺开始旋转
	Base_Control_t.Gyro_Rotating_Flag=1;
}
