/**
  ******************************************************************************
  * Description : 这是设置PWM的代码
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- 初始化PWM-------------------- */

/**
  * @brief  初始化PWM
  * @param  None
  * @retval None
*/
void PWM_Init(void)
{
	//Timer1通道1(PE9)输出PWM(频率500HZ)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//开启GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//开启Timer1时钟
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); //GH9 复用为 TIM1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); //GH11 复用为 TIM1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); //GH13 复用为 TIM1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1); //GH14 复用为 TIM1
	
	GPIO_InitTypeDef GPIO_InitStructure;//初始化GPIO
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM1);//选择内部时钟为时钟源
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//时基单元初始化
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//滤波器不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period=100-1;//ARR自动重装器(100方便调占空比)
	TIM_TimeBaseInitStruct.TIM_Prescaler=3360-1;//PSC预分频器
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//重复计数器
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	TIM_OCInitTypeDef TIM_OCInitStruct;//初始化Timer1输出比较通道1
	TIM_OCStructInit(&TIM_OCInitStruct);//先给结构体赋初始值，防止使用高级定时器时参数配置不完全导致无法正常输出PWM
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;//PWM模式1(常用)
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;//TIM_OCPolarity_High REF极性不翻转
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;//输出使能
	TIM_OCInitStruct.TIM_Pulse=0;//设置CCR的值(占空比)
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);//初始化四个通道
	TIM_OC2Init(TIM1, &TIM_OCInitStruct);
	TIM_OC3Init(TIM1, &TIM_OCInitStruct);
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
	TIM_Cmd(TIM1, ENABLE);//使能计数器
}


/**
  * @brief  设置PWM占空比
  * @param  占空比
  * @retval None
*/
void PWM_SetCompare(uint16_t Compare_T1C1,uint16_t Compare_T1C2, uint16_t Compare_T1C3,uint16_t Compare_T1C4)//调用函数可以更改占空比
{
	TIM_SetCompare1(TIM1, Compare_T1C1);//更改CCR寄存器1的值
	TIM_SetCompare2(TIM1, Compare_T1C2);//更改CCR寄存器2的值
	TIM_SetCompare3(TIM1, Compare_T1C3);//更改CCR寄存器3的值
	TIM_SetCompare4(TIM1, Compare_T1C4);//更改CCR寄存器4的值
}



/* -------------------- snail驱动-------------------- */


/**
  * @brief  snail发射
  * @param  转速(20~100)
  * @retval None
*/
void Snail_Shoot(uint16_t Speed)
{
	PWM_SetCompare(Speed,Speed,Speed,Speed);
}


/**
  * @brief  snail行程校准
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
