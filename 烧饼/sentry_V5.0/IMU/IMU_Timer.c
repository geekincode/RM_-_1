#include "stm32f4xx.h"                  // Device header


/* -------------------- ȫ�ֱ��� -------------------- */

//IMU���������
uint32_t IMU_Timer_Count;
//IMU��ʱ��־λ
uint8_t IMU_Timer_Count_Flag;


/* -------------------- IMU��ʱ���ӿں��� -------------------- */

/**
  * @brief  �����Ƕ�ʱ����ʼ��
  * @param  None
  * @retval None
*/
void IMU_Timer_Init(void)
{
	//��ʱ��7Ƶ��1ms
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//������ʱ��7��ʱ��
	
	TIM_InternalClockConfig(TIM7);//ѡ���ڲ�ʱ��Ϊʱ��Դ
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//ʱ����Ԫ��ʼ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//�˲�������Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period=2000-1;//ARR�Զ���װ��
	TIM_TimeBaseInitStruct.TIM_Prescaler=42-1;//PSCԤ��Ƶ��
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//�ظ�������
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);
	
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);//�ڳ�ʼ��֮ǰ�����1���жϱ�־λ��ʹ����ֵ��0��ʼ(������Բ���)
	
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);//�����жϣ�ѡ������ж�
	
	NVIC_InitTypeDef NVIC_InitStruct;//����NVIC
	NVIC_InitStruct.NVIC_IRQChannel=TIM7_IRQn;//ѡ��ʱ��6���ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;//ָ����ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;//ָ����Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM7, DISABLE);//�رռ�����
}

/**
  * @brief  ��ʼ��ʱ
  * @param  None
  * @retval None
*/
void IMU_Timer_Start_Count(void)
{
	TIM_SetCounter(TIM7, 0);//��ʱ������������
	TIM_Cmd(TIM7, ENABLE);//ʹ�ܼ�����
	IMU_Timer_Count=0;//�������������
	IMU_Timer_Count_Flag=1;//�ر���ʱ����
}

/**
  * @brief  ֹͣ��ʱ������
  * @param  None
  * @retval None
*/
uint32_t IMU_Timer_Stop_Count(void)
{
	TIM_Cmd(TIM7, DISABLE);//�رռ�����
	uint32_t temp_count = IMU_Timer_Count;
	uint32_t temp_timer_count = (uint32_t)((float)TIM_GetCounter(TIM7)/2);
	TIM_SetCounter(TIM7, 0);//��ʱ������������
	IMU_Timer_Count=0;//�������������
	IMU_Timer_Count_Flag=0;//������ʱ����
	return (temp_count*1000 + temp_timer_count);
}

/**
  * @brief  ������ֵ
  * @param  None
  * @retval None
*/
uint32_t IMU_Timer_Get_Count(void)
{
	uint32_t temp_count = IMU_Timer_Count;
	uint32_t temp_timer_count = (uint32_t)((float)TIM_GetCounter(TIM7)/2);
	return (temp_count*1000 + temp_timer_count);
}


/* -------------------- �жϺ��� -------------------- */

/**
  * @brief  ��ʱ��7�жϺ���
  * @param  None
  * @retval None
*/
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7, TIM_IT_Update)==SET)//�жϱ�־λ�Ƿ���ȷ
	{
		IMU_Timer_Count++;
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);//�����־λ
	}
}
