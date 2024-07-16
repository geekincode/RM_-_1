/**
  ******************************************************************************
  * Description : 这是一坨裁判系统通信的代码
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- 全局变量 -------------------- */

//裁判系统信息结构体
//Judge_Data_Struct Judge_Data_t;

//Judge_Struct Judge_t;

#include "stm32f4xx.h"                  // Device header
#include "Judge.h"
#include "Judge_crc.h"
#include "Judge_Task.h"


/* -------------------- 裁判系统解析变量 -------------------- */

//DMA接收原始数据
uint8_t judge_buf[2][220] = {0};
//裁判系统数据结构体
judge_t Judge;



/* -------------------- 裁判系统解析函数声明 -------------------- */

//裁判系统数据筛选
void Judge_Data_Check(uint8_t *judge_data);
//裁判系统数据更新
int Judge_Data_Update(uint8_t * judge_data,judge_t* judge);



/* -------------------- 裁判系统初始化 -------------------- */

/**
  * @brief  裁判系统串口初始化
  * @param  None
  * @retval None
*/
void Judge_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//复位串口 2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);  //PA2 复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //PA3复用为USART2
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2,A3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //速度 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化GPIO

	USART_InitTypeDef USART_InitStructure;
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为 8 位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0; //响应优先级 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化 VIC 寄存器
	
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE); //使能串口DMA
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

	USART_ClearFlag(USART2, USART_FLAG_IDLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); //开启空闲中断

	USART_Cmd(USART2, ENABLE); //使能串口 
	
	//初始化DMA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE); //DMA时钟
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Stream5); 

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;//DMA通道
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART2->DR);//外设寄存器
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)judge_buf[0];//内存寄存器
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 200;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);//DMA初始化
	
	DMA_DoubleBufferModeConfig(DMA1_Stream5, (uint32_t)judge_buf[1], DMA_Memory_0);
	DMA_DoubleBufferModeCmd(DMA1_Stream5, ENABLE);
	
	DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);//DMA中断
	DMA_Cmd(DMA1_Stream5, DISABLE); 
	DMA_Cmd(DMA1_Stream5, ENABLE);//DMA使能
}


/* -------------------- 串口接收发送函数 -------------------- */

void Judge_SendByte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);//发送函数
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
	//等待数据发送完成(USART_FLAG_TXE:数据寄存器空标志位,等待它置1,无需手动清零)
}

void Judge_SendString(char *string)//发送字符串函数(字符串自带结束标志位0，无须传入长度) 使用\r\n换行
{
	uint8_t i;
	for(i=0;string[i]!=0;i++)//遍历字符串(若string[i]=结束标志位0则停止循环)
	{
		Judge_SendByte(string[i]);//发送函数
	}
}

void Judge_SendArray(uint8_t *Array, uint16_t Length)//发送数组函数
{
	uint16_t i;
	for(i=0;i<Length;i++)//遍历数组
	{
		Judge_SendByte(Array[i]);//发送函数
	}
}


/* -------------------- 串口接收中断函数 -------------------- */

/**
  * @brief  USART2中断函数
  * @param  None
  * @retval None
*/
void USART2_IRQHandler(void)
{	
	//DMA错误中断
	if(DMA_GetITStatus(DMA1_Stream5,DMA_IT_DMEIF2) != RESET)
	{
		USART_ReceiveData(USART2);
	}
	//串口空闲中断
	else if(USART_GetITStatus(USART2,USART_IT_IDLE) != RESET)
	{		
		USART2->SR;
		USART2->DR;
		
		if(DMA_GetCurrentMemoryTarget(DMA1_Stream5) == 1)
		{
			DMA_MemoryTargetConfig(DMA1_Stream5,(uint32_t)judge_buf[0],DMA_Memory_0);
			Judge_Data_Check(judge_buf[0]);
			memset(judge_buf[0],0,200);
		}
		else
		{
			DMA_MemoryTargetConfig(DMA1_Stream5,(uint32_t)judge_buf[1],DMA_Memory_1);
			Judge_Data_Check(judge_buf[1]);
			memset(judge_buf[1],0,200);
		}
		
		//置裁判系统接收标志位
		Judge_t.Judge_Receive_Flag=1;
	}
}


