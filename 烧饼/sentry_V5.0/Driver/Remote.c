/**
  ******************************************************************************
  * Description : 这是一坨用于接收遥控器数据的代码
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"


uint8_t originRemoteDatas[18];

float ch0_ratio,ch1_ratio,ch2_ratio,ch3_ratio,pull;

RC_Data_Struct rc_data;

void RC_Init(void)
{
	/* -------------- 清空配置 ----------------------------*/
	
	USART_DeInit(USART3);
	DMA_DeInit(DMA1_Stream1);
	
	/* -------------- 启用模块时钟源 ----------------------------*/
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11, GPIO_AF_USART3);
	
	/* -------------- 初始化GPIO ---------------------------------------*/

	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart3;
	gpio.GPIO_Pin = GPIO_Pin_11 ;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &gpio);
	
	/* -------------- 初始化USART ---------------------------------------*/
	 
	USART_InitTypeDef usart;
	usart.USART_BaudRate=100000;
	usart.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	usart.USART_Mode=USART_Mode_Rx;
	usart.USART_Parity=USART_Parity_Even;
	usart.USART_StopBits=USART_StopBits_1;
	usart.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART3,&usart);
	USART_Cmd(USART3,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	
	 /* -------------- 初始化NVIC ---------------------------------------*/
	
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = DMA1_Stream1_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	 
	 /* -------------- 初始化DMA -----------------------------------------*/
	 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	DMA_InitTypeDef dma;
	dma.DMA_BufferSize=18;
	dma.DMA_Channel=DMA_Channel_4;
	dma.DMA_DIR=DMA_DIR_PeripheralToMemory;
	dma.DMA_FIFOMode=DMA_FIFOMode_Disable;
	dma.DMA_FIFOThreshold=DMA_FIFOThreshold_1QuarterFull;
	dma.DMA_Memory0BaseAddr=(uint32_t)&originRemoteDatas;
	dma.DMA_MemoryBurst=DMA_MemoryBurst_Single;
	dma.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	dma.DMA_MemoryInc=DMA_MemoryInc_Enable;
	dma.DMA_Mode=DMA_Mode_Circular;
	dma.DMA_PeripheralBaseAddr=(uint32_t)&USART3->DR;
	dma.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
	dma.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	dma.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	dma.DMA_Priority=DMA_Priority_VeryHigh;
	DMA_Init(DMA1_Stream1,&dma);
	DMA_Cmd(DMA1_Stream1,ENABLE);
	DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);
}

void Remote_Init()
{
	RC_Init();
	Remote_StructInit();
}

void Remote_StructInit()
{
	rc_data.rc.ch0=RC_CH_VALUE_OFFSET;
	rc_data.rc.ch1=RC_CH_VALUE_OFFSET;
	rc_data.rc.ch2=RC_CH_VALUE_OFFSET;
	rc_data.rc.ch3=RC_CH_VALUE_OFFSET;
	rc_data.rc.pulley_wheel=RC_CH_VALUE_OFFSET;
	rc_data.rc.s1=RC_SW_MID;
	rc_data.rc.s2=RC_SW_MID;
}


