/**
  ******************************************************************************
  * Description : 这是一坨用于启动定时器的代码
  *  
  ******************************************************************************
  * @attention	
  *
  *			
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- 全局变量 -------------------- */

//延时标志位
uint8_t Delay_Flag;



/* -------------------- 初始化定时器6 -------------------- */

/**
  * @brief  初始化函数
  * @param  None
  * @retval None
*/
void Base_Timer_Init(void)
{
	//定时器6频率1ms
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);//开启定时器6的时钟
	
	TIM_InternalClockConfig(TIM6);//选择内部时钟为时钟源
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//时基单元初始化
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//滤波器不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period=10-1;//ARR自动重装器
	TIM_TimeBaseInitStruct.TIM_Prescaler=8400-1;//PSC预分频器
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//重复计数器
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);//在初始化之前先清除1次中断标志位，使计数值从0开始(这个可以不加)
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);//开启中断，选择更新中断
	
	NVIC_InitTypeDef NVIC_InitStruct;//配置NVIC
	NVIC_InitStruct.NVIC_IRQChannel=TIM6_DAC_IRQn;//选择定时器6的中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;//指定抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;//指定响应优先级
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM6, ENABLE);//使能计数器
}


/* -------------------- ms延时函数 -------------------- */

/**
  * @brief  ms延时函数
  * @param  延时毫秒数
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


/* -------------------- 定时器6中断函数 -------------------- */

void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update)==SET)//判断标志位是否正确
	{
		//延时标志位
		Delay_Flag=1;
		
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);//清除标志位
	}
}
