#ifndef __DETECT_H
#define __DETECT_H


//错误检测结构体
typedef struct 
{
	//遥控器数据错误标志位
	uint8_t RC_Data_Error_Flag;
	//遥控器连接标志位
	uint8_t RC_Connected_Flag;
	//裁判系统连接标志位
	uint8_t Judge_Connected_Flag;
	
	//电机连接标志位
	uint8_t BASE_MOTOR1_Connected_Flag;	
	uint8_t BASE_MOTOR2_Connected_Flag;
	uint8_t BASE_MOTOR3_Connected_Flag;
	uint8_t BASE_MOTOR4_Connected_Flag;
	uint8_t SHOOT_LEFT_MOTOR_Connected_Flag;
	uint8_t SHOOT_RIGHT_MOTOR_Connected_Flag;
	uint8_t YAW_MOTOR_Connected_Flag;
	uint8_t PITCH_MOTOR_Connected_Flag;
	
	//掉线错误标志位
	uint8_t RC_Disconnected_Flag;
	uint8_t BASE_MOTOR1_Disconnected_Flag;	
	uint8_t BASE_MOTOR2_Disconnected_Flag;
	uint8_t BASE_MOTOR3_Disconnected_Flag;
	uint8_t BASE_MOTOR4_Disconnected_Flag;
	uint8_t SHOOT_LEFT_MOTOR_Disconnected_Flag;
	uint8_t SHOOT_RIGHT_MOTOR_Disconnected_Flag;
	uint8_t YAW_MOTOR_Disconnected_Flag;
	uint8_t PITCH_MOTOR_Disconnected_Flag;
	uint8_t Judge_Disconnected_Flag;
	
	//循环检测数据标志位
	uint8_t RC_Cycle_Detection_Flag;
	uint8_t Motor_Cycle_Detection_Flag;
	
}Error_Detect_Struct;


//整机检测任务入口函数
void Inspection_thread_entry(void *parameter);
//检测遥控器数据
void Remote_Data_Detect(void);

#endif
