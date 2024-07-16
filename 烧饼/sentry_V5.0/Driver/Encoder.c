/**
  ******************************************************************************
  * Description : 这是一坨接收电机返回数据的代码
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"


/* -------------------- 全局变量 -------------------- */

//底盘电机返回值
DJI_Motor_Data_Struct BASE_MOTOR1_Data_t;				///3508和6020电机都使用DJI_Motor_Data_Struct结构体来实例化
DJI_Motor_Data_Struct BASE_MOTOR2_Data_t;
DJI_Motor_Data_Struct BASE_MOTOR3_Data_t;
DJI_Motor_Data_Struct BASE_MOTOR4_Data_t;

//左右拨弹盘电机返回值
DJI_Motor_Data_Struct SHOOT_LEFT_MOTOR_Data_t;
DJI_Motor_Data_Struct SHOOT_RIGHT_MOTOR_Data_t;

//Yaw轴Pitch轴电机返回值
DJI_Motor_Data_Struct YAW_MOTOR_Data_t;
DJI_Motor_Data_Struct PITCH_MOTOR_Data_t;

//Yaw轴数据处理结构体
Yaw_Motor_Data_Handle_Struct Yaw_Motor_Data_Handle_t;



/* -------------------- 数据特殊处理 -------------------- */

/**
  * @brief  Yaw轴角度数据特殊处理
  * @param  None
  * @retval None
*/
void Yaw_Angle_Data_Special_Handling(void)
{
	//Yaw轴角度原始数据展开
	if(YAW_MOTOR_Data_t.Angle - Yaw_Motor_Data_Handle_t.YAW_Last_Data > 4095)//8191->0
	{
		Yaw_Motor_Data_Handle_t.YAW_Rotation_Number--;
	}
	if(YAW_MOTOR_Data_t.Angle - Yaw_Motor_Data_Handle_t.YAW_Last_Data < -4095)//0->8191
	{
		Yaw_Motor_Data_Handle_t.YAW_Rotation_Number++;
	}
	Yaw_Motor_Data_Handle_t.YAW_Expand_Data = YAW_MOTOR_Data_t.Angle + Yaw_Motor_Data_Handle_t.YAW_Rotation_Number*8191;
	Yaw_Motor_Data_Handle_t.YAW_Last_Data = YAW_MOTOR_Data_t.Angle;
	
	//云台真实数据
	Yaw_Motor_Data_Handle_t.Yaw_Real_Expand_Data = (float)Yaw_Motor_Data_Handle_t.YAW_Expand_Data/3*2;
	//云台真实圈数
	Yaw_Motor_Data_Handle_t.Yaw_Real_Rotation_Number = (int16_t)(Yaw_Motor_Data_Handle_t.Yaw_Real_Expand_Data/8191);
	
	//Yaw角度3:2赋值
	YAW_MOTOR_Data_t.Angle = (int16_t)Yaw_Motor_Data_Handle_t.Yaw_Real_Expand_Data - Yaw_Motor_Data_Handle_t.Yaw_Real_Rotation_Number*8191;
	if(YAW_MOTOR_Data_t.Angle<0){YAW_MOTOR_Data_t.Angle=8191+YAW_MOTOR_Data_t.Angle;}
	
	//数据特殊处理
	if(Yaw_Motor_Data_Handle_t.Yaw_Angle_Data_Special_Flag==1)//<120
	{
		YAW_MOTOR_Data_t.Angle = YAW_MOTOR_Data_t.Angle+2730;
		if(YAW_MOTOR_Data_t.Angle>8191)
		{
			YAW_MOTOR_Data_t.Angle = YAW_MOTOR_Data_t.Angle-8191;
		}
	}
	else if(Yaw_Motor_Data_Handle_t.Yaw_Angle_Data_Special_Flag==2)//>240
	{
		YAW_MOTOR_Data_t.Angle = YAW_MOTOR_Data_t.Angle-2730;
		if(YAW_MOTOR_Data_t.Angle<0)
		{
			YAW_MOTOR_Data_t.Angle = 8191+YAW_MOTOR_Data_t.Angle;
		}
	}
}



/* -------------------- 接收底盘3508和Yaw轴6020电机返回值 -------------------- */

