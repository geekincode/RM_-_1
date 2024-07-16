#include "stm32f4xx.h"                  // Device header


/* -------------------- 陀螺仪底层驱动 -------------------- */

/**
  * @brief  SPI初始化
  * @param  None
  * @retval None
*/
void IMU_SPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能 SPI1 时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//GPIO时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//SPI1复用
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); //PA7复用为MOSI
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4复用为MISO
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3复用为SCLK
	
	//硬件SPI引脚
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);// 初始化PB3,4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);// 初始化PA7
	//模拟CSS引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);// 初始化PA4(加速度片选)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);// 初始化PB0(陀螺仪片选)
	
	//SPI配置
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//主模式
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//指定SPI的单向或双向数据模式：双线全双工
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//选择模式：CPOL=1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//选择模式：CPHA=2
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//指定时钟分频
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//指定SPI数据大小：8Byte
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//指定高位先行还是低位先行：高位先行
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//指定SS信号是由硬件或软件触发：软件
	SPI_InitStructure.SPI_CRCPolynomial = 10;//指定用于CRC计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  
	
	GPIO_SetBits(GPIOA,GPIO_Pin_4);//先将片选拉高
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	
	SPI_Cmd(SPI1, ENABLE); //使能 SPI1
}

/**
  * @brief  SPI读取加速度计起始条件
  * @param  None
  * @retval None
*/
void IMU_SPI_Accel_Start(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);//拉低加速度计片选
}

/**
  * @brief  SPI读取加速度计结束条件
  * @param  None
  * @retval None
*/
void IMU_SPI_Accel_Stop(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_4);//拉高加速度计片选
}

/**
  * @brief  SPI读取陀螺仪起始条件
  * @param  None
  * @retval None
*/
void IMU_SPI_Gyro_Start(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);;//拉低陀螺仪片选
}

/**
  * @brief  SPI读取陀螺仪结束条件
  * @param  None
  * @retval None
*/
void IMU_SPI_Gyro_Stop(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_0);//拉高陀螺仪片选
}

/**
  * @brief  SPI交换字节函数
  * @param  None
  * @retval None
*/
uint8_t IMU_SPI_SwapByte(uint8_t ByteSend)
{
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)!=SET);//等待SPI发送寄存器不为空
	SPI_I2S_SendData(SPI1,ByteSend);//发送
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)!=SET);//等待SPI交换字节完毕
	return SPI_I2S_ReceiveData(SPI1);//读取接收的数据
}
