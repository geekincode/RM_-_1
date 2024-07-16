/**
  ******************************************************************************
  * Description : 这是一坨线程控制的代码
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"


/* -------------------- 全局变量 -------------------- */

//执行周期检测结构体
Execution_Count_Struct Execution_Count_t;



/* -------------------- 功能初始化 -------------------- */

//功能初始化任务返回值
static rt_thread_t Fun_Init_thread;
//imu零飘校准任务返回值
static rt_thread_t IMU_Offset_thread;
//采样任务返回值
static rt_thread_t Sampling_thread;


/**
  * @brief  功能初始化
  * @param  None
  * @retval None
*/
void Function_Init(void)
{
	/* 功能初始化任务 */
	Fun_Init_thread = rt_thread_create(
		"Fun_Init",						//线程名称
		 Fun_Init_thread_entry,			//入口函数
		 RT_NULL,						//线程参数
		 500,							//堆栈大小
		 FUN_INIT_PRIORITY, 			//优先级(越小越高)
		 1								//时间片
	);
	
	/* IMU零飘校准任务 */
	IMU_Offset_thread = rt_thread_create(
		"IMU_Offset",					//线程名称
		 IMU_Offset_thread_entry,		//入口函数
		 RT_NULL,						//线程参数
		 384,							//堆栈大小
		 IMU_OFFSET_PRIORITY, 			//优先级(越小越高)
		 1								//时间片
	);
	
	/* 采样任务 */
	Sampling_thread = rt_thread_create(
		"Sampling",						//线程名称
		 Sampling_thread_entry,			//入口函数
		 RT_NULL,						//线程参数
		 1500,							//堆栈大小
		 SAMPLING_PRIORITY, 			//优先级(越小越高)
		 1								//时间片
	);
	
	//把线程添加到就绪队列中
	if(Fun_Init_thread != RT_NULL){rt_thread_startup(Fun_Init_thread);}
	if(IMU_Offset_thread != RT_NULL){rt_thread_startup(IMU_Offset_thread);}
	if(Sampling_thread != RT_NULL){rt_thread_startup(Sampling_thread);}
}



/* -------------------- 线程初始化 -------------------- */

//底盘任务返回值
static rt_thread_t Base_thread;
//云台任务返回值
static rt_thread_t Gimbal_thread;
//上位机通信任务返回值
static rt_thread_t PC_Communicate_thread;

//主线程任务返回值
static rt_thread_t Start_Main_thread;
//开启待机模式任务返回值
static rt_thread_t Turn_On_Standby_thread;
//整机检测任务返回值
static rt_thread_t Inspection_thread;

//主线程任务入口函数声明
void Start_Main_thread_entry(void *parameter);
//开启待机模式入口函数声明
void Turn_On_Standby_thread_entry(void *parameter);


/**
  * @brief  线程初始化函数
  * @param  None
  * @retval None
*/
void Task_Init(void)
{	
	/* 整机检测任务 */
	Inspection_thread = rt_thread_create(
		"Inspection",					//线程名称
		 Inspection_thread_entry,		//入口函数
		 RT_NULL,						//线程参数
		 1500,							//堆栈大小
		 INSPECTION_PRIORITY, 			//优先级(越小越高)
		 1								//时间片
	);
	
	/* 底盘任务 */
	Base_thread = rt_thread_create(
		"Base",							//线程名称
		 Base_thread_entry,				//入口函数
		 RT_NULL,						//线程参数
		 1500,							//堆栈大小
		 BASE_PRIORITY, 				//优先级(越小越高)
		 1								//时间片
	);
	
	/* 云台任务 */
	Gimbal_thread = rt_thread_create(
		"Gimbal",						//线程名称
		 Gimbal_thread_entry,			//入口函数
		 RT_NULL,						//线程参数
		 1500,							//堆栈大小
		 GIMBAL_PRIORITY, 				//优先级(越小越高)
		 1								//时间片
	);
	
	/* 上位机通信任务 */
	PC_Communicate_thread = rt_thread_create(
		"PC_Communicate",				//线程名称
		 PC_Communicate_thread_entry,	//入口函数
		 RT_NULL,						//线程参数
		 1500,							//堆栈大小
		 PC_COMMUNICATE_PRIORITY, 		//优先级(越小越高)
		 1								//时间片
	);
	
	/* 开启待机模式任务 */
	Turn_On_Standby_thread = rt_thread_create(
		"Turn_On_Standby",				//线程名称
		 Turn_On_Standby_thread_entry,	//入口函数
		 RT_NULL,						//线程参数
		 384,							//堆栈大小
		 TURN_ON_STANDBY_PRIORITY, 		//优先级(越小越高)
		 1								//时间片
	);
}



/* -------------------- 线程控制 -------------------- */

/**
  * @brief  开启待机模式
  * @param  None
  * @retval None
*/
void Turn_On_Standby_Mode(void)
{	
	//恢复待机线程
	rt_thread_resume(Turn_On_Standby_thread);
}

/**
  * @brief  启动主线程
  * @param  None
  * @retval None
*/
void Start_Thread(void)
{
	/* 启动主线程任务 */
	Start_Main_thread = rt_thread_create(
		"Start_Main",					//线程名称
		 Start_Main_thread_entry,		//入口函数
		 RT_NULL,						//线程参数
		 256,							//堆栈大小
		 START_MAIN_PRIORITY, 			//优先级(越小越高)
		 1								//时间片
	);
	
	//把线程添加到就绪队列中
	if(Start_Main_thread != RT_NULL){rt_thread_startup(Start_Main_thread);}
}



/* -------------------- 线程控制入口函数 -------------------- */

//主线程任务入口函数
void Start_Main_thread_entry(void *parameter)
{
	//把线程添加到就绪队列中
	if(Inspection_thread != RT_NULL){rt_thread_startup(Inspection_thread);}
	if(PC_Communicate_thread != RT_NULL){rt_thread_startup(PC_Communicate_thread);}
	if(Base_thread != RT_NULL){rt_thread_startup(Base_thread);}
	if(Gimbal_thread != RT_NULL){rt_thread_startup(Gimbal_thread);}
	if(Turn_On_Standby_thread != RT_NULL){rt_thread_startup(Turn_On_Standby_thread);}
	//挂起待机任务
	rt_thread_suspend(Turn_On_Standby_thread);
}

//开启待机模式入口函数
void Turn_On_Standby_thread_entry(void *parameter)
{
	while(1)
	{			
		//手动退出待机模式
		if(rc_data.rc.s1!=1){rt_thread_suspend(Turn_On_Standby_thread);rt_schedule();}
	}
}
