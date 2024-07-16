#ifndef __RM_CILENT_UI_H
#define __RM_CILENT_UI_H

#include "stm32f4xx.h"
#include "stdarg.h"
#include "judge.h"
#include "rtthread.h"
#include "judge_crc.h"

#pragma pack(1)            



/***UI图形操作***/
typedef enum
{
	UI_NOTHING = 0,             //空操作
	UI_STRUCT,                  //创建
	UI_UPDATA,                  //更新
	UI_DELETE,                  //删除
}UI_MODE;

/***UI图形操作***/
typedef enum
{
	UI_DELETE_NOTHING = 0,             //空操作
	UI_DELETE_ONLY,             //仅此一个图层
	UI_DELETE_ALL,             //所有
}UI_DELETE_MODE;

/***UI图形颜色***/
typedef enum
{ 
	UI_COLOR_MAIN = 0,          //红蓝主色
	UI_COLOR_YELLOW,            //黄色
	UI_COLOR_GREEN,             //绿色
	UI_COLOR_ORANGE,            //橘色
	UI_COLOR_PUEPLISH_RED,      //紫红色
	UI_COLOR_PINK,         			//粉色
	UI_COLOR_CYAN,        		  //青色
	UI_COLOR_BLACK,             //黑色
	UI_COLOR_WHITE,             //白色
}UI_COLOR;

typedef struct
{
   uint8_t SOF;                    
   uint16_t Data_Length;         
   uint8_t Seq;                    //包序号
   uint8_t CRC8;                   
   uint16_t CMD_ID;              
} UI_head_t;            

typedef struct
{
   uint16_t Data_ID;             
   uint16_t Sender_ID;            
   uint16_t Receiver_ID;          
} UI_Operate;         

typedef struct
{
	UI_head_t head;
	UI_Operate datahead;
  uint8_t Delete_mode;       
  uint8_t layer;   
	uint16_t frametail;		
} UI_Delete;

typedef struct
{
  uint8_t Delete_mode;       
  uint8_t layer;   
} UI_Delete1;


typedef struct
{ 
	UI_head_t head;
	UI_Operate datahead;
	uint8_t graphic_name[3]; 
	uint32_t operate_tpye:3; 
	uint32_t graphic_tpye:3; 
	uint32_t layer:4; 
	uint32_t color:4; 
	uint32_t start_angle:9;
	uint32_t end_angle:9;
	uint32_t width:10; 
	uint32_t start_x:11; 
	uint32_t start_y:11;
	uint32_t radius:10; 
	uint32_t end_x:11; 
	uint32_t end_y:11;   
	uint16_t frametail;	
} UI_Graph_Data;


typedef struct
{
	UI_head_t head;
	UI_Operate datahead;
	uint8_t graphic_name[3]; 
	uint32_t operate_tpye:3; 
	uint32_t graphic_tpye:3; 
	uint32_t layer:4; 
	uint32_t color:4; 
	uint32_t start_angle:9;
	uint32_t end_angle:9;
	uint32_t width:10; 
	uint32_t start_x:11; 
	uint32_t start_y:11;
	uint32_t radius:10; 
	uint32_t end_x:11; 
	uint32_t end_y:11; 
  uint8_t char_data[30];
	uint16_t frametail;
} UI_char_Data;                  

typedef struct
{ 
	UI_head_t head;
	UI_Operate datahead;
  uint8_t graphic_name[3]; 
  uint32_t operate_tpye:3; 
  uint32_t graphic_tpye:3; 
  uint32_t layer:4; 
  uint32_t color:4; 
  uint32_t start_angle:9;
  uint32_t end_angle:9;
  uint32_t width:10; 
  uint32_t start_x:11; 
  uint32_t start_y:11;
  int32_t Data_int;
	uint16_t frametail;	
} UI_int_Data;

typedef struct
{ 
	UI_head_t head;
	UI_Operate datahead;
	uint8_t graphic_name[3]; 
	uint32_t operate_tpye:3; 
	uint32_t graphic_tpye:3; 
	uint32_t layer:4; 
	uint32_t color:4; 
	uint32_t start_angle:9;
	uint32_t end_angle:9;
	uint32_t width:10; 
	uint32_t start_x:11; 
	uint32_t start_y:11;
	int32_t Data_int;            
	uint16_t frametail;
} UI_float_Data;


void UI_DMA_Send(uint8_t * send_data,uint16_t len);
void UI_Init (void);
void UI_Get_ID(void);
void UI_delete(uint16_t board_num,uint16_t delete_mode);
void UI_delete1(uint16_t board_num,uint16_t delete_mode);

void UI_line_Data_Set(uint8_t mode,uint8_t *name,uint16_t start_x,uint16_t start_y,uint16_t end_x,uint16_t end_y,uint16_t line_width,uint16_t board_num,uint16_t color);
void UI_circle_Data_Set(uint8_t mode,uint8_t *name,uint16_t start_x,uint16_t start_y,uint16_t circle_radius,uint16_t line_width,uint16_t board_num,uint16_t color);
void UI_char_Data_Set(uint8_t mode,uint8_t *name,uint16_t start_x,uint16_t start_y,uint16_t board_num,uint16_t color,uint16_t char_size,uint16_t char_len,uint8_t * Buffer);
void UI_int_Data_Set(uint8_t mode,uint8_t *name,uint16_t start_x,uint16_t start_y,uint16_t board_num,uint16_t color,uint16_t int_size,int32_t data);
void UI_float_Data_Set(uint8_t mode,uint8_t *name,uint16_t start_x,uint16_t start_y,uint16_t board_num,uint16_t color,uint16_t float_size,float data);

#endif  /*__RM_CILENT_UI_H*/
