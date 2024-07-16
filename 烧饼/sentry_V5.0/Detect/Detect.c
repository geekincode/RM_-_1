/**
  ******************************************************************************
  * Description : 这是一坨整机检测的代码
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- 全局变量 -------------------- */

//错误检测结构体
Error_Detect_Struct Error_Detect_t;



/* -------------------- 整机检测任务入口函数 -------------------- */

/**
  * @brief  整机检测任务入口函数
  * @param  None
  * @retval None
*/
void Inspection_thread_entry(void *parameter)
{
	while(1)
	{
		//执行周期计数
		#if(EXECUTION_COUNT_SWITCH==1)
			Execution_Count_t.Detect_Execution_Count++;
		#endif
	
		//检测错误
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
		
		//重置标志位
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
		
		//电机掉线保护
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
				//置循环检测标志位
				Error_Detect_t.Motor_Cycle_Detection_Flag=1;
			}
			else
			{
				//重置循环检测标志位
				Error_Detect_t.Motor_Cycle_Detection_Flag=0;
			}
		#endif
		//遥控器掉线保护	
		#if (RC_DISCONNECT_SWITCH==1)
			if(Error_Detect_t.RC_Disconnected_Flag==1)
			{
				//置循环检测标志位
				Error_Detect_t.RC_Cycle_Detection_Flag=1;
			}
			else
			{
				//重置循环检测标志位
				Error_Detect_t.RC_Cycle_Detection_Flag=0;
			}
		#endif
		
		//错误检测执行周期
		if(Error_Detect_t.RC_Cycle_Detection_Flag==0 && Error_Detect_t.Motor_Cycle_Detection_Flag==0) rt_thread_mdelay(DETECT_EXECUTION_CYCLE);
	}
}


/**
  * @brief  检测遥控器数据
  * @param  None
  * @retval None
*/
void Remote_Data_Detect(void)
{
	//遥控器数据错误检测
	if( rc_data.rc.ch0>660 || rc_data.rc.ch0<-660 ||
		rc_data.rc.ch0>660 || rc_data.rc.ch0<-660 ||
		rc_data.rc.ch0>660 || rc_data.rc.ch0<-660 ||
		rc_data.rc.ch0>660 || rc_data.rc.ch0<-660 )
	{
		//置错误标志位
		Error_Detect_t.RC_Data_Error_Flag = 1;
		
		while ( rc_data.rc.ch0>660 || rc_data.rc.ch0<-660 ||
				rc_data.rc.ch0>660 || rc_data.rc.ch0<-660 ||
				rc_data.rc.ch0>660 || rc_data.rc.ch0<-660 ||
				rc_data.rc.ch0>660 || rc_data.rc.ch0<-660 )
		{
			//重新初始化遥控器
			Remote_Init();
			//等待遥控器收到完整数据包
			Error_Detect_t.RC_Connected_Flag=0;
			while(Error_Detect_t.RC_Connected_Flag==0);
			Timer_Delay_ms(200);
		}
		
		//解除错误标志位
		Error_Detect_t.RC_Data_Error_Flag = 0;
		
	}
}
