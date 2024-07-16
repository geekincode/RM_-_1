/**
  ******************************************************************************
  * Description : 这是一坨机器人初始化的代码
  *  
  ******************************************************************************
  * @attention	
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"


/* -------------------- 全局变量 -------------------- */

//功能初始化结构体
Robot_Init_Struct Robot_Init_t;



/* -------------------- 初始化 -------------------- */


/**
  * @brief  外设初始化
  * @param  None
  * @retval None
*/
void Peripheral_Init(void)
{	
	//定时器初始化
	Base_Timer_Init();
	
	//低通滤波初始化
	Low_Pass_Filter_Init();
	
	//PID初始化
	PID_All_Init();	
	
	//CAN初始化
	MyCAN_Init();	
	
	//遥控器初始化
	Remote_Init();	
	
	//检测遥控器数据
	while(Error_Detect_t.RC_Connected_Flag==0);//等待遥控器收到完整数据包
	Timer_Delay_ms(200);
	Remote_Data_Detect();
	
	//底盘功率限制初始化
	//Power_Limit_Init();
	
	//串口初始化
	Serial_Init();
	
	//低通滤波器初始化
	Low_Pass_Filter_Init();

	//PWM初始化
	PWM_Init();
	
	//陀螺仪初始化
	IMU_Init();
}


/**
  * @brief  功能初始化任务入口
  * @param  None
  * @retval None
*/
void Fun_Init_thread_entry(void *parameter)
{
	//snial行程校准
	Snail_Init();
	
	//Pitch轴初始角度
	Gimbal_Control_t.GimBla_Pitch_Angle = PITCH_INIT_ANGLE;
	
	while(1)
	{
		if(rc_data.rc.s1==1)
		{
			//开始前向角校准
			Robot_Init_t.Yaw_Angle_Out_Start_Flag=1;
			//控制转向
			Gimbal_Control_t.GimBla_Yaw_Angle -= (float)(rc_data.rc.ch0)*RC_YAW_TURN_SENS*TASK_EXECUTION_CYCLE;
			
			//Yaw轴PID计算
			Robot_Init_t.Angle_Result = Pos_PID(&Yaw_IMU_Angle_t,Gimbal_Control_t.GimBla_Yaw_Angle,INS.YawTotalAngle);
			Robot_Init_t.Speed_Result = Pos_PID(&Yaw_Motor_Speed_t,Robot_Init_t.Angle_Result,YAW_MOTOR_Data_t.Speed);
			//输出
			Yaw_Motor_SetSpeed(Robot_Init_t.Speed_Result);
			
			//Pitch轴PID计算
			Gimbal_Control_t.Pitch_Angle_Result = Pos_PID(&Pitch_Motor_Angle_t,Gimbal_Control_t.GimBla_Pitch_Angle,PITCH_MOTOR_Data_t.Angle);
			Motor_Current_t.Pitch_Motor_Driver_Current = Pos_PID(&Pitch_Motor_Speed_t,Gimbal_Control_t.Pitch_Angle_Result,PITCH_MOTOR_Data_t.Speed);
			//输出
			Pitch_Motor_SetSpeed(Motor_Current_t.Pitch_Motor_Driver_Current);
		}
		
		//前向角校准完成
		if(rc_data.rc.s1==3 && Robot_Init_t.Yaw_Angle_Out_Start_Flag==1)
		{
			//前向角初始值
			Base_Control_t.Original_Forward_Direction = YAW_MOTOR_Data_t.Angle;
			
			if(Base_Control_t.Original_Forward_Direction<2730)//特殊处理情况1
			{
				Yaw_Motor_Data_Handle_t.Yaw_Angle_Data_Special_Flag=1;
				Base_Control_t.Original_Forward_Direction = Base_Control_t.Original_Forward_Direction+2730;
			}
			if(Base_Control_t.Original_Forward_Direction>5460)//特殊处理情况2
			{
				Yaw_Motor_Data_Handle_t.Yaw_Angle_Data_Special_Flag=2;
				Base_Control_t.Original_Forward_Direction = Base_Control_t.Original_Forward_Direction-2730; 
			}
			
			//启动主线程
			Start_Thread();
			
			//退出前向角校准
			return;
		}
		
		//系统延时
		rt_thread_mdelay(TASK_EXECUTION_CYCLE);
	}
}
