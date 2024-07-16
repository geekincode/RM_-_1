/**
  ******************************************************************************
  * Description : ����һ�����ϵͳͨ�ŵĴ���
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- ȫ�ֱ��� -------------------- */

//����ϵͳ��Ϣ�ṹ��
//Judge_Data_Struct Judge_Data_t;

//Judge_Struct Judge_t;

#include "stm32f4xx.h"                  // Device header
#include "Judge.h"
#include "Judge_crc.h"
#include "Judge_Task.h"


/* -------------------- ����ϵͳ�������� -------------------- */

//DMA����ԭʼ����
uint8_t judge_buf[2][220] = {0};
//����ϵͳ���ݽṹ��
judge_t Judge;



/* -------------------- ����ϵͳ������������ -------------------- */

//����ϵͳ����ɸѡ
void Judge_Data_Check(uint8_t *judge_data);
//����ϵͳ���ݸ���
int Judge_Data_Update(uint8_t * judge_data,judge_t* judge);



/* -------------------- ����ϵͳ��ʼ�� -------------------- */

/**
  * @brief  ����ϵͳ���ڳ�ʼ��
  * @param  None
  * @retval None
*/
void Judge_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//��λ���� 2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);  //PA2 ����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //PA3����ΪUSART2
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2,A3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //�ٶ� 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��GPIO

	USART_InitTypeDef USART_InitStructure;
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ 8 λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0; //��Ӧ���ȼ� 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ�� VIC �Ĵ���
	
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE); //ʹ�ܴ���DMA
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

	USART_ClearFlag(USART2, USART_FLAG_IDLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); //���������ж�

	USART_Cmd(USART2, ENABLE); //ʹ�ܴ��� 
	
	//��ʼ��DMA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE); //DMAʱ��
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Stream5); 

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;//DMAͨ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART2->DR);//����Ĵ���
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)judge_buf[0];//�ڴ�Ĵ���
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
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);//DMA��ʼ��
	
	DMA_DoubleBufferModeConfig(DMA1_Stream5, (uint32_t)judge_buf[1], DMA_Memory_0);
	DMA_DoubleBufferModeCmd(DMA1_Stream5, ENABLE);
	
	DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);//DMA�ж�
	DMA_Cmd(DMA1_Stream5, DISABLE); 
	DMA_Cmd(DMA1_Stream5, ENABLE);//DMAʹ��
}


/* -------------------- ���ڽ��շ��ͺ��� -------------------- */

void Judge_SendByte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);//���ͺ���
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
	//�ȴ����ݷ������(USART_FLAG_TXE:���ݼĴ����ձ�־λ,�ȴ�����1,�����ֶ�����)
}

void Judge_SendString(char *string)//�����ַ�������(�ַ����Դ�������־λ0�����봫�볤��) ʹ��\r\n����
{
	uint8_t i;
	for(i=0;string[i]!=0;i++)//�����ַ���(��string[i]=������־λ0��ֹͣѭ��)
	{
		Judge_SendByte(string[i]);//���ͺ���
	}
}

void Judge_SendArray(uint8_t *Array, uint16_t Length)//�������麯��
{
	uint16_t i;
	for(i=0;i<Length;i++)//��������
	{
		Judge_SendByte(Array[i]);//���ͺ���
	}
}


/* -------------------- ���ڽ����жϺ��� -------------------- */

/**
  * @brief  USART2�жϺ���
  * @param  None
  * @retval None
*/
void USART2_IRQHandler(void)
{	
	//DMA�����ж�
	if(DMA_GetITStatus(DMA1_Stream5,DMA_IT_DMEIF2) != RESET)
	{
		USART_ReceiveData(USART2);
	}
	//���ڿ����ж�
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
		
		//�ò���ϵͳ���ձ�־λ
		Judge_t.Judge_Receive_Flag=1;
	}
}


/* -------------------- ����ϵͳ���ݽ��� -------------------- */

/**
  * @brief  ����ϵͳ����ɸѡ
  * @param  None
  * @retval None
*/
void Judge_Data_Check(uint8_t *judge_data)
{
	static int16_t judge_data_len;
	int16_t i;
	for(i = 0;i < 200;i++)
	{
		//Ѱ����ȷ�õ�����
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
  * @brief  ����ϵͳ���ݸ���
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
			Judge_Data_Len = (uint16_t)judge->Judge_Data.frame_header.data_length + 9;  //��ȡ����֡����
			judge->Judge_Data.cmd_id = (uint16_t)(judge_data[5] | (judge_data[6] << 8)); //��ȡ����֡ID
			if(fabs(Judge_Data_Len) > 50) //������ݴ���
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

