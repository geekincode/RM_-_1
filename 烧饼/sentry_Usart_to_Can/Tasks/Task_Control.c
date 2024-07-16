#include "stm32f4xx.h"                  // Device header
#include "rtthread.h"
#include "Judge_Task.h"
#include "UI_Task.h"
#include "Judge.h"



/* -------------------- �̳߳�ʼ�� -------------------- */

//��ϵͳ��Ϣ�������񷵻�ֵ
static rt_thread_t Judge_Routing_thread;
//UI�������񷵻�ֵ
static rt_thread_t UI_Protract_thread;


/**
  * @brief  �̳߳�ʼ������
  * @param  None
  * @retval None
*/
void Task_Init(void)
{
	/* ����ϵͳ��Ϣ�������� */
	Judge_Routing_thread = rt_thread_create(
		"Judge",						//�߳�����
		 Judge_Routing_thread_entry,	//��ں���
		 RT_NULL,						//�̲߳���
		 1600,							//��ջ��С
		 1,					 			//���ȼ�(ԽСԽ��)
		 1								//ʱ��Ƭ
	);
	
	/* UI�������� */
	UI_Protract_thread = rt_thread_create(
		"Inspection",					//�߳�����
		 UI_Protract_thread_entry,		//��ں���
		 RT_NULL,						//�̲߳���
		 1600,							//��ջ��С
		 0, 							//���ȼ�(ԽСԽ��)
		 1								//ʱ��Ƭ
	);
	
	long a = Judge.game_robot_status.shooter_id1_17mm_speed_limit;
	
	//��������
	if(Judge_Routing_thread != RT_NULL){rt_thread_startup(Judge_Routing_thread);}
	if(UI_Protract_thread != RT_NULL){rt_thread_startup(UI_Protract_thread);}
}
