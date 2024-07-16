/**
  ******************************************************************************
  * Description : ����һ�����ڳ�ʼ������6�Ĵ���
  *  
  ******************************************************************************
  * @attention	��λ��ͨ�Ŵ��ڳ�ʼ��
  * 
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"


/* -------------------- ��ʼ������6 -------------------- */

/**
  * @brief  ��ʼ������6
  * @param  None
  * @retval None
*/
void Serial_Init(void)
{
	//����6ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	//����1ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//��λ����6																	///�ȸ��ô��ں���GPIO���ù���
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6);  //PG9 ����ΪUSART6
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //PG14����ΪUSART6
	//��λ����1 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //PA9 ����Ϊ USART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1); //PB7����ΪUSART1
	
	GPIO_InitTypeDef GPIO_InitStructure;
	//����6����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOG9,G14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //�ٶ� 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOG,&GPIO_InitStructure); //��ʼ��GPIO
	//����1����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //GPIOA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ� 50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ�� PA9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //GPIOB7
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ�� PB7

	USART_InitTypeDef USART_InitStructure;
	//USART6��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ 8 λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(USART6, &USART_InitStructure); //��ʼ������
	//USART1��ʼ������
	USART_Init(USART1, &USART_InitStructure); //��ʼ������

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//�����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0; //��Ӧ���ȼ� 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����6�ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=5;//��ռ���ȼ� 
	NVIC_Init(&NVIC_InitStructure); //����1�ж�

	USART_Cmd(USART6, ENABLE); //ʹ�ܴ���6
	USART_Cmd(USART1, ENABLE); //ʹ�ܴ���1
}


/* -------------------- ����6���Ͳ��� -------------------- */

void Serial_6_SendByte(uint8_t Byte)
{
	USART_SendData(USART6, Byte);//���ͺ���
	while(USART_GetFlagStatus(USART6, USART_FLAG_TXE)==RESET);
	//�ȴ����ݷ������(USART_FLAG_TXE:���ݼĴ����ձ�־λ,�ȴ�����1,�����ֶ�����)
}

void Serial_6_SendString(char *string)//�����ַ�������(�ַ����Դ�������־λ0�����봫�볤��) ʹ��\r\n����
{
	uint8_t i;
	for(i=0;string[i]!=0;i++)//�����ַ���(��string[i]=������־λ0��ֹͣѭ��)
	{
		Serial_6_SendByte(string[i]);//���ͺ���
	}
}

void Serial_6_SendArray(uint8_t *Array, uint16_t Length)//�������麯��
{
	uint16_t i;
	for(i=0;i<Length;i++)//��������
	{
		Serial_6_SendByte(Array[i]);//���ͺ���
	}
}


/* -------------------- ����1���Ͳ��� -------------------- */

void Serial_1_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);//���ͺ���
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
	//�ȴ����ݷ������(USART_FLAG_TXE:���ݼĴ����ձ�־λ,�ȴ�����1,�����ֶ�����)
}

void Serial_1_SendString(char *string)//�����ַ�������(�ַ����Դ�������־λ0�����봫�볤��) ʹ��\r\n����
{
	uint8_t i;
	for(i=0;string[i]!=0;i++)//�����ַ���(��string[i]=������־λ0��ֹͣѭ��)
	{
		Serial_1_SendByte(string[i]);//���ͺ���
	}
}

void Serial_1_SendArray(uint8_t *Array, uint16_t Length)//�������麯��
{
	uint16_t i;
	for(i=0;i<Length;i++)//��������
	{
		Serial_1_SendByte(Array[i]);//���ͺ���
	}
}

/**
  * @brief  ��ӳ���printf����
  * @param  ������ַ���
  * @retval None
*/
void Serial_1_Printf(char *format, ...)//��װsprintfʵ�ֶമ��ʹ��printf
{
	char String[100];//����һ���ַ��������ڽ���printf������
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);//��printf�ض���String��
	va_end(arg);
	Serial_1_SendString(String);//ʹ��������ڰ�����ַ�������ȥ
}



/* -------------------- ����6�����жϺ��� -------------------- */

void USART6_IRQHandler(void)
{	
	if(USART_GetITStatus(USART6, USART_IT_RXNE)==SET)//�жϱ�־λ  USART_IT_RXNE:�����ݼĴ����ǿ�
	{
		
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);//���RXNE��־λ
	}
}
