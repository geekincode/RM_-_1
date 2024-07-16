#include "RM_Client_UI.h"

/***********************************
	UI
屏幕中心
			x:961 y:550
字体大小：
			推荐 20
纵向间隔推荐: 
					30 
错误状态：		
			x:50 y:800 
超级电容：
			x:1360 y:470
底盘状态：
			x:900 y:900
射击状态：
			x:480 y:470
自瞄状态：
			x:480 y:440
***********************************/
static uint16_t Robot_ID = 0,Client_ID = 0;

uint8_t UI_Send_Buffer[200];
__IO int16_t UI_Seq  = 0; 
const static judge_t *judge;

void UI_Init (void)    
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Stream6);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);   
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_4; 
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)UI_Send_Buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = 70;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);

	DMA_Init(DMA1_Stream6,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);
	
	DMA_Cmd(DMA1_Stream6,DISABLE);
	DMA_Cmd(DMA1_Stream6,ENABLE);
	{
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	judge = Get_JUDGE_Data();
}

void DMA1_Stream6_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6) != RESET)
	{
		DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
	}
}

void UI_DMA_Send(uint8_t * send_data,uint16_t len)
{
	while (DMA_GetCurrDataCounter(DMA1_Stream6));
	memset(UI_Send_Buffer,0,200);
	memcpy((uint8_t *)UI_Send_Buffer,send_data,len);
	
	DMA_Cmd(DMA1_Stream6, DISABLE);  // 关闭DMA发送
	DMA_SetCurrDataCounter(DMA1_Stream6, len);// 设置发送长度
	DMA_Cmd(DMA1_Stream6, ENABLE);  // 启动DMA发送
	UI_Seq++;
	if(UI_Seq == 0xff)
	{
		UI_Seq = 0;
	}
	rt_thread_mdelay(50);
}

void UI_Get_ID(void)
{
	Robot_ID = judge->game_robot_status.robot_id;
	if(Robot_ID < 0x100)
		Client_ID = 0x100 + Robot_ID;
	else
		Client_ID = 0x64 + Robot_ID;
}


/******************************
****裁判系统客户端绘制 删除****
输入：
		board_num 图层数
		delete_mode 模式
******************************/	
void UI_delete(uint16_t board_num,uint16_t delete_mode)
{
	unsigned char *point;          
	uint16_t frametail=0xFFFF;                  
	UI_Delete Data = {0};

	point=(unsigned char *)&Data;

	Data.head.SOF=0xa5;
	Data.head.Data_Length=8;
	Data.head.Seq=UI_Seq;
	Data.head.CRC8=get_crc8_check_sum(point,4,0xFF);
	Data.head.CMD_ID=0x301;              

	Data.datahead.Data_ID = 0x100;
	Data.datahead.Sender_ID = Robot_ID;
	Data.datahead.Receiver_ID = Client_ID;	

	Data.Delete_mode = delete_mode;
	Data.layer = board_num; 

	Data.frametail = get_crc16_check_sum(point,sizeof(Data) - 2,frametail);

	append_crc16_check_sum(point,sizeof(Data));
	UI_DMA_Send((uint8_t*)&Data,sizeof(Data));
}



/******************************
****裁判系统客户端绘制 直线****
输入：
		mode  UI图形操作
		name  图形名字 					(uint8_t *)"name"
		start_x 起点x坐标
		start_y 起点y坐标
		end_x 终点x坐标
		end_y 终点y坐标
		board_num 图层数
		color 颜色
******************************/	
void UI_line_Data_Set(uint8_t mode,uint8_t *name,uint16_t start_x,uint16_t start_y,uint16_t end_x,uint16_t end_y,uint16_t line_width,uint16_t board_num,uint16_t color)
{
	unsigned char *point;          
	uint16_t frametail=0xFFFF;                  
	UI_Graph_Data Data = {0};

	point=(unsigned char *)&Data;

	Data.head.SOF=0xa5;
	Data.head.Data_Length=21;
	Data.head.Seq=UI_Seq;
	Data.head.CRC8=get_crc8_check_sum(point,4,0xFF);
	Data.head.CMD_ID=0x301;              

	Data.datahead.Data_ID = 0x101;
	Data.datahead.Sender_ID = Robot_ID;
	Data.datahead.Receiver_ID = Client_ID;	

	Data.graphic_name[0] = *(name); 
	Data.graphic_name[1] = *(name+1); 
	Data.graphic_name[2] = *(name+2);
	Data.operate_tpye = mode;
	Data.graphic_tpye = 0;
	Data.layer = board_num; 
	Data.color = color;
	Data.start_angle = 0;
	Data.end_angle = 0;
	Data.width = line_width; 
	Data.start_x = start_x;
	Data.start_y = start_y;
	Data.end_x = end_x;
	Data.end_y = end_y ;
	Data.radius	 = 0;
	
	append_crc16_check_sum(point,sizeof(Data));
	UI_DMA_Send((uint8_t*)&Data,sizeof(Data));

}


