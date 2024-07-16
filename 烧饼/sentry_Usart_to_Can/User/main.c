#include "rtthread.h"
#include "stm32f4xx.h"
#include "Judge.h"
#include "Task_Control.h"
#include "MyCAN.h"
#include "RM_Client_UI.h"


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	//裁判系统串口初始化
	Judge_Init();
	//CAN初始化
	MyCAN_Init();
	//UI初始化
	UI_Init();
	
	//启动线程
	Task_Init();
	
	while (1){
		
		
	}
}
