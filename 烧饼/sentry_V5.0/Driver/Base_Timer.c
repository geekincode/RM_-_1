/**
  ******************************************************************************
  * Description : ����һ������������ʱ���Ĵ���
  *  
  ******************************************************************************
  * @attention	
  *
  *			
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- ȫ�ֱ��� -------------------- */

//��ʱ��־λ
uint8_t Delay_Flag;



/* -------------------- ��ʼ����ʱ��6 -------------------- */

/**
  * @brief  ��ʼ������
  * @param  None
  * @retval None
*/
void Base_Timer_Init(void)
{
	//��ʱ��6Ƶ��1ms
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);//������ʱ��6��ʱ��
	
	TIM_InternalClockConfig(TIM6);//ѡ���ڲ�ʱ��Ϊʱ��Դ
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//ʱ����Ԫ��ʼ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//�˲�������Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period=10-1;//ARR�Զ���װ��
	TIM_TimeBaseInitStruct.TIM_Prescaler=8400-1;//PSCԤ��Ƶ��
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//�ظ�������
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);//�ڳ�ʼ��֮ǰ�����1���жϱ�־λ��ʹ����ֵ��0��ʼ(������Բ���)
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);//�����жϣ�ѡ������ж�
	
	NVIC_InitTypeDef NVIC_InitStruct;//����NVIC
	NVIC_InitStruct.NVIC_IRQChannel=TIM6_DAC_IRQn;//ѡ��ʱ��6���ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;//ָ����ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;//ָ����Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM6, ENABLE);//ʹ�ܼ�����
}


/* -------------------- ms��ʱ���� -------------------- */

/**
  * @brief  ms��ʱ����
  * @param  ��ʱ������
  * @retval None
*/
void Timer_Delay_ms(uint16_t time)
{
	int16_t Count=0;
	while(Count<=time)
	{
		if(Delay_Flag==1)
		{
			Delay_Flag=0;
			Count++;
		}
	}
}


/* -------------------- ��ʱ��6�жϺ��� -------------------- */

void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update)==SET)//�жϱ�־λ�Ƿ���ȷ
	{
		//��ʱ��־λ
		Delay_Flag=1;
		
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);//�����־λ
	}
}
