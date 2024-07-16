#include "rtthread.h"
#include "stm32f4xx.h"
#include "Judge.h"
#include "Task_Control.h"
#include "MyCAN.h"
#include "RM_Client_UI.h"


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	//����ϵͳ���ڳ�ʼ��
	Judge_Init();
	//CAN��ʼ��
	MyCAN_Init();
	//UI��ʼ��
	UI_Init();
	
	//�����߳�
	Task_Init();
	
	while (1){
		
		
	}
}
