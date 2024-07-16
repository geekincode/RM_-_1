#include "stm32f4xx.h"                  // Device header
#include "rtthread.h"
#include "Judge_Task.h"
#include "Judge.h"



/* -------------------- 全局变量 -------------------- */

//裁判系统信息结构体
Judge_Struct Judge_t;



/* -------------------- 函数声明 -------------------- */

//发送机器人状态
void Send_Robot_State(void);
//发送规则信息
void Send_Rule_Information(void);
//发送比赛状态信息
void Send_Game_Progress(void);



/* -------------------- 裁判系统信息发送任务入口 -------------------- */

/**
  * @brief  裁判系统信息发送任务入口
  * @param  None
  * @retval None
*/
void UI_Protract_thread_entry(void *parameter)
{
	while(1)
	{
		//发送裁判系统数据
		if(Judge_t.Judge_Receive_Flag==1)
		{
			//发送机器人状态
			Send_Robot_State();
			
			//发送规则信息
			Send_Rule_Information();
			
			//发布比赛状态信息
			Send_Game_Progress();
		}
		
		//接收标志位清零
		Judge_t.Judge_Receive_Flag=0;
		
		//任务延时
		rt_thread_mdelay(1);
	}
}


/* -------------------- 发送裁判系统数据 -------------------- */

/**
  * @brief  发送机器人状态
  * @param  None
  * @retval None
*/
void Send_Robot_State(void)
{
	//发送的数据结构体
	CanTxMsg TxMessage;
	TxMessage.StdId=0x001;//11位报文标准ID号
	TxMessage.ExtId=0;//29位扩展报文ID号
	TxMessage.IDE=CAN_Id_Standard;//使用标准ID号
	TxMessage.RTR=CAN_RTR_Data;//使用数据帧
	TxMessage.DLC=8;//数据帧的长度
	//发送的数据
	TxMessage.Data[0]=(uint16_t)(Judge.power_heat_data.chassis_power*100)>>8;//底盘功率*100
	TxMessage.Data[1]=(uint16_t)(Judge.power_heat_data.chassis_power*100);
	TxMessage.Data[2]=Judge.power_heat_data.chassis_volt>>8;//底盘电压
	TxMessage.Data[3]=Judge.power_heat_data.chassis_volt;
	TxMessage.Data[4]=Judge.power_heat_data.chassis_power_buffer>>8;//底盘能量缓冲
	TxMessage.Data[5]=Judge.power_heat_data.chassis_power_buffer;
	TxMessage.Data[6]=Judge.power_heat_data.shooter_id1_17mm_cooling_heat>>8;//枪口热量
	TxMessage.Data[7]=Judge.power_heat_data.shooter_id1_17mm_cooling_heat;
	
	uint8_t box;//发送状态返回值
	//发送数据
	box=CAN_Transmit(CAN2, &TxMessage);//有返回值(返回发送邮箱号)
	
	//等待发送完成
	while(CAN_TransmitStatus(CAN2,box)==CAN_TxStatus_Failed);
}


/**
  * @brief  发送规则信息
  * @param  None
  * @retval None
*/
void Send_Rule_Information(void)
{
	//发送的数据结构体
	CanTxMsg TxMessage;
	TxMessage.StdId=0x002;//11位报文标准ID号
	TxMessage.ExtId=0;//29位扩展报文ID号
	TxMessage.IDE=CAN_Id_Standard;//使用标准ID号
	TxMessage.RTR=CAN_RTR_Data;//使用数据帧
	TxMessage.DLC=8;//数据帧的长度
	//发送的数据
	TxMessage.Data[0]=Judge.game_robot_status.shooter_id1_17mm_speed_limit>>8;//底盘功率上限
	TxMessage.Data[1]=Judge.game_robot_status.shooter_id1_17mm_speed_limit;
	TxMessage.Data[2]=Judge.game_robot_status.shooter_id1_17mm_speed_limit>>8;//枪口射速上限
	TxMessage.Data[3]=Judge.game_robot_status.shooter_id1_17mm_speed_limit;
	TxMessage.Data[4]=Judge.game_robot_status.shooter_id1_17mm_cooling_limit>>8;//枪口热量上限
	TxMessage.Data[5]=Judge.game_robot_status.shooter_id1_17mm_cooling_limit;
	TxMessage.Data[6]=Judge.game_robot_status.shooter_id1_17mm_cooling_rate>>8;//枪口热量每秒冷却值
	TxMessage.Data[7]=Judge.game_robot_status.shooter_id1_17mm_cooling_rate;
	
	uint8_t box;//发送状态返回值
	//发送数据
	box=CAN_Transmit(CAN2, &TxMessage);//有返回值(返回发送邮箱号)
	
	//等待发送完成
	while(CAN_TransmitStatus(CAN2,box)==CAN_TxStatus_Failed);
}


/**
  * @brief  发送比赛状态信息
  * @param  None
  * @retval None
*/
void Send_Game_Progress(void)
{
	//发送的数据结构体
	CanTxMsg TxMessage;
	TxMessage.StdId=0x003;//11位报文标准ID号
	TxMessage.ExtId=0;//29位扩展报文ID号
	TxMessage.IDE=CAN_Id_Standard;//使用标准ID号
	TxMessage.RTR=CAN_RTR_Data;//使用数据帧
	TxMessage.DLC=8;//数据帧的长度
	//发送的数据
	TxMessage.Data[0]=Judge.game_status.game_progress;//比赛状态
	TxMessage.Data[1]=0;
	TxMessage.Data[2]=0;
	TxMessage.Data[3]=0;
	TxMessage.Data[4]=0;
	TxMessage.Data[5]=0;
	TxMessage.Data[6]=0;
	TxMessage.Data[7]=0;
	
	uint8_t box;//发送状态返回值
	//发送数据
	box=CAN_Transmit(CAN2, &TxMessage);//有返回值(返回发送邮箱号)
	
	//等待发送完成
	while(CAN_TransmitStatus(CAN2,box)==CAN_TxStatus_Failed);
}
