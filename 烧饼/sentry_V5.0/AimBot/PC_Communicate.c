/**
  ******************************************************************************
  * Description : ����һ������ͨ�ŵĴ���
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"


/* -------------------- ȫ�ֱ��� -------------------- */

//�����������(yaw,pitch)
int16_t Target_Angle[2];
float Target_Yaw;
float Target_Pitch;
//�����־λ
uint8_t Shoot_Flag;
//����ԭʼ����
uint8_t Serial_RxPacket[sizeof(Target_Angle)+sizeof(Shoot_Flag)+1];



/* -------------------- �������ݴ��� -------------------- */

/**
  * @brief  �������ݴ���
  * @param  None
  * @retval None
*/
void AiMBot_Data_Handle(void)
{
	//���ݽ���
	Target_Angle[0] = ((int16_t)Serial_RxPacket[0]<<8)+((int16_t)Serial_RxPacket[1]);
	Target_Angle[1] = ((int16_t)Serial_RxPacket[2]<<8)+((int16_t)Serial_RxPacket[3]);
	
	//�����־λ
	Shoot_Flag = Serial_RxPacket[4];
	
	//yaw��pitch�����(��λ������ģʽ)
	if(Shoot_Flag==1 && rc_data.rc.s2==2)
	{
		Target_Yaw = (float)Target_Angle[0]/10000;
		Target_Pitch = (float)Target_Angle[1]/10000;
		
		Gimbal_Control_t.GimBla_Pitch_Angle = -(float)(Target_Pitch)*4095/PI + PITCH_INIT_ANGLE;
		Gimbal_Control_t.GimBla_Yaw_Angle = (float)(Target_Yaw)*180/PI + QEKF_INS.YawRoundCount*360;
	}
}



/* -------------------- ��λ��ͨ������ -------------------- */

/**
  * @brief  ��λ��ͨ���������
  * @param  None
  * @retval None
*/
void PC_Communicate_thread_entry(void *parameter)
{
	while(1)
	{
		//ִ�����ڼ���
		#if(EXECUTION_COUNT_SWITCH==1)
			Execution_Count_t.PC_Execution_Count++;
		#endif
		
		//���͵�����(roll,yaw.pitch,speed)
		int16_t Serial_SendData[4];
		Serial_SendData[0]=(int16_t)(INS.Roll/180*PI*10000);
		Serial_SendData[1]=-(int16_t)((float)(Motor_Filiter_Data_t.Pitch_Motor_Angle-PITCH_INIT_ANGLE)/8191*PI*10000);
		Serial_SendData[2]=(int16_t)(INS.Yaw/180*PI*10000);
		Serial_SendData[3]=28*100; //����
		
		//���ڷ���
		uint8_t SendData[sizeof(Serial_SendData)+1];
		SendData[0] = Serial_SendData[0]>>8;
		SendData[1] = Serial_SendData[0];
		SendData[2] = Serial_SendData[1]>>8;
		SendData[3] = Serial_SendData[1];
		SendData[4] = Serial_SendData[2]>>8;
		SendData[5] = Serial_SendData[2];
		SendData[6] = Serial_SendData[3]>>8;
		SendData[7] = Serial_SendData[3];
		SendData[8] = 0;//��0��1
		
		//��ͷ
		Serial_1_SendByte(0xA1);
		//����
		Serial_1_SendArray(SendData,sizeof(SendData));
		//У���
		Serial_1_SendByte((SendData[0]>>4) + (SendData[2]>>4));
		
		//ִ��Ƶ��
		rt_thread_mdelay(TASK_EXECUTION_CYCLE);
	}
}



/* -------------------- ����1�����жϺ��� -------------------- */

void USART1_IRQHandler(void)
{	
	if(USART_GetITStatus(USART1, USART_IT_RXNE)==SET)//�жϱ�־λ  USART_IT_RXNE:�����ݼĴ����ǿ�
	{
		static uint8_t RxState=0;//״̬��״̬��־��0�ȴ���ͷ,1��������
		static uint8_t pRxPacket=0;//�洢��������ĳ���
		
		if(USART_GetITStatus(USART1, USART_IT_RXNE)==SET)//�жϱ�־λ  USART_IT_RXNE:�����ݼĴ����ǿ�
		{
			uint8_t RxData=USART_ReceiveData(USART1);//�洢���յ�ÿһλ����
			
			if(RxState==0)//��Ϊ0���жϽ��յ��ֽ��Ƿ�Ϊ��ͷ��������Ϊ1������������ȴ���ͷ
			{
				if(RxData==0xA0)//�յ����ǰ�ͷ
				{
					RxState=1;
					pRxPacket=0;
				}
			}
			else if(RxState==1)//��Ϊ1����������
			{
				Serial_RxPacket[pRxPacket]=RxData;//�����ݴ����������
				pRxPacket++;
				if(pRxPacket>=sizeof(Serial_RxPacket))//���ѽ���������
				{
					//�ɽ�����һ�ν���
					RxState=0;
					
					//��У��
					if(Serial_RxPacket[sizeof(Serial_RxPacket)-1]==Serial_RxPacket[1]+Serial_RxPacket[3])
					{
						//���ݴ���
						AiMBot_Data_Handle();
					}
				}
			}
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);//���RXNE��־λ
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);//���RXNE��־λ
	}
}
