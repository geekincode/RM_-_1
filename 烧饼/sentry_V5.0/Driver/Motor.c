/**
  ******************************************************************************
  * Description : 这是一坨电机驱动的代码
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- 全局变量 -------------------- */

//电机驱动电流
Motor_Current_Struct Motor_Current_t;



/* -------------------- 底盘电机驱动(CAN1总线) -------------------- */

/**
  * @brief  电机驱动
  * @param  底盘电机1234的电流值(-16384~16384)
  * @retval None
*/
void Base_MotorAll_SetSpeed(int16_t Motor1, int16_t Motor2, int16_t Motor3, int16_t Motor4)
{
	#if (MOTOR_SWITCH==1 && BASE_MOTOR_SWITCH==1)
	//发送的数据结构体
	CanTxMsg TxMessage;
	TxMessage.StdId=BASE_MOTOR_ALL_ID;//11位报文标准ID号
	TxMessage.ExtId=0;//29位扩展报文ID号
	TxMessage.IDE=CAN_Id_Standard;//使用标准ID号
	TxMessage.RTR=CAN_RTR_Data;//使用数据帧
	TxMessage.DLC=8;//数据帧的长度
	//发送的数据
	TxMessage.Data[0]=Motor1>>8;
	TxMessage.Data[1]=Motor1;
	TxMessage.Data[2]=Motor2>>8;
	TxMessage.Data[3]=Motor2;
	TxMessage.Data[4]=Motor3>>8;
	TxMessage.Data[5]=Motor3;
	TxMessage.Data[6]=Motor4>>8;
	TxMessage.Data[7]=Motor4;
	
	uint8_t box;//发送状态返回值
	//发送数据
	box=CAN_Transmit(CAN1, &TxMessage);//有返回值(返回发送邮箱号)
	
	//等待发送完成
	while(CAN_TransmitStatus(CAN1,box)==CAN_TxStatus_Failed);
	#endif
}


/* -------------------- 云台2006拨弹电机驱动(CAN2总线) -------------------- */

/**
  * @brief  电机驱动
  * @param  2006电机的电流值(-10000~10000)
  * @retval None
*/
void Gimbal_MotorAll_SetSpeed(int16_t Motor_Left, int16_t Motor_Right)
{
	#if (MOTOR_SWITCH==1 && SHOOT_MOTOR_SWITCH==1)
	//发送的数据结构体
	CanTxMsg TxMessage;
	TxMessage.StdId=GIMBAL_MOTOR_ALL_ID;//11位报文标准ID号
	TxMessage.ExtId=0;//29位扩展报文ID号
	TxMessage.IDE=CAN_Id_Standard;//使用标准ID号
	TxMessage.RTR=CAN_RTR_Data;//使用数据帧
	TxMessage.DLC=8;//数据帧的长度
	//发送的数据
	TxMessage.Data[0]=Motor_Left>>8;
	TxMessage.Data[1]=Motor_Left;
	TxMessage.Data[2]=Motor_Right>>8;
	TxMessage.Data[3]=Motor_Right;
	TxMessage.Data[4]=0;
	TxMessage.Data[5]=0;
	TxMessage.Data[6]=0;
	TxMessage.Data[7]=0;
	
	uint8_t box;//发送状态返回值
	//发送数据
	box=CAN_Transmit(CAN2, &TxMessage);//有返回值(返回发送邮箱号)
	
	//等待发送完成
	while(CAN_TransmitStatus(CAN2,box)==CAN_TxStatus_Failed);
	#endif
}


/* -------------------- Yaw轴6020驱动(CAN1总线) -------------------- */

/**
  * @brief  电机驱动
  * @param  Yaw轴6020电流值(-30000~30000)
  * @retval None
*/
void Yaw_Motor_SetSpeed(int16_t Yaw_Motor)
{
	#if (MOTOR_SWITCH==1 && YAW_MOTOR_SWITCH==1)
	//发送的数据结构体
	CanTxMsg TxMessage;
	TxMessage.StdId=YAW_PITCH_MOTOR_ALL_ID;//11位报文标准ID号
	TxMessage.ExtId=0;//29位扩展报文ID号
	TxMessage.IDE=CAN_Id_Standard;//使用标准ID号
	TxMessage.RTR=CAN_RTR_Data;//使用数据帧
	TxMessage.DLC=8;//数据帧的长度
	//发送的数据
	TxMessage.Data[0]=Yaw_Motor>>8;
	TxMessage.Data[1]=Yaw_Motor;
	TxMessage.Data[2]=0;
	TxMessage.Data[3]=0;
	TxMessage.Data[4]=0;
	TxMessage.Data[5]=0;
	TxMessage.Data[6]=0;
	TxMessage.Data[7]=0;
	
	uint8_t box;//发送状态返回值
	//发送数据
	box=CAN_Transmit(CAN1, &TxMessage);//有返回值(返回发送邮箱号)
	
	//等待发送完成
	while(CAN_TransmitStatus(CAN1,box)==CAN_TxStatus_Failed);
	#endif
}


/* -------------------- Pitch轴6020驱动(CAN2总线) -------------------- */

/**
  * @brief  电机驱动
  * @param  Pitch轴6020电流值(-30000~30000)
  * @retval None
*/
void Pitch_Motor_SetSpeed(int16_t Pitch_Right)
{
	#if (MOTOR_SWITCH==1 && PITCH_MOTOR_SWITCH==1)
	//发送的数据结构体
	CanTxMsg TxMessage;
	TxMessage.StdId=YAW_PITCH_MOTOR_ALL_ID;//11位报文标准ID号
	TxMessage.ExtId=0;//29位扩展报文ID号
	TxMessage.IDE=CAN_Id_Standard;//使用标准ID号
	TxMessage.RTR=CAN_RTR_Data;//使用数据帧
	TxMessage.DLC=8;//数据帧的长度
	//发送的数据
	TxMessage.Data[0]=Pitch_Right>>8;
	TxMessage.Data[1]=Pitch_Right;
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
	#endif
}
