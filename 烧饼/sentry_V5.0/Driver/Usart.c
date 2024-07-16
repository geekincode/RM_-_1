/**
  ******************************************************************************
  * Description : 这是一坨用于初始化串口6的代码
  *  
  ******************************************************************************
  * @attention	上位机通信串口初始化
  * 
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"


/* -------------------- 初始化串口6 -------------------- */

/**
  * @brief  初始化串口6
  * @param  None
  * @retval None
*/
void Serial_Init(void)
{
	//串口6时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	//串口1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//复位串口6																	///先复用串口后开启GPIO复用功能
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6);  //PG9 复用为USART6
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //PG14复用为USART6
	//复位串口1 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //PA9 复用为 USART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1); //PB7复用为USART1
	
	GPIO_InitTypeDef GPIO_InitStructure;
	//串口6引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOG9,G14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //速度 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化GPIO
	//串口1引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //GPIOA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化 PA9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //GPIOB7
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化 PB7

	USART_InitTypeDef USART_InitStructure;
	//USART6初始化设置
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为 8 位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	USART_Init(USART6, &USART_InitStructure); //初始化串口
	//USART1初始化设置
	USART_Init(USART1, &USART_InitStructure); //初始化串口

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0; //响应优先级 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道使能
	NVIC_Init(&NVIC_InitStructure); //串口6中断
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=5;//抢占优先级 
	NVIC_Init(&NVIC_InitStructure); //串口1中断

	USART_Cmd(USART6, ENABLE); //使能串口6
	USART_Cmd(USART1, ENABLE); //使能串口1
}


/* -------------------- 串口6发送部分 -------------------- */

void Serial_6_SendByte(uint8_t Byte)
{
	USART_SendData(USART6, Byte);//发送函数
	while(USART_GetFlagStatus(USART6, USART_FLAG_TXE)==RESET);
	//等待数据发送完成(USART_FLAG_TXE:数据寄存器空标志位,等待它置1,无需手动清零)
}

void Serial_6_SendString(char *string)//发送字符串函数(字符串自带结束标志位0，无须传入长度) 使用\r\n换行
{
	uint8_t i;
	for(i=0;string[i]!=0;i++)//遍历字符串(若string[i]=结束标志位0则停止循环)
	{
		Serial_6_SendByte(string[i]);//发送函数
	}
}

void Serial_6_SendArray(uint8_t *Array, uint16_t Length)//发送数组函数
{
	uint16_t i;
	for(i=0;i<Length;i++)//遍历数组
	{
		Serial_6_SendByte(Array[i]);//发送函数
	}
}


/* -------------------- 串口1发送部分 -------------------- */

void Serial_1_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);//发送函数
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
	//等待数据发送完成(USART_FLAG_TXE:数据寄存器空标志位,等待它置1,无需手动清零)
}

void Serial_1_SendString(char *string)//发送字符串函数(字符串自带结束标志位0，无须传入长度) 使用\r\n换行
{
	uint8_t i;
	for(i=0;string[i]!=0;i++)//遍历字符串(若string[i]=结束标志位0则停止循环)
	{
		Serial_1_SendByte(string[i]);//发送函数
	}
}

void Serial_1_SendArray(uint8_t *Array, uint16_t Length)//发送数组函数
{
	uint16_t i;
	for(i=0;i<Length;i++)//遍历数组
	{
		Serial_1_SendByte(Array[i]);//发送函数
	}
}

/**
  * @brief  重映射的printf函数
  * @param  输出的字符串
  * @retval None
*/
void Serial_1_Printf(char *format, ...)//封装sprintf实现多串口使用printf
{
	char String[100];//定义一个字符串，用于接收printf的内容
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);//将printf重定向到String中
	va_end(arg);
	Serial_1_SendString(String);//使用这个串口把这个字符串发出去
}



/* -------------------- 串口6接收中断函数 -------------------- */

void USART6_IRQHandler(void)
{	
	if(USART_GetITStatus(USART6, USART_IT_RXNE)==SET)//判断标志位  USART_IT_RXNE:读数据寄存器非空
	{
		
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);//清除RXNE标志位
	}
}