void RemoteDataProcess(void)
{
	rc_data.rc.ch0 = (int16_t)(((int16_t)originRemoteDatas[0] | ((int16_t)originRemoteDatas[1] << 8)) & 0x07FF) -1024; 
	rc_data.rc.ch1 = (int16_t)((((int16_t)originRemoteDatas[1] >> 3) | ((int16_t)originRemoteDatas[2] << 5)) & 0x07FF) -1024;
	rc_data.rc.ch2 = (int16_t)((((int16_t)originRemoteDatas[2] >> 6) | ((int16_t)originRemoteDatas[3] << 2) |((int16_t)originRemoteDatas[4] << 10)) & 0x07FF) -1024;
	rc_data.rc.ch3 = (int16_t)((((int16_t)originRemoteDatas[4] >> 1) | ((int16_t)originRemoteDatas[5]<<7)) & 0x07FF) -1024;

	rc_data.rc.s1 = ((originRemoteDatas[5] >> 4) & 0x000C) >> 2;
	rc_data.rc.s2 = ((originRemoteDatas[5] >> 4) & 0x0003);
	rc_data.mouse.x = ((int16_t)originRemoteDatas[6]) | ((int16_t)originRemoteDatas[7] << 8);
	rc_data.mouse.y = ((int16_t)originRemoteDatas[8]) | ((int16_t)originRemoteDatas[9] << 8);
	rc_data.mouse.z = ((int16_t)originRemoteDatas[10]) | ((int16_t)originRemoteDatas[11] << 8); 
	rc_data.mouse.press_l = originRemoteDatas[12];
	rc_data.mouse.press_r = originRemoteDatas[13];
	rc_data.key.Key_v = ((int16_t)originRemoteDatas[14]) | ((int16_t)originRemoteDatas[15] << 8);
	rc_data.rc.pulley_wheel=-(int16_t)(((((int16_t)originRemoteDatas[16]) | ((int16_t)originRemoteDatas[17]<<8)) & 0x07FF) -1024);
	
	
	rc_data.key.Key_W=GetByteBit(rc_data.key.Key_v,0);
	rc_data.key.Key_S=GetByteBit(rc_data.key.Key_v,1);
	rc_data.key.Key_A=GetByteBit(rc_data.key.Key_v,2);
	rc_data.key.Key_D=GetByteBit(rc_data.key.Key_v,3);
	rc_data.key.Key_Shift=GetByteBit(rc_data.key.Key_v,4);
	rc_data.key.Key_Ctrl=GetByteBit(rc_data.key.Key_v,5);
	rc_data.key.Key_Q=GetByteBit(rc_data.key.Key_v,6);
	rc_data.key.Key_E=GetByteBit(rc_data.key.Key_v,7);
	rc_data.key.Key_R=GetByteBit(rc_data.key.Key_v,8);
	rc_data.key.Key_F=GetByteBit(rc_data.key.Key_v,9);
	rc_data.key.Key_G=GetByteBit(rc_data.key.Key_v,10);
	rc_data.key.Key_Z=GetByteBit(rc_data.key.Key_v,11);
	rc_data.key.Key_X=GetByteBit(rc_data.key.Key_v,12);
	rc_data.key.Key_C=GetByteBit(rc_data.key.Key_v,13);
	rc_data.key.Key_V=GetByteBit(rc_data.key.Key_v,14);
	rc_data.key.Key_B=GetByteBit(rc_data.key.Key_v,15);
	
	Remote_RCRatio_Cal();
}

void DMA1_Stream1_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_Stream1,DMA_FLAG_TCIF1)==SET)//当遥控DMA传输完成数据进行数据处理
	{	
		RemoteDataProcess();
		DMA_ClearITPendingBit(DMA1_Stream1,DMA_IT_TCIF1);
	}
}


/*计算每个遥控通道的比例*/
void Remote_RCRatio_Cal()
{
	ch0_ratio=fabs(rc_data.rc.ch0-RC_CH_VALUE_OFFSET)/RC_DeltaRCValue;
	ch1_ratio=fabs(rc_data.rc.ch1-RC_CH_VALUE_OFFSET)/RC_DeltaRCValue;
	ch2_ratio=fabs(rc_data.rc.ch2-RC_CH_VALUE_OFFSET)/RC_DeltaRCValue;
	ch3_ratio=fabs(rc_data.rc.ch3-RC_CH_VALUE_OFFSET)/RC_DeltaRCValue;
	pull=fabs(rc_data.rc.pulley_wheel-RC_CH_VALUE_OFFSET)/RC_DeltaRCValue;
	
	//置遥控器连接标志位
	Error_Detect_t.RC_Connected_Flag = 1;
}
