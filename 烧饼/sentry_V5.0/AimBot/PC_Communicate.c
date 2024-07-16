/**
  ******************************************************************************
  * Description : 这是一坨自瞄通信的代码
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"


/* -------------------- 全局变量 -------------------- */

//解析后的数据(yaw,pitch)
int16_t Target_Angle[2];
float Target_Yaw;
float Target_Pitch;
//开火标志位
uint8_t Shoot_Flag;
//接收原始数据
uint8_t Serial_RxPacket[sizeof(Target_Angle)+sizeof(Shoot_Flag)+1];



/* -------------------- 自瞄数据处理 -------------------- */

/**
  * @brief  自瞄数据处理
  * @param  None
  * @retval None
*/
void AiMBot_Data_Handle(void)
{
	//数据解析
	Target_Angle[0] = ((int16_t)Serial_RxPacket[0]<<8)+((int16_t)Serial_RxPacket[1]);
	Target_Angle[1] = ((int16_t)Serial_RxPacket[2]<<8)+((int16_t)Serial_RxPacket[3]);
	
	//开火标志位
	Shoot_Flag = Serial_RxPacket[4];
	
	//yaw轴pitch轴输出(上位机控制模式)
	if(Shoot_Flag==1 && rc_data.rc.s2==2)
	{
		Target_Yaw = (float)Target_Angle[0]/10000;
		Target_Pitch = (float)Target_Angle[1]/10000;
		
		Gimbal_Control_t.GimBla_Pitch_Angle = -(float)(Target_Pitch)*4095/PI + PITCH_INIT_ANGLE;
		Gimbal_Control_t.GimBla_Yaw_Angle = (float)(Target_Yaw)*180/PI + QEKF_INS.YawRoundCount*360;
	}
}



/* -------------------- 上位机通信任务 -------------------- */

/**
  * @brief  上位机通信任务入口
  * @param  None
  * @retval None
*/
void PC_Communicate_thread_entry(void *parameter)
{
	while(1)
	{
		//执行周期计数
		#if(EXECUTION_COUNT_SWITCH==1)
			Execution_Count_t.PC_Execution_Count++;
		#endif
		
		//发送的数据(roll,yaw.pitch,speed)
		int16_t Serial_SendData[4];
		Serial_SendData[0]=(int16_t)(INS.Roll/180*PI*10000);
		Serial_SendData[1]=-(int16_t)((float)(Motor_Filiter_Data_t.Pitch_Motor_Angle-PITCH_INIT_ANGLE)/8191*PI*10000);
		Serial_SendData[2]=(int16_t)(INS.Yaw/180*PI*10000);
		Serial_SendData[3]=28*100; //弹速
		
		//串口发送
		uint8_t SendData[sizeof(Serial_SendData)+1];
		SendData[0] = Serial_SendData[0]>>8;
		SendData[1] = Serial_SendData[0];
		SendData[2] = Serial_SendData[1]>>8;
		SendData[3] = Serial_SendData[1];
		SendData[4] = Serial_SendData[2]>>8;
		SendData[5] = Serial_SendData[2];
		SendData[6] = Serial_SendData[3]>>8;
		SendData[7] = Serial_SendData[3];
		SendData[8] = 0;//红0蓝1
		
		//包头
		Serial_1_SendByte(0xA1);
		//数据
		Serial_1_SendArray(SendData,sizeof(SendData));
		//校验和
		Serial_1_SendByte((SendData[0]>>4) + (SendData[2]>>4));
		
		//执行频率
		rt_thread_mdelay(TASK_EXECUTION_CYCLE);
	}
}



/* -------------------- 串口1接收中断函数 -------------------- */

void USART1_IRQHandler(void)
{	
	if(USART_GetITStatus(USART1, USART_IT_RXNE)==SET)//判断标志位  USART_IT_RXNE:读数据寄存器非空
	{
		static uint8_t RxState=0;//状态机状态标志，0等待包头,1接收数据
		static uint8_t pRxPacket=0;//存储接收数组的长度
		
		if(USART_GetITStatus(USART1, USART_IT_RXNE)==SET)//判断标志位  USART_IT_RXNE:读数据寄存器非空
		{
			uint8_t RxData=USART_ReceiveData(USART1);//存储接收的每一位数据
			
			if(RxState==0)//若为0，判断接收的字节是否为包头，是则置为1，不是则继续等待包头
			{
				if(RxData==0xA0)//收到的是包头
				{
					RxState=1;
					pRxPacket=0;
				}
			}
			else if(RxState==1)//若为1，接收数据
			{
				Serial_RxPacket[pRxPacket]=RxData;//将数据存入接收数组
				pRxPacket++;
				if(pRxPacket>=sizeof(Serial_RxPacket))//若已接收完数据
				{
					//可进行下一次接收
					RxState=0;
					
					//和校验
					if(Serial_RxPacket[sizeof(Serial_RxPacket)-1]==Serial_RxPacket[1]+Serial_RxPacket[3])
					{
						//数据处理
						AiMBot_Data_Handle();
					}
				}
			}
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清除RXNE标志位
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清除RXNE标志位
	}
}
