/**
  ******************************************************************************
  * Description : 功能配置文件
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#ifndef __FUNCTION_CONFIG_H
#define __FUNCTION_CONFIG_H



/********************************************************************
							功能开关  
**********************************************************************/

/**
  * @attention
  *
  *	
*/

//执行周期计数器开关
#define EXECUTION_COUNT_SWITCH			1
//待机模式开关
#define STANDBY_SWITCH					1
//云台Yaw轴是否开启调试模式
#define YAW_DEBUGGING_MODE				0
//是否自动巡检
#define AUTOMATIC_INSPECTION_SWITCH		1
//是否开启遥控器掉线保护
#define RC_DISCONNECT_SWITCH			0
//是否开启电机掉线保护
#define MOTOR_DISCONNECT_SWITCH			0
//普通模式是否使能Pitch轴
#define NORMAL_PITCH_SWITCH				0



/********************************************************************
							执行频率  
**********************************************************************/

/**
  * @attention
  *
  *	
*/

//任务执行周期(ms)
#define TASK_EXECUTION_CYCLE			2
//错误检测执行周期(ms)
#define DETECT_EXECUTION_CYCLE			1000



/********************************************************************
							底盘功能参数  
**********************************************************************/

/**
  * @attention
  *
  *	
*/

//遥控器底盘平移灵敏度
#define RC_BASE_MOVE_SENS				8
//小陀螺平移方向补偿(6000/15)
#define GYRO_DIRECTION_COMPENSATION 	400
//遥控器模式小陀螺速度
#define RC_GYRO_MAX_SPEED				4500
//遥控器小陀螺启动加速步长
#define RC_GYRO_START_STEP_SIZE			30
//遥控器小陀螺停止减速步长
#define RC_GYRO_STOP_STEP_SIZE			30
//遥控器小陀螺停止最小速度
#define RC_GYRO_STOP_MIN_SPEED			4000
//遥控器小陀螺停止最小角度差
#define RC_GYRO_STOP_MIN_ANGLE			30



/********************************************************************
							云台功能参数  
**********************************************************************/

/**
  * @attention
  *
  *	
*/

//遥控器云台Yaw轴转向灵敏度
#define RC_YAW_TURN_SENS				0.0004
//遥控器Pitch轴灵敏度568
#define RC_PITCH_SENS					1200
//Pitch轴上限角度(6020角度)6500
#define PITCH_UP_LIMIT					8000
//Pitch轴下限角度(6020角度)5100
#define PITCH_DOWN_LIMIT				2000
//Pitch轴水平角度(6020角度)
#define PITCH_INIT_ANGLE				6000
//Yaw轴巡检模式灵敏度
#define YAW_PATROL_SEN					0.08
//Pitch轴巡检模式灵敏度
#define PITCH_PATROL_SEN				2.5



/********************************************************************
							火控功能参数  
**********************************************************************/

/**
  * @attention
  *
  *	
*/

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



/********************************************************************
						  软件任务系统优先级配置  
**********************************************************************/

/**
  * @attention
  *
  *	数越小优先级越高
*/

//功能初始化任务优先级
#define FUN_INIT_PRIORITY				0
//启动主线程任务优先级
#define START_MAIN_PRIORITY				1
//开启待机模式任务优先级
#define TURN_ON_STANDBY_PRIORITY		2
//数据处理任务优先级
#define SAMPLING_PRIORITY				3
//整机检测任务优先级
#define INSPECTION_PRIORITY				4
//零飘校准任务优先级
#define IMU_OFFSET_PRIORITY				4
//上位机通信任务优先级
#define PC_COMMUNICATE_PRIORITY			5
//底盘任务优先级
#define BASE_PRIORITY					5
//云台任务优先级
#define GIMBAL_PRIORITY					5



/********************************************************************
							  电机开关  
**********************************************************************/

/**
  * @attention
  *
  *	可配置是否关闭所有或某个电机
*/

//电机总开关
#define	MOTOR_SWITCH					1
//底盘四个3508开关
#define	BASE_MOTOR_SWITCH				1
//YAW轴6020开关
#define	YAW_MOTOR_SWITCH				1
//PITCH轴6020开关
#define	PITCH_MOTOR_SWITCH				1
//拨弹盘和摩擦轮开关
#define SHOOT_MOTOR_SWITCH				1



#endif
