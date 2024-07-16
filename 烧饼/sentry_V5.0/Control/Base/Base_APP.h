#ifndef __BASE_APP_H
#define __BASE_APP_H


//底盘运动学解算结构体
typedef struct 
{
	//xy速度(相对于底盘)
	int16_t Base_X_Speed;
	int16_t Base_Y_Speed;
	
	//xy速度(相对于云台)
	int16_t Gimbal_X_Speed;
	int16_t Gimbal_Y_Speed;
	
	//z速度
	int16_t Z_Speed;
	
	//底盘云台相对角度
	float Yaw_Df_Ag;			///角度
	float Yaw_Df_Ag_pi;			///弧度
	
	//解析后的目标速度
	int16_t Target1,Target2,Target3,Target4;
	
	//平移角度补偿
	float Direction_Compensation;
	
}Kinematic_Solving_Struct;


//速度转换(前正后负，左正右负)
void Move_Speed_Transform(void);
//遥控器模式小陀螺启动
void Remote_Gyro_Start(void);
//遥控器模式小陀螺停止
void Remote_Gyro_Stop(void);
//底盘跟随云台动作
void Base_Follow_Gimbal_Action(int32_t Target);
//改变云台底盘相对角度
void Alter_Relative_Angles(void);

//轮子速度环动作
void Wheel_Speed_Action(int16_t Target1,int16_t Target2,int16_t Target3,int16_t Target4);


#endif