/******************************
****裁判系统客户端绘制 圆****
输入：
		mode  UI图形操作
		name  图形名字 					(uint8_t *)"name"
		start_x 起点x坐标
		start_y 起点y坐标
		end_x 终点x坐标
		end_y 终点y坐标
		board_num 图层数
		color 颜色
******************************/	
void UI_circle_Data_Set(uint8_t mode,uint8_t *name,uint16_t start_x,uint16_t start_y,uint16_t circle_radius,uint16_t line_width,uint16_t board_num,uint16_t color)
{
	unsigned char *point;          
	uint16_t frametail=0xFFFF;                  
	UI_Graph_Data Data = {0};

	point=(unsigned char *)&Data;

	Data.head.SOF=0xa5;
	Data.head.Data_Length=21;
	Data.head.Seq=UI_Seq;
	Data.head.CRC8=get_crc8_check_sum(point,4,0xFF);
	Data.head.CMD_ID=0x301;              

	Data.datahead.Data_ID = 0x101;
	Data.datahead.Sender_ID = Robot_ID;
	Data.datahead.Receiver_ID = Client_ID;	

	Data.graphic_name[0] = *(name); 
	Data.graphic_name[1] = *(name+1); 
	Data.graphic_name[2] = *(name+2);
	Data.operate_tpye = mode;
	Data.graphic_tpye = 2;
	Data.layer = board_num; 
	Data.color = color;
	Data.start_angle = 0;
	Data.end_angle = 0;
	Data.width = line_width; 
	Data.start_x = start_x;
	Data.start_y = start_y;
	Data.end_x = 0;
	Data.end_y = 0;
	Data.radius	 = circle_radius;
	
	append_crc16_check_sum(point,sizeof(Data));
	UI_Seq++;
	if(UI_Seq == 0xff)
	{
		UI_Seq = 0;
	}
	point = (uint8_t *)&(Data);
	for(int i = 0;i < sizeof(Data);i++)
	{
		USART2->DR = (*point & (uint16_t)0x01FF);
    while ((USART2->SR & USART_FLAG_TXE) == RESET);
		point++;		
	}
}



/******************************
****裁判系统客户端绘制字符****
输入：
		mode  UI图形操作
		name  图形名字 					(uint8_t *)"name"
		start_x 起点x坐标
		start_y 起点y坐标
		board_num 图层数
		color 颜色
		char_size 字体大小
		char_len 字符长度
		Buffer 字符             (uint8_t *)"buffer"
******************************/	
void UI_char_Data_Set(uint8_t mode,uint8_t *name,uint16_t start_x,uint16_t start_y,uint16_t board_num,uint16_t color,uint16_t char_size,uint16_t char_len,uint8_t * Buffer)
{
	unsigned char *point;          
	uint16_t frametail=0xFFFF;                  
	UI_char_Data Data = {0};

	point=(unsigned char *)&Data;

	Data.head.SOF=0xa5;
	Data.head.Data_Length=51;
	Data.head.Seq=UI_Seq;
	Data.head.CRC8=get_crc8_check_sum(point,4,0xFF);
	Data.head.CMD_ID=0x301;              

	Data.datahead.Data_ID = 0x110;
	Data.datahead.Sender_ID = Robot_ID;
	Data.datahead.Receiver_ID = Client_ID;	

	Data.graphic_name[0] = *(name); 
	Data.graphic_name[1] = *(name+1); 
	Data.graphic_name[2] = *(name+2);
	Data.operate_tpye = mode;
	Data.graphic_tpye = 7;
	Data.layer = board_num; 
	Data.color = color;
	Data.start_angle = char_size;
	Data.end_angle = char_len;
	Data.width = 2; 
	Data.start_x = start_x;
	Data.start_y = start_y;
	Data.end_x = 0;
	Data.end_y = 0;
	Data.radius	 = 0;
	
	for(int i = 0;i < char_len;i++)
	{
		Data.char_data[i] = *Buffer;
		Buffer++;
	}

	append_crc16_check_sum(point,sizeof(Data));
	UI_DMA_Send((uint8_t*)&Data,sizeof(Data));

}