/**
  * @brief  接收返回值(角度=原始数据/8191*360)
  * @param  None
  * @retval None
*/
void Get_Base_Encode_Data(void)
{
	switch(MyCAN1_Rece_Data.StdId)
	{
		case BASE_MOTOR1_ID:							/// 底盘轮子1的3508电机
			BASE_MOTOR1_Data_t.Angle=(MyCAN1_Rece_Data.Data[0]<<8)+MyCAN1_Rece_Data.Data[1];//机械转子角度
			BASE_MOTOR1_Data_t.Speed=(MyCAN1_Rece_Data.Data[2]<<8)+MyCAN1_Rece_Data.Data[3];//转速
			BASE_MOTOR1_Data_t.Current=(MyCAN1_Rece_Data.Data[4]<<8)+MyCAN1_Rece_Data.Data[5];//电流
			BASE_MOTOR1_Data_t.Temperature=MyCAN1_Rece_Data.Data[6];//电机温度
			Error_Detect_t.BASE_MOTOR1_Connected_Flag=1;//置标志位
			break;
		
		case BASE_MOTOR2_ID:							/// 底盘轮子2的3508电机
			BASE_MOTOR2_Data_t.Angle=(MyCAN1_Rece_Data.Data[0]<<8)+MyCAN1_Rece_Data.Data[1];//机械转子角度
			BASE_MOTOR2_Data_t.Speed=(MyCAN1_Rece_Data.Data[2]<<8)+MyCAN1_Rece_Data.Data[3];//转速
			BASE_MOTOR2_Data_t.Current=(MyCAN1_Rece_Data.Data[4]<<8)+MyCAN1_Rece_Data.Data[5];//电流
			BASE_MOTOR2_Data_t.Temperature=MyCAN1_Rece_Data.Data[6];//电机温度
			Error_Detect_t.BASE_MOTOR2_Connected_Flag=1;//置标志位
			break;
		
		case BASE_MOTOR3_ID:							/// 底盘轮子3的3508电机
			BASE_MOTOR3_Data_t.Angle=(MyCAN1_Rece_Data.Data[0]<<8)+MyCAN1_Rece_Data.Data[1];//机械转子角度
			BASE_MOTOR3_Data_t.Speed=(MyCAN1_Rece_Data.Data[2]<<8)+MyCAN1_Rece_Data.Data[3];//转速
			BASE_MOTOR3_Data_t.Current=(MyCAN1_Rece_Data.Data[4]<<8)+MyCAN1_Rece_Data.Data[5];//电流
			BASE_MOTOR3_Data_t.Temperature=MyCAN1_Rece_Data.Data[6];//电机温度
			Error_Detect_t.BASE_MOTOR3_Connected_Flag=1;//置标志位
			break;
		
		case BASE_MOTOR4_ID:							/// 底盘轮子4的3508电机
			BASE_MOTOR4_Data_t.Angle=(MyCAN1_Rece_Data.Data[0]<<8)+MyCAN1_Rece_Data.Data[1];//机械转子角度
			BASE_MOTOR4_Data_t.Speed=(MyCAN1_Rece_Data.Data[2]<<8)+MyCAN1_Rece_Data.Data[3];//转速
			BASE_MOTOR4_Data_t.Current=(MyCAN1_Rece_Data.Data[4]<<8)+MyCAN1_Rece_Data.Data[5];//电流
			BASE_MOTOR4_Data_t.Temperature=MyCAN1_Rece_Data.Data[6];//电机温度
			Error_Detect_t.BASE_MOTOR4_Connected_Flag=1;//置标志位
			break;
		
		case YAW_MOTOR_ID:								///Yaw轴小陀螺旋转的6020电机
			YAW_MOTOR_Data_t.Angle=(MyCAN1_Rece_Data.Data[0]<<8)+MyCAN1_Rece_Data.Data[1];//机械转子角度
			YAW_MOTOR_Data_t.Speed=(MyCAN1_Rece_Data.Data[2]<<8)+MyCAN1_Rece_Data.Data[3];//转速
			YAW_MOTOR_Data_t.Current=(MyCAN1_Rece_Data.Data[4]<<8)+MyCAN1_Rece_Data.Data[5];//电流
			YAW_MOTOR_Data_t.Temperature=MyCAN1_Rece_Data.Data[6];//电机温度
			//Yaw轴角度数据特殊处理
			Yaw_Angle_Data_Special_Handling();
			Error_Detect_t.YAW_MOTOR_Connected_Flag=1;//置标志位
		/*
		case ROBOT_STATE_ID:
			Judge_t.Chassis_power=(float)((MyCAN1_Rece_Data.Data[0]<<8)+MyCAN1_Rece_Data.Data[1])/100;//底盘功率
			Judge_t.Chassis_volt=(MyCAN1_Rece_Data.Data[2]<<8)+MyCAN1_Rece_Data.Data[3];//底盘电压
			Judge_t.Chassis_power_buffer=(MyCAN1_Rece_Data.Data[4]<<8)+MyCAN1_Rece_Data.Data[5];//底盘能量缓冲
			Judge_t.Shooter_heat=(MyCAN1_Rece_Data.Data[6]<<8)+MyCAN1_Rece_Data.Data[7];//枪口热量
			Error_Detect_t.Judge_Connected_Flag=1;//置标志位
			break;
		
		case ROBOT_RULES_ID:
			Judge_t.Chassis_power_limit=(MyCAN1_Rece_Data.Data[0]<<8)+MyCAN1_Rece_Data.Data[1];//底盘功率上限
			Judge_t.Shooter_speed_limit=(MyCAN1_Rece_Data.Data[2]<<8)+MyCAN1_Rece_Data.Data[3];//枪口射速上限
			Judge_t.Shooter_cooling_limit=(MyCAN1_Rece_Data.Data[4]<<8)+MyCAN1_Rece_Data.Data[5];//枪口热量上限
			Judge_t.Shooter_cooling_rate=(MyCAN1_Rece_Data.Data[6]<<8)+MyCAN1_Rece_Data.Data[7];//枪口热量每秒冷却值
			Error_Detect_t.Judge_Connected_Flag=1;//置标志位
			break;*/

	}
}


