#include "stm32f4xx.h"                  // Device header
#include "rtthread.h"
#include "RM_Client_UI.h"
#include "Judge_Task.h"



/* -------------------- UI����������� -------------------- */

/**
  * @brief  UI�����������
  * @param  None
  * @retval None
*/
void Judge_Routing_thread_entry(void *parameter)
{	
	while(1)
	{
		//������ʱ
		rt_thread_mdelay(1);
	}
}
