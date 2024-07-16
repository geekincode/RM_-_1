/**
  ******************************************************************************
  * Description : 全局仓库
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#ifndef __SHARED_WAREHOUSE_H
#define __SHARED_WAREHOUSE_H


/* -------------------- 头文件 -------------------- */

#include "stm32f4xx.h"
#include <rtthread.h>           				
#include <stdio.h>                  
#include <stdarg.h>    
#include "arm_math.h"				// DSP
#include "Function_Config.h"		// 模式相关配置文件
#include "Formula_Config.h"			// 公式相关配置文件
#include "MyCAN.h"					// CAN发送和接收
#include "Motor.h"					// 电机驱动
#include "Encoder.h"				// 电机回传数据
#include "Function_Config.h"		// 模式相关配置文件
#include "Formula_Config.h"			// 公式相关配置文件
#include "PWM.h"					// PWM设置
#include "Task_Control.h"			// rtthread任务控制
#include "Robot_Init.h"				// 机器人初始化
#include "Usart.h"					// 上位机串口初始化
#include "Base_Timer.h"				// 定时器初始化
#include "remote.h"					// 遥控器接收
#include "Filter.h"					// 滤波器公式
#include "PID.h"					// PID公式
#include "Sampling.h"				// 数据处理
#include "Base_Control.h"			// 底盘控制
#include "Base_APP.h"				// 底盘控制
#include "Gimbal_Control.h"			// 云台控制
#include "Gimbal_APP.h"				// 云台控制
#include "Detect.h"					// 整机检测
#include "PC_Communicate.h"			// 上位机通信
#include "AiMBot_Config.h"			// 自瞄配置
#include "Power_Limit.h"			// 底盘功率限制
#include "Power_Limit_Config.h"		// 底盘功率限制配置文件
#include "Judge.h"					// 裁判系统解析
#include "IMU.h"					// 陀螺仪姿态解算
#include "PWM.h"					// snail驱动
#include "Judge_crc.h"
#include "Judge_Task.h"



/* -------------------- 全局变量仓库 -------------------- */

/*  CAN12接收的原始数据包  */
extern CanRxMsg MyCAN1_Rece_Data;
extern CanRxMsg MyCAN2_Rece_Data;

/*  遥控器接收数据  */
extern RC_Data_Struct rc_data;

/*  执行周期检测结构体  */
extern Execution_Count_Struct Execution_Count_t;

/*  一阶低通滤波结构体  */
extern LowPass_Struct Wheel1_Low_Pass_t;						//底盘电机滤波结构体
extern LowPass_Struct Wheel2_Low_Pass_t;
extern LowPass_Struct Wheel3_Low_Pass_t;
extern LowPass_Struct Wheel4_Low_Pass_t;
extern LowPass_Struct Yaw_Speed_Low_Pass_t;						//Yaw轴Pitch轴电机滤波结构体
extern LowPass_Struct Pitch_Speed_Low_Pass_t;
extern LowPass_Struct Yaw_Angle_Low_Pass_t;						//Yaw轴Pitch轴电机角度滤波结构体
extern LowPass_Struct Pitch_Angle_Low_Pass_t;
extern LowPass_Struct Friction1_Speed_Low_Pass_t;				//摩擦轮电机速度滤波结构体
extern LowPass_Struct Friction2_Speed_Low_Pass_t;
extern LowPass_Struct Shoot_Speed_Low_Pass_t;					//拨弹盘电机速度滤波结构体
extern LowPass_Struct Shoot_Angle_Low_Pass_t;					//拨弹盘电机角度滤波结构体

/*  电机返回原始数据  */
extern DJI_Motor_Data_Struct BASE_MOTOR1_Data_t;				//底盘电机返回值
extern DJI_Motor_Data_Struct BASE_MOTOR2_Data_t;
extern DJI_Motor_Data_Struct BASE_MOTOR3_Data_t;
extern DJI_Motor_Data_Struct BASE_MOTOR4_Data_t;
extern DJI_Motor_Data_Struct SHOOT_LEFT_MOTOR_Data_t;			//拨弹盘摩擦轮电机返回值
extern DJI_Motor_Data_Struct SHOOT_RIGHT_MOTOR_Data_t;
extern DJI_Motor_Data_Struct YAW_MOTOR_Data_t;					//Yaw轴Pitch轴电机返回值
extern DJI_Motor_Data_Struct PITCH_MOTOR_Data_t;	

/*  PID结构体  */
extern PID_Struct Wheel1_Speed_t;								//轮子结构体
extern PID_Struct Wheel2_Speed_t;
extern PID_Struct Wheel3_Speed_t;
extern PID_Struct Wheel4_Speed_t;
extern PID_Struct Base_Follow_Gimbal_t;							//底盘跟随云台结构体
extern PID_Struct Yaw_Motor_Speed_t;							//Yaw轴角度速度环结构体(IMU绝对角度)
extern PID_Struct Yaw_IMU_Angle_t;
extern PID_Struct Pitch_Motor_Speed_t;							//Pitch轴角度速度环结构体(IMU绝对角度)
extern PID_Struct Pitch_Motor_Angle_t;
extern PID_Struct Shoot1_Speed_t;								//拨弹盘PID结构体
extern PID_Struct Shoot2_Speed_t;

/*  数据处理  */
extern Motor_Filiter_Data_Struct Motor_Filiter_Data_t;			//电机滤波数据结构体
extern Yaw_Motor_Data_Handle_Struct Yaw_Motor_Data_Handle_t;	//Yaw轴数据处理结构体

/*  电机驱动电流  */
extern Motor_Current_Struct Motor_Current_t;

/*  控制结构体  */
extern Kinematic_Solving_Struct Kinematic_Solving_t;			//底盘运动解析结构体
extern Base_Control_Struct Base_Control_t;						//底盘控制结构体
extern Gimbal_Control_Struct Gimbal_Control_t;					//云台控制结构体

/*  自瞄变量  */
extern uint8_t Shoot_Flag;										//开火标志位

/*  错误检测  */
extern Error_Detect_Struct Error_Detect_t;						//错误检测结构体


//键鼠标志位结构体
extern KeyMouse_Flag KM_Flag_t;

//DMA接收原始数据
//extern uint8_t judge_buf[2][220] = {0};
//裁判系统数据结构体
extern judge_t Judge;
extern judge_get_data_t Judge_Data;




#endif
