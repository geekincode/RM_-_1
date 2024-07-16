#ifndef __TASK_CONTROL_H
#define __TASK_CONTROL_H

//执行周期检测结构体
typedef struct
{
	//数据处理任务执行次数
	uint32_t Sampling_Execution_Count;
	//检测任务执行次数
	uint32_t Detect_Execution_Count; 
	//上位机任务执行次数
	uint32_t PC_Execution_Count; 	
	//底盘任务执行次数
	uint32_t Base_Execution_Count; 
	//云台任务执行次数
	uint32_t Gimbal_Execution_Count; 
	
}Execution_Count_Struct;


//功能初始化
void Function_Init(void);
//线程初始化函数
void Task_Init(void);
//启动主线程
void Start_Thread(void);
//开启待机模式
void Turn_On_Standby_Mode(void);
//开启整机检测模式
void Complete_Machine_Inspection(void);

#endif
