#include "stm32f4xx.h"                  // Device header


/* -------------------- �����ǵײ����� -------------------- */

/**
  * @brief  SPI��ʼ��
  * @param  None
  * @retval None
*/
void IMU_SPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//ʹ�� SPI1 ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//GPIOʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//SPI1����
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); //PA7����ΪMOSI
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4����ΪMISO
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3����ΪSCLK
	
	//Ӳ��SPI����
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);// ��ʼ��PB3,4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);// ��ʼ��PA7
	//ģ��CSS����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);// ��ʼ��PA4(���ٶ�Ƭѡ)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);// ��ʼ��PB0(������Ƭѡ)
	
	//SPI����
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//��ģʽ
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ָ��SPI�ĵ����˫������ģʽ��˫��ȫ˫��
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//ѡ��ģʽ��CPOL=1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//ѡ��ģʽ��CPHA=2
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//ָ��ʱ�ӷ�Ƶ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//ָ��SPI���ݴ�С��8Byte
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//ָ����λ���л��ǵ�λ���У���λ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//ָ��SS�ź�����Ӳ����������������
	SPI_InitStructure.SPI_CRCPolynomial = 10;//ָ������CRC����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  
	
	GPIO_SetBits(GPIOA,GPIO_Pin_4);//�Ƚ�Ƭѡ����
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	
	SPI_Cmd(SPI1, ENABLE); //ʹ�� SPI1
}

/**
  * @brief  SPI��ȡ���ٶȼ���ʼ����
  * @param  None
  * @retval None
*/
void IMU_SPI_Accel_Start(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);//���ͼ��ٶȼ�Ƭѡ
}

/**
  * @brief  SPI��ȡ���ٶȼƽ�������
  * @param  None
  * @retval None
*/
void IMU_SPI_Accel_Stop(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_4);//���߼��ٶȼ�Ƭѡ
}

/**
  * @brief  SPI��ȡ��������ʼ����
  * @param  None
  * @retval None
*/
void IMU_SPI_Gyro_Start(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);;//����������Ƭѡ
}

/**
  * @brief  SPI��ȡ�����ǽ�������
  * @param  None
  * @retval None
*/
void IMU_SPI_Gyro_Stop(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_0);//����������Ƭѡ
}

/**
  * @brief  SPI�����ֽں���
  * @param  None
  * @retval None
*/
uint8_t IMU_SPI_SwapByte(uint8_t ByteSend)
{
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)!=SET);//�ȴ�SPI���ͼĴ�����Ϊ��
	SPI_I2S_SendData(SPI1,ByteSend);//����
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)!=SET);//�ȴ�SPI�����ֽ����
	return SPI_I2S_ReceiveData(SPI1);//��ȡ���յ�����
}
