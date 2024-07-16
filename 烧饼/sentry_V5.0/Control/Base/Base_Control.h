#ifndef __BASE_CONTROL_H
#define __BASE_CONTROL_H


//底盘控制结构体
typedef struct 
{
	//底盘前进方向(相对yaw轴6020展开前的数据)
	int16_t Original_Forward_Direction;
	//遥控器模式小陀螺模式速度
	int16_t Remote_Gyro_Target_Speed;
	//小陀螺旋转标志位
	uint8_t Gyro_Rotating_Flag;
	
}Base_Control_Struct;

//键鼠标志位结构体
typedef struct
{
	uint8_t Z_Flag;	 				//键盘Z键标志位
	uint8_t Push_R_Flag; 			//鼠标右键标志位
	uint8_t Shift_Flag; 			//键盘Shift键标志位
	uint8_t E_Flag; 				//键盘E键标志位
	uint8_t F_Flag; 				//键盘F键标志位
	uint8_t C_Flag; 				//键盘C键标志位
	
	uint8_t Key_Friction_Flag;		//键盘摩擦轮标志位
	uint8_t Key_AimBot_Flag; 		//键盘自瞄模式标志位
	uint8_t Key_Gyro_Flag; 			//键盘小陀螺模式标志位
	uint8_t Key_Auto_Shot_Flag; 	//键盘自瞄自动开火模式标志位
	uint8_t Key_Turn_Flag; 			//键盘云台反转模式标志位
	uint8_t Key_Cap_Flag; 			//键盘超电模式标志位
	
}KeyMouse_Flag;

//遥控器底盘任务入口
void Base_thread_entry(void *parameter);


#endif
