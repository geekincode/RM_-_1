#include "stm32f4xx.h"                  // Device header


/* -------------------- 全局变量 -------------------- */

//IMU软件计数器
uint32_t IMU_Timer_Count;
//IMU计时标志位
uint8_t IMU_Timer_Count_Flag;


/* -------------------- IMU定时器接口函数 -------------------- */

/**
  * @brief  陀螺仪定时器初始化
  * @param  None
  * @retval None
*/
void IMU_Timer_Init(void)
{
	//定时器7频率1ms
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//开启定时器7的时钟
	
	TIM_InternalClockConfig(TIM7);//选择内部时钟为时钟源
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//时基单元初始化
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//滤波器不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period=2000-1;//ARR自动重装器
	TIM_TimeBaseInitStruct.TIM_Prescaler=42-1;//PSC预分频器
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//重复计数器
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);
	
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);//在初始化之前先清除1次中断标志位，使计数值从0开始(这个可以不加)
	
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);//开启中断，选择更新中断
	
	NVIC_InitTypeDef NVIC_InitStruct;//配置NVIC
	NVIC_InitStruct.NVIC_IRQChannel=TIM7_IRQn;//选择定时器6的中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;//指定抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;//指定响应优先级
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM7, DISABLE);//关闭计数器
}

/**
  * @brief  开始计时
  * @param  None
  * @retval None
*/
void IMU_Timer_Start_Count(void)
{
	TIM_SetCounter(TIM7, 0);//定时器计数器清零
	TIM_Cmd(TIM7, ENABLE);//使能计数器
	IMU_Timer_Count=0;//软件计数器清零
	IMU_Timer_Count_Flag=1;//关闭延时函数
}

/**
  * @brief  停止计时并读数
  * @param  None
  * @retval None
*/
uint32_t IMU_Timer_Stop_Count(void)
{
	TIM_Cmd(TIM7, DISABLE);//关闭计数器
	uint32_t temp_count = IMU_Timer_Count;
	uint32_t temp_timer_count = (uint32_t)((float)TIM_GetCounter(TIM7)/2);
	TIM_SetCounter(TIM7, 0);//定时器计数器清零
	IMU_Timer_Count=0;//软件计数器清零
	IMU_Timer_Count_Flag=0;//开启延时函数
	return (temp_count*1000 + temp_timer_count);
}

/**
  * @brief  读计数值
  * @param  None
  * @retval None
*/
uint32_t IMU_Timer_Get_Count(void)
{
	uint32_t temp_count = IMU_Timer_Count;
	uint32_t temp_timer_count = (uint32_t)((float)TIM_GetCounter(TIM7)/2);
	return (temp_count*1000 + temp_timer_count);
}


/* -------------------- 中断函数 -------------------- */

/**
  * @brief  定时器7中断函数
  * @param  None
  * @retval None
*/
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7, TIM_IT_Update)==SET)//判断标志位是否正确
	{
		IMU_Timer_Count++;
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);//清除标志位
	}
}