/* -------------------- 接收云台3508和2006和Pitch轴6020电机返回值 -------------------- */

/**
  * @brief  接收返回值(角度=原始数据/8191*360)
  * @param  None
  * @retval None
*/
void Get_Gimbal_Encode_Data(void)
{
	switch(MyCAN2_Rece_Data.StdId)
	{		
		case SHOOT_LEFT_MOTOR_ID:							///左摩擦轮电机
			SHOOT_LEFT_MOTOR_Data_t.Angle=(MyCAN2_Rece_Data.Data[0]<<8)+MyCAN2_Rece_Data.Data[1];//机械转子角度
			SHOOT_LEFT_MOTOR_Data_t.Speed=(MyCAN2_Rece_Data.Data[2]<<8)+MyCAN2_Rece_Data.Data[3];//转速
			SHOOT_LEFT_MOTOR_Data_t.Current=(MyCAN2_Rece_Data.Data[4]<<8)+MyCAN2_Rece_Data.Data[5];//电流
			SHOOT_LEFT_MOTOR_Data_t.Temperature=MyCAN2_Rece_Data.Data[6];//电机温度
			Error_Detect_t.SHOOT_LEFT_MOTOR_Connected_Flag=1;//置标志位
			break;
		
		case SHOOT_RIGHT_MOTOR_ID:							///右摩擦轮电机
			SHOOT_RIGHT_MOTOR_Data_t.Angle=(MyCAN2_Rece_Data.Data[0]<<8)+MyCAN2_Rece_Data.Data[1];//机械转子角度
			SHOOT_RIGHT_MOTOR_Data_t.Speed=(MyCAN2_Rece_Data.Data[2]<<8)+MyCAN2_Rece_Data.Data[3];//转速
			SHOOT_RIGHT_MOTOR_Data_t.Current=(MyCAN2_Rece_Data.Data[4]<<8)+MyCAN2_Rece_Data.Data[5];//电流
			SHOOT_RIGHT_MOTOR_Data_t.Temperature=MyCAN2_Rece_Data.Data[6];//电机温度
			Error_Detect_t.SHOOT_RIGHT_MOTOR_Connected_Flag=1;//置标志位
			break;
		
		case PITCH_MOTOR_ID:								///Pitch轴上下移动6020电机
			PITCH_MOTOR_Data_t.Angle=(MyCAN2_Rece_Data.Data[0]<<8)+MyCAN2_Rece_Data.Data[1];//机械转子角度
			PITCH_MOTOR_Data_t.Speed=(MyCAN2_Rece_Data.Data[2]<<8)+MyCAN2_Rece_Data.Data[3];//转速
			PITCH_MOTOR_Data_t.Current=(MyCAN2_Rece_Data.Data[4]<<8)+MyCAN2_Rece_Data.Data[5];//电流
			PITCH_MOTOR_Data_t.Temperature=MyCAN2_Rece_Data.Data[6];//电机温度
			Error_Detect_t.PITCH_MOTOR_Connected_Flag=1;//置标志位
			break;
	}
}