/* -------------------- 裁判系统数据解析 -------------------- */

/**
  * @brief  裁判系统数据筛选
  * @param  None
  * @retval None
*/
void Judge_Data_Check(uint8_t *judge_data)
{
	static int16_t judge_data_len;
	int16_t i;
	for(i = 0;i < 200;i++)
	{
		//寻找正确得到数据
		judge_data_len = Judge_Data_Update(judge_data + i,&Judge);
		if(judge_data_len != 0)
		{
			i = i + judge_data_len - 1;
		}
		if(i > 200)
		{
			return;
		}
	}
}

/**
  * @brief  裁判系统数据更新
  * @param  None
  * @retval None
*/
int Judge_Data_Update(uint8_t * judge_data,judge_t* judge)
{
	int Judge_Data_Len = 0;
	if(judge_data[0] == 0xA5)
	{
		judge->Judge_Data.frame_header.sof = 0xA5;
		judge->Judge_Data.frame_header.data_length = (uint16_t)(judge_data[1] | (judge_data[2] << 8));
		judge->Judge_Data.frame_header.seq = judge_data[3];
		if(verify_crc8_check_sum(judge_data,5) == 1)
		{
			Judge_Data_Len = (uint16_t)judge->Judge_Data.frame_header.data_length + 9;  //获取数据帧长度
			judge->Judge_Data.cmd_id = (uint16_t)(judge_data[5] | (judge_data[6] << 8)); //获取数据帧ID
			if(fabs(Judge_Data_Len) > 50) //如果数据错误
			return 200;
			
			if(verify_crc16_check_sum(judge_data,Judge_Data_Len) == 1)
			{
				switch(judge->Judge_Data.cmd_id)
				{
					case game_status_ID:
					{
						memcpy(&judge->game_status,(judge_data+7),11);
						break;
					}
					case game_result_ID:
					{
						memcpy(&judge->game_result,(judge_data+7),1);
						break;
					}
					case game_robot_HP_ID:
					{
						memcpy(&judge->game_robot_HP,(judge_data+7),32);
						break;
					}
					case ICRA_buff_debuff_zone_status_ID:
					{
						memcpy(&judge->ICRA_buff_debuff_zone_status,(judge_data+7),11);
						break;
					}
					case event_data_ID:
					{
						memcpy(&judge->event_data,(judge_data+7),4);
						break;
					}
					case supply_projectile_action_ID:
					{
						memcpy(&judge->supply_projectile_action,(judge_data+7),4);
						break;
					}
					case referee_warning_ID:
					{
						memcpy(&judge->referee_warning,(judge_data+7),2);
						break;
					}
					case dart_remaining_time_ID:
					{
						memcpy(&judge->dart_remaining_time,(judge_data+7),1);
						break;
					}
					case game_robot_status_ID:
					{
						memcpy(&judge->game_robot_status,(judge_data+7),26);
						break;
					}
					case power_heat_data_ID:
					{
						memcpy(&judge->power_heat_data,(judge_data+7),16);
						break;
					}
					case game_robot_pos_ID:
					{
						memcpy(&judge->game_robot_pos,(judge_data+7),16);
						break;
					}
					case buff_ID:
					{
						memcpy(&judge->buff,(judge_data+7),1);
						break;
					}
					case aerial_robot_energy_ID:
					{
						memcpy(&judge->aerial_robot_energy,(judge_data+7),1);
						break;
					}
					case robot_hurt_ID:
					{
						memcpy(&judge->robot_hurt,(judge_data+7),1);
						break;
					}
					case shoot_data_ID:
					{
						memcpy(&judge->shoot_data,(judge_data+7),7);
						break;
					}
					case bullet_remaining_ID:
					{
						memcpy(&judge->bullet_remaining,(judge_data+7),6);
						break;
					}
					case rfid_status_ID:
					{
						memcpy(&judge->rfid_status,(judge_data + 7),4);
						break;
					}
					case dart_client_cmd_ID:
					{
						memcpy(&judge->dart_client_cmd,(judge_data + 7),6);
						break;
					}
				}
				return Judge_Data_Len;
			}
			return 0;
		}
		return 0;
	}
	return 0;
}


const judge_t * Get_JUDGE_Data(void)
{
	return &Judge;
}

