#include "stm32f4xx.h"                  // Device header
#include "Judge_Task.h"



/* -------------------- 全局变量 -------------------- */

//存储CAN1接收数据
CanRxMsg MyCAN1_Rece_Data;//存储接收的数据
uint8_t MyCAN1_Rece_Flag;//接收数据标志位
//存储CAN2接收数据
CanRxMsg MyCAN2_Rece_Data;//存储接收的数据
uint8_t MyCAN2_Rece_Flag;//接收数据标志位



/* -------------------- 初始化CAN1,CAN2 -------------------- */

/**
  * @brief  配置CAN的模式
  * @param  None
  * @retval None
*/
void MyCAN_Mode_Config(void)
{
	//CAN总线初始化
	CAN_InitTypeDef CAN_InitStruct;
	//工作模式配置
	CAN_InitStruct.CAN_Mode=CAN_Mode_Normal;//正常模式
	//CAN_InitStruct.CAN_Mode=CAN_Mode_LoopBack;//回环测试
	//波特率配置
	CAN_InitStruct.CAN_Prescaler=5;//CAN外设时钟分频(1~1024)
	CAN_InitStruct.CAN_SJW=CAN_SJW_4tq;//配置CAN重新同步每次可增加或缩短的最大长度(1~4Tq)
	CAN_InitStruct.CAN_BS1=CAN_BS1_5tq;//配置BS1段的长度(1~16Tq)
	CAN_InitStruct.CAN_BS2=CAN_BS2_3tq;//配置BS2段的长度(1~8Tq)
	//功能开关
	CAN_InitStruct.CAN_TTCM=DISABLE;//是否使用时间触发功能(加入时间戳,一般不用)
	CAN_InitStruct.CAN_ABOM=ENABLE;//是否使用自动离线管理功能(节点在适当的时候退出离线)
	CAN_InitStruct.CAN_AWUM=ENABLE;//是否使用自动唤醒功能(总线有活动时可以自动退出低功耗)
	CAN_InitStruct.CAN_NART=DISABLE;//是否使用自动重传功能
	CAN_InitStruct.CAN_RFLM=DISABLE;//是否使用接收锁定功能(6个接收邮箱填满之后会不再接收报文)
	CAN_InitStruct.CAN_TXFP=DISABLE;//报文优先级判定方法  DISABLE:根据报文优先级  ENABLE:根据报文存入邮箱的先后顺序
	
	//CAN1初始化
	CAN_Init(CAN1,&CAN_InitStruct);
	//CAN2初始化
	CAN_Init(CAN2,&CAN_InitStruct);
}

/**
  * @brief  筛选器初始化
  * @param  None
  * @retval None
*/
void MyCAN_Filter_Config(void)
{
	//CAN筛选器
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	CAN_FilterInitStruct.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStruct.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh=0x0000;//CAN_FxR2寄存器高16位
	CAN_FilterInitStruct.CAN_FilterMaskIdLow=0x0000;//CAN_FxR2寄存器低16位
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//设置筛选后数据存储到FIFO0
	CAN_FilterInitStruct.CAN_FilterNumber=0;//筛选器编号(f103:0~13  f407:0~27)
	CAN_FilterInitStruct.CAN_FilterMode=CAN_FilterMode_IdMask;//筛选器模式选择掩码模式
	CAN_FilterInitStruct.CAN_FilterScale=CAN_FilterScale_32bit;//筛选器尺度32bit
	CAN_FilterInitStruct.CAN_FilterActivation=ENABLE;//是否使能本筛选器
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	//CAN2筛选器
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment=CAN_Filter_FIFO1;//设置筛选后数据存储到FIFO1
	CAN_FilterInitStruct.CAN_FilterNumber=14;//筛选器编号(f103:0~13  f407:0~27)
	CAN_FilterInitStruct.CAN_FilterActivation=ENABLE;//是否使能本筛选器
	CAN_FilterInit(&CAN_FilterInitStruct);
}

/**
  * @brief  接收中断初始化
  * @param  None
  * @retval None
*/
void MyCAN_NVIC_Config(void)
{
	//CAN1中断初始化
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=CAN1_RX0_IRQn;//选择中断通道(CAN1接收邮箱0的中断通道)
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;//指定抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;//指定响应优先级
	NVIC_Init(&NVIC_InitStruct);
	
	//CAN2中断初始化
	NVIC_InitStruct.NVIC_IRQChannel=CAN2_RX1_IRQn;//选择中断通道(CAN2接收邮箱1的中断通道)
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;//指定抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;//指定响应优先级
	NVIC_Init(&NVIC_InitStruct);
	
	//使能中断
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//CAN1邮箱0
	CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);//CAN2邮箱1
}

/**
  * @brief  CAN整体初始化函数
  * @param  None
  * @retval None
*/
void MyCAN_Init(void)
{
	//时钟初始化
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//CAN1的GPIO初始化
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//CAN2的GPIO初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//CAN1引脚复用映射配置
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); 
	//CAN2引脚复用映射配置
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_CAN2);	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_CAN2);
	
	//CAN模式初始化
	MyCAN_Mode_Config();
	//筛选器初始化
	MyCAN_Filter_Config();
	//接收中断初始化
	MyCAN_NVIC_Config();
}



/* -------------------- CAN1接收处理 -------------------- */

/**
  * @brief  接收中断初始化
  * @param  None
  * @retval None
*/
void Get_Master_Encode_Data(void)
{
	switch(MyCAN2_Rece_Data.StdId)
	{
		case 0x101:
			Judge_t.Friction=MyCAN2_Rece_Data.Data[0];//摩擦轮状态
			Judge_t.Gyro=MyCAN2_Rece_Data.Data[1];//小陀螺状态
			Judge_t.AimBot=MyCAN2_Rece_Data.Data[2];//自瞄状态
			Judge_t.Cap=MyCAN2_Rece_Data.Data[3];//超电开关
			Judge_t.Auto=MyCAN2_Rece_Data.Data[4];//自动开火状态
			break;
	}
}


/* -------------------- 接收中断 -------------------- */

//CAN1邮箱0接收中断
void CAN1_RX0_IRQHandler(void)
{
	//无须清除标志位
	CAN_Receive(CAN1,CAN_FIFO0,&MyCAN1_Rece_Data);//读取邮箱0的数据存储在CAN1_Rece_Data中
}

//CAN2邮箱1接收中断
void CAN2_RX1_IRQHandler(void)
{
	//无须清除标志位
	CAN_Receive(CAN2,CAN_FIFO1,&MyCAN2_Rece_Data);//读取邮箱1的数据存储在CAN2_Rece_Data中

	//接收数据
	Get_Master_Encode_Data();
}
