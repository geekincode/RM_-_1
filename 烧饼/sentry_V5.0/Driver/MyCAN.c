/**
  ******************************************************************************
  * Description : ����һ��CAN���õĴ���
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"


/* -------------------- ��ʼ��CAN1,CAN2 -------------------- */

/**
  * @brief  ����CAN��ģʽ
  * @param  None
  * @retval None
*/
void MyCAN_Mode_Config(void)
{
	//CAN���߳�ʼ��
	CAN_InitTypeDef CAN_InitStruct;
	//����ģʽ����
	CAN_InitStruct.CAN_Mode=CAN_Mode_Normal;//����ģʽ
	//CAN_InitStruct.CAN_Mode=CAN_Mode_LoopBack;//�ػ�����
	//����������
	CAN_InitStruct.CAN_Prescaler=3;//CAN����ʱ�ӷ�Ƶ(1~1024)
	CAN_InitStruct.CAN_SJW=CAN_SJW_1tq;//����CAN����ͬ��ÿ�ο����ӻ����̵���󳤶�(1~4Tq)
	CAN_InitStruct.CAN_BS1=CAN_BS1_10tq;//����BS1�εĳ���(1~16Tq)
	CAN_InitStruct.CAN_BS2=CAN_BS2_3tq;//����BS2�εĳ���(1~8Tq)
	//���ܿ���
	CAN_InitStruct.CAN_TTCM=DISABLE;//�Ƿ�ʹ��ʱ�䴥������(����ʱ���,һ�㲻��)
	CAN_InitStruct.CAN_ABOM=ENABLE;//�Ƿ�ʹ���Զ����߹�����(�ڵ����ʵ���ʱ���˳�����)
	CAN_InitStruct.CAN_AWUM=ENABLE;//�Ƿ�ʹ���Զ����ѹ���(�����лʱ�����Զ��˳��͹���)
	CAN_InitStruct.CAN_NART=DISABLE;//�Ƿ�ʹ���Զ��ش�����
	CAN_InitStruct.CAN_RFLM=DISABLE;//�Ƿ�ʹ�ý�����������(6��������������֮��᲻�ٽ��ձ���)
	CAN_InitStruct.CAN_TXFP=DISABLE;//�������ȼ��ж�����  DISABLE:���ݱ������ȼ�  ENABLE:���ݱ��Ĵ���������Ⱥ�˳��
	
	//CAN1��ʼ��
	CAN_Init(CAN1,&CAN_InitStruct);
	//CAN2��ʼ��
	CAN_Init(CAN2,&CAN_InitStruct);
}

/**
  * @brief  ɸѡ����ʼ��
  * @param  None
  * @retval None
*/
void MyCAN_Filter_Config(void)
{
	//CANɸѡ��
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	CAN_FilterInitStruct.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStruct.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh=0x0000;//CAN_FxR2�Ĵ�����16λ
	CAN_FilterInitStruct.CAN_FilterMaskIdLow=0x0000;//CAN_FxR2�Ĵ�����16λ
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//����ɸѡ�����ݴ洢��FIFO0
	CAN_FilterInitStruct.CAN_FilterNumber=0;//ɸѡ�����(f103:0~13  f407:0~27)
	CAN_FilterInitStruct.CAN_FilterMode=CAN_FilterMode_IdMask;//ɸѡ��ģʽѡ������ģʽ
	CAN_FilterInitStruct.CAN_FilterScale=CAN_FilterScale_32bit;//ɸѡ���߶�32bit
	CAN_FilterInitStruct.CAN_FilterActivation=ENABLE;//�Ƿ�ʹ�ܱ�ɸѡ��
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	//CAN2ɸѡ��
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment=CAN_Filter_FIFO1;//����ɸѡ�����ݴ洢��FIFO1
	CAN_FilterInitStruct.CAN_FilterNumber=14;//ɸѡ�����(f103:0~13  f407:0~27)
	CAN_FilterInitStruct.CAN_FilterActivation=ENABLE;//�Ƿ�ʹ�ܱ�ɸѡ��
	CAN_FilterInit(&CAN_FilterInitStruct);
}

/**
  * @brief  �����жϳ�ʼ��
  * @param  None
  * @retval None
*/
void MyCAN_NVIC_Config(void)
{
	//CAN1�жϳ�ʼ��
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=CAN1_RX0_IRQn;//ѡ���ж�ͨ��(CAN1��������0���ж�ͨ��)
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;//ָ����ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;//ָ����Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStruct);
	
	//CAN2�жϳ�ʼ��
	NVIC_InitStruct.NVIC_IRQChannel=CAN2_RX1_IRQn;//ѡ���ж�ͨ��(CAN2��������1���ж�ͨ��)
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;//ָ����ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;//ָ����Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStruct);
	
	//ʹ���ж�
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//CAN1����0
	CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);//CAN2����1
}

/**
  * @brief  CAN�����ʼ������
  * @param  None
  * @retval None
*/
void MyCAN_Init(void)
{
	//ʱ�ӳ�ʼ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	//CAN1��GPIO��ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ�� PD0,PD1
	//CAN2��GPIO��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ�� PB5,PB6
	
	//CAN1���Ÿ���ӳ������
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_CAN1);//PD0 ����Ϊ CAN1
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_CAN1); //PD1 ����Ϊ CAN1
	//CAN2���Ÿ���ӳ������
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_CAN2);	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_CAN2);
	
	//CANģʽ��ʼ��
	MyCAN_Mode_Config();
	//ɸѡ����ʼ��
	MyCAN_Filter_Config();
	//�����жϳ�ʼ��
	MyCAN_NVIC_Config();
}



/* -------------------- CAN1,CAN2���մ��� -------------------- */

//�洢CAN1��������
CanRxMsg MyCAN1_Rece_Data;//�洢���յ�����
uint8_t MyCAN1_Rece_Flag;//�������ݱ�־λ

//CAN1����0�����ж�
void CAN1_RX0_IRQHandler(void)
{
	//���������־λ
	CAN_Receive(CAN1,CAN_FIFO0,&MyCAN1_Rece_Data);//��ȡ����0�����ݴ洢��CAN1_Rece_Data��
	
	//��������
	Get_Base_Encode_Data();
}

//�洢CAN2��������
CanRxMsg MyCAN2_Rece_Data;//�洢���յ�����
uint8_t MyCAN2_Rece_Flag;//�������ݱ�־λ

//CAN2����1�����ж�
void CAN2_RX1_IRQHandler(void)
{
	//���������־λ
	CAN_Receive(CAN2,CAN_FIFO1,&MyCAN2_Rece_Data);//��ȡ����1�����ݴ洢��CAN2_Rece_Data��
	
	//��������
	Get_Gimbal_Encode_Data();
}
