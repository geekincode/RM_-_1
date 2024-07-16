#include "stm32f4xx.h"                  // Device header
#include "rtthread.h"
#include "RM_Client_UI.h"
#include "Judge_Task.h"



/* -------------------- UI绘制任务入口 -------------------- */

/**
  * @brief  UI绘制任务入口
  * @param  None
  * @retval None
*/
void Judge_Routing_thread_entry(void *parameter)
{	
	while(1)
	{
		//任务延时
		rt_thread_mdelay(1);
	}
}
