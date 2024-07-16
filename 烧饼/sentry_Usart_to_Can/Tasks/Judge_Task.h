#ifndef __JUDGE_TASK_H
#define __JUDGE_TASK_H

//裁判系统信息结构体
typedef struct 
{
	//裁判系统接收标志位
	uint8_t Judge_Receive_Flag;
	
	//接受的摩擦轮状态,小陀螺状态,自瞄状态,超电开关,自动开火状态
	uint8_t Friction;
	uint8_t Gyro;
	uint8_t AimBot;
	uint8_t Cap;
	uint8_t Auto;
	
	//接收的调试数据
	int16_t Debug_Data;
	
}Judge_Struct;

//裁判系统信息结构体
extern Judge_Struct Judge_t;


//裁判系统信息发送任务入口
void UI_Protract_thread_entry(void *parameter);


#endif
