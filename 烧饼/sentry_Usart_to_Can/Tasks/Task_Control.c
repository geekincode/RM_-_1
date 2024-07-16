#include "stm32f4xx.h"                  // Device header
#include "rtthread.h"
#include "Judge_Task.h"
#include "UI_Task.h"
#include "Judge.h"



/* -------------------- 线程初始化 -------------------- */

//判系统信息发送任务返回值
static rt_thread_t Judge_Routing_thread;
//UI绘制任务返回值
static rt_thread_t UI_Protract_thread;


/**
  * @brief  线程初始化函数
  * @param  None
  * @retval None
*/
void Task_Init(void)
{
	/* 裁判系统信息发送任务 */
	Judge_Routing_thread = rt_thread_create(
		"Judge",						//线程名称
		 Judge_Routing_thread_entry,	//入口函数
		 RT_NULL,						//线程参数
		 1600,							//堆栈大小
		 1,					 			//优先级(越小越高)
		 1								//时间片
	);
	
	/* UI绘制任务 */
	UI_Protract_thread = rt_thread_create(
		"Inspection",					//线程名称
		 UI_Protract_thread_entry,		//入口函数
		 RT_NULL,						//线程参数
		 1600,							//堆栈大小
		 0, 							//优先级(越小越高)
		 1								//时间片
	);
	
	long a = Judge.game_robot_status.shooter_id1_17mm_speed_limit;
	
	//启动任务
	if(Judge_Routing_thread != RT_NULL){rt_thread_startup(Judge_Routing_thread);}
	if(UI_Protract_thread != RT_NULL){rt_thread_startup(UI_Protract_thread);}
}