/******************************
****裁判系统客户端绘制整数****
输入：
		mode  UI图形操作
		name  图形名字 					(uint8_t *)"name"
		start_x 起点x坐标
		start_y 起点y坐标
		board_num 图层数
		color 颜色
		int_size 字体大小
		data 32位整数，int32_t
******************************/	
void UI_int_Data_Set(uint8_t mode,uint8_t *name,uint16_t start_x,uint16_t start_y,uint16_t board_num,uint16_t color,uint16_t int_size,int32_t data)
{
	unsigned char *point;          
	uint16_t frametail=0xFFFF;                  
	UI_int_Data Data = {0};

	point=(unsigned char *)&Data;

	Data.head.SOF=0xa5;
	Data.head.Data_Length=21;
	Data.head.Seq=UI_Seq;
	Data.head.CRC8=get_crc8_check_sum(point,4,0xFF);
	Data.head.CMD_ID=0x301;              

	Data.datahead.Data_ID = 0x101;
	Data.datahead.Sender_ID = Robot_ID;
	Data.datahead.Receiver_ID = Client_ID;	

	Data.graphic_name[0] = *(name); 
	Data.graphic_name[1] = *(name+1); 
	Data.graphic_name[2] = *(name+2);
	Data.operate_tpye = mode;
	Data.graphic_tpye = 5;
	Data.layer = board_num; 
	Data.color = color;
	Data.start_angle = int_size;
	Data.end_angle = 0;
	Data.width = 2; 
	Data.start_x = start_x;
	Data.start_y = start_y;

	Data.Data_int = data * 1000;
	append_crc16_check_sum(point,sizeof(Data));
	UI_DMA_Send((uint8_t*)&Data,sizeof(Data));

}

/******************************
****裁判系统客户端绘制浮点数****
输入：
		mode  UI图形操作
		name  图形名字 					(uint8_t *)"name"
		start_x 起点x坐标
		start_y 起点y坐标
		board_num 图层数
		color 颜色
		int_size 字体大小
		data 32位浮点数，float
******************************/	
void UI_float_Data_Set(uint8_t mode,uint8_t *name,uint16_t start_x,uint16_t start_y,uint16_t board_num,uint16_t color,uint16_t float_size,float data)
{
	unsigned char *point;          
	uint16_t frametail=0xFFFF;                  
	UI_float_Data Data = {0};

	point=(unsigned char *)&Data;

	Data.head.SOF = 0xa5;
	Data.head.Data_Length = 21;
	Data.head.Seq=UI_Seq;
	Data.head.CRC8=get_crc8_check_sum(point,4,0xFF);
	Data.head.CMD_ID=0x301;              

	Data.datahead.Data_ID = 0x101;
	Data.datahead.Sender_ID = Robot_ID;
	Data.datahead.Receiver_ID = Client_ID;	

	Data.graphic_name[0] = *(name); 
	Data.graphic_name[1] = *(name+1); 
	Data.graphic_name[2] = *(name+2);
	Data.operate_tpye = mode;
	Data.graphic_tpye = 5;
	Data.layer = board_num; 
	Data.color = color;
	Data.start_angle = float_size;
	Data.end_angle = 1;
	Data.width = 2; 
	Data.start_x = start_x;
	Data.start_y = start_y;

	Data.Data_int = data * 1000.f;
	append_crc16_check_sum(point,sizeof(Data));
	UI_DMA_Send((uint8_t*)&Data,sizeof(Data));

}
