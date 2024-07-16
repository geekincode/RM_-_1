#ifndef __ENCODER_H
#define __ENCODER_H

//大疆电机回传数据结构体
typedef struct
{
	//机械角度
	int16_t Angle;
	//转速
	int16_t Speed;
	//电流
	int16_t Current;
	//温度
	int16_t Temperature;
	
}DJI_Motor_Data_Struct;

//Yaw轴数据处理结构体
typedef struct
{
	//Yaw轴上一次原始数据
	int16_t YAW_Last_Data;
	//Yaw轴电机数据旋转圈数
	int16_t YAW_Rotation_Number;
	//Yaw轴原始数据展开
	int32_t YAW_Expand_Data;
	//Yaw轴真实数据展开
	float Yaw_Real_Expand_Data;
	//Yaw轴真实旋转圈数
	int16_t Yaw_Real_Rotation_Number;
	//Yaw轴数据特殊处理标志位
	uint8_t Yaw_Angle_Data_Special_Flag;
	
}Yaw_Motor_Data_Handle_Struct;


//接收3508和2006电机返回值
void Get_Base_Encode_Data(void);
//接收云台3508和2006和6020电机返回值
void Get_Gimbal_Encode_Data(void);

#endif
