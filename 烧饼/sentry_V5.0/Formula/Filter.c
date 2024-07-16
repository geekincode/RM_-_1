/**
  ******************************************************************************
  * Description : 这是一坨滤波器的代码
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- 一阶低通滤波结构体 -------------------- */

//底盘电机滤波结构体
LowPass_Struct Wheel1_Low_Pass_t;
LowPass_Struct Wheel2_Low_Pass_t;
LowPass_Struct Wheel3_Low_Pass_t;
LowPass_Struct Wheel4_Low_Pass_t;
//Yaw轴Pitch轴电机速度滤波结构体
LowPass_Struct Yaw_Speed_Low_Pass_t;
LowPass_Struct Pitch_Speed_Low_Pass_t;
//Yaw轴Pitch轴电机角度滤波结构体
LowPass_Struct Yaw_Angle_Low_Pass_t;
LowPass_Struct Pitch_Angle_Low_Pass_t;
//摩擦轮电机速度滤波结构体
LowPass_Struct Friction1_Speed_Low_Pass_t;
LowPass_Struct Friction2_Speed_Low_Pass_t;
//拨弹盘电机速度滤波结构体
LowPass_Struct Shoot_Speed_Low_Pass_t;
//拨弹盘电机角度滤波结构体
LowPass_Struct Shoot_Angle_Low_Pass_t;



/* -------------------- 一阶低通滤波 -------------------- */

/**
  * @brief  一阶低通滤波公式
  * @param  滤波结构体filter ，本次采样数据num
  * @retval 滤波结果
*/
float Low_Pass_Filter(LowPass_Struct *filter,float num)
{
	//滤波公式
	filter->OutPut = filter->KL * num + (1 - filter->KL) * filter->Last_num; 				///Y(n)=kX(n)+(1-k)Y(n-1)
	
	//赋值上次采样值
	filter->Last_num = filter->OutPut;

	//输出
	return filter->OutPut;
}

/**
  * @brief  一阶低通滤波初始化
  * @param  None
  * @retval None
*/
void Low_Pass_Filter_Init(void)
{
	Wheel1_Low_Pass_t.KL = Wheel2_Low_Pass_t.KL = Wheel3_Low_Pass_t.KL = Wheel4_Low_Pass_t.KL = WHEEL_KL;
	Yaw_Speed_Low_Pass_t.KL = YAW_SPEED_KL;
	Pitch_Speed_Low_Pass_t.KL = PITCH_SPEED_KL;
	Yaw_Angle_Low_Pass_t.KL = YAW_ANGLE_KL;
	Pitch_Angle_Low_Pass_t.KL = PITCH_ANGLE_KL;
	Friction1_Speed_Low_Pass_t.KL = Friction2_Speed_Low_Pass_t.KL = FRICTION_SPEED_KL;
	Shoot_Speed_Low_Pass_t.KL = SHOOT_SPEED_KL;
	Shoot_Angle_Low_Pass_t.KL = SHOOT_ANGLE_KL;
}

/**
  * @brief  一阶低通滤波清零
  * @param  滤波结构体
  * @retval None
*/
void Low_Pass_Filter_Clear(LowPass_Struct *filter)
{
	filter->Last_num=0;
}
