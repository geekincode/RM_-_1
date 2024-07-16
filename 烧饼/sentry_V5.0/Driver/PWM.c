/**
  ******************************************************************************
  * Description : ��������PWM�Ĵ���
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- ��ʼ��PWM-------------------- */

/**
  * @brief  ��ʼ��PWM
  * @param  None
  * @retval None
*/
void PWM_Init(void)
{
	//Timer1ͨ��1(PE9)���PWM(Ƶ��500HZ)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//����GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//����Timer1ʱ��
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); //GH9 ����Ϊ TIM1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); //GH11 ����Ϊ TIM1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); //GH13 ����Ϊ TIM1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1); //GH14 ����Ϊ TIM1
	
	GPIO_InitTypeDef GPIO_InitStructure;//��ʼ��GPIO
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM1);//ѡ���ڲ�ʱ��Ϊʱ��Դ
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//ʱ����Ԫ��ʼ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//�˲�������Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period=100-1;//ARR�Զ���װ��(100�����ռ�ձ�)
	TIM_TimeBaseInitStruct.TIM_Prescaler=3360-1;//PSCԤ��Ƶ��
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//�ظ�������
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	TIM_OCInitTypeDef TIM_OCInitStruct;//��ʼ��Timer1����Ƚ�ͨ��1
	TIM_OCStructInit(&TIM_OCInitStruct);//�ȸ��ṹ�帳��ʼֵ����ֹʹ�ø߼���ʱ��ʱ�������ò���ȫ�����޷��������PWM
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;//PWMģʽ1(����)
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;//TIM_OCPolarity_High REF���Բ���ת
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;//���ʹ��
	TIM_OCInitStruct.TIM_Pulse=0;//����CCR��ֵ(ռ�ձ�)
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);//��ʼ���ĸ�ͨ��
	TIM_OC2Init(TIM1, &TIM_OCInitStruct);
	TIM_OC3Init(TIM1, &TIM_OCInitStruct);
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
	TIM_Cmd(TIM1, ENABLE);//ʹ�ܼ�����
}


/**
  * @brief  ����PWMռ�ձ�
  * @param  ռ�ձ�
  * @retval None
*/
void PWM_SetCompare(uint16_t Compare_T1C1,uint16_t Compare_T1C2, uint16_t Compare_T1C3,uint16_t Compare_T1C4)//���ú������Ը���ռ�ձ�
{
	TIM_SetCompare1(TIM1, Compare_T1C1);//����CCR�Ĵ���1��ֵ
	TIM_SetCompare2(TIM1, Compare_T1C2);//����CCR�Ĵ���2��ֵ
	TIM_SetCompare3(TIM1, Compare_T1C3);//����CCR�Ĵ���3��ֵ
	TIM_SetCompare4(TIM1, Compare_T1C4);//����CCR�Ĵ���4��ֵ
}



/* -------------------- snail����-------------------- */


/**
  * @brief  snail����
  * @param  ת��(20~100)
  * @retval None
*/
void Snail_Shoot(uint16_t Speed)
{
	PWM_SetCompare(Speed,Speed,Speed,Speed);
}


/**
  * @brief  snail�г�У׼
  * @param  None
  * @retval None
*/
void Snail_Init(void)
{
	rt_thread_mdelay(2000);
	PWM_SetCompare(MIN_INTERVAL,MIN_INTERVAL,MIN_INTERVAL,MIN_INTERVAL);
	rt_thread_mdelay(2000);
	PWM_SetCompare(RATE_OF_FIRE,RATE_OF_FIRE,RATE_OF_FIRE,RATE_OF_FIRE);
	Snail_Shoot(MIN_INTERVAL);
}
