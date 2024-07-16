/**
  ******************************************************************************
  * Description : ����һ���������Ĵ���
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"



/* -------------------- ȫ�ֱ��� -------------------- */

//�����������
Motor_Current_Struct Motor_Current_t;



/* -------------------- ���̵������(CAN1����) -------------------- */

/**
  * @brief  �������
  * @param  ���̵��1234�ĵ���ֵ(-16384~16384)
  * @retval None
*/
void Base_MotorAll_SetSpeed(int16_t Motor1, int16_t Motor2, int16_t Motor3, int16_t Motor4)
{
	#if (MOTOR_SWITCH==1 && BASE_MOTOR_SWITCH==1)
	//���͵����ݽṹ��
	CanTxMsg TxMessage;
	TxMessage.StdId=BASE_MOTOR_ALL_ID;//11λ���ı�׼ID��
	TxMessage.ExtId=0;//29λ��չ����ID��
	TxMessage.IDE=CAN_Id_Standard;//ʹ�ñ�׼ID��
	TxMessage.RTR=CAN_RTR_Data;//ʹ������֡
	TxMessage.DLC=8;//����֡�ĳ���
	//���͵�����
	TxMessage.Data[0]=Motor1>>8;
	TxMessage.Data[1]=Motor1;
	TxMessage.Data[2]=Motor2>>8;
	TxMessage.Data[3]=Motor2;
	TxMessage.Data[4]=Motor3>>8;
	TxMessage.Data[5]=Motor3;
	TxMessage.Data[6]=Motor4>>8;
	TxMessage.Data[7]=Motor4;
	
	uint8_t box;//����״̬����ֵ
	//��������
	box=CAN_Transmit(CAN1, &TxMessage);//�з���ֵ(���ط��������)
	
	//�ȴ��������
	while(CAN_TransmitStatus(CAN1,box)==CAN_TxStatus_Failed);
	#endif
}


/* -------------------- ��̨2006�����������(CAN2����) -------------------- */

/**
  * @brief  �������
  * @param  2006����ĵ���ֵ(-10000~10000)
  * @retval None
*/
void Gimbal_MotorAll_SetSpeed(int16_t Motor_Left, int16_t Motor_Right)
{
	#if (MOTOR_SWITCH==1 && SHOOT_MOTOR_SWITCH==1)
	//���͵����ݽṹ��
	CanTxMsg TxMessage;
	TxMessage.StdId=GIMBAL_MOTOR_ALL_ID;//11λ���ı�׼ID��
	TxMessage.ExtId=0;//29λ��չ����ID��
	TxMessage.IDE=CAN_Id_Standard;//ʹ�ñ�׼ID��
	TxMessage.RTR=CAN_RTR_Data;//ʹ������֡
	TxMessage.DLC=8;//����֡�ĳ���
	//���͵�����
	TxMessage.Data[0]=Motor_Left>>8;
	TxMessage.Data[1]=Motor_Left;
	TxMessage.Data[2]=Motor_Right>>8;
	TxMessage.Data[3]=Motor_Right;
	TxMessage.Data[4]=0;
	TxMessage.Data[5]=0;
	TxMessage.Data[6]=0;
	TxMessage.Data[7]=0;
	
	uint8_t box;//����״̬����ֵ
	//��������
	box=CAN_Transmit(CAN2, &TxMessage);//�з���ֵ(���ط��������)
	
	//�ȴ��������
	while(CAN_TransmitStatus(CAN2,box)==CAN_TxStatus_Failed);
	#endif
}


/* -------------------- Yaw��6020����(CAN1����) -------------------- */

/**
  * @brief  �������
  * @param  Yaw��6020����ֵ(-30000~30000)
  * @retval None
*/
void Yaw_Motor_SetSpeed(int16_t Yaw_Motor)
{
	#if (MOTOR_SWITCH==1 && YAW_MOTOR_SWITCH==1)
	//���͵����ݽṹ��
	CanTxMsg TxMessage;
	TxMessage.StdId=YAW_PITCH_MOTOR_ALL_ID;//11λ���ı�׼ID��
	TxMessage.ExtId=0;//29λ��չ����ID��
	TxMessage.IDE=CAN_Id_Standard;//ʹ�ñ�׼ID��
	TxMessage.RTR=CAN_RTR_Data;//ʹ������֡
	TxMessage.DLC=8;//����֡�ĳ���
	//���͵�����
	TxMessage.Data[0]=Yaw_Motor>>8;
	TxMessage.Data[1]=Yaw_Motor;
	TxMessage.Data[2]=0;
	TxMessage.Data[3]=0;
	TxMessage.Data[4]=0;
	TxMessage.Data[5]=0;
	TxMessage.Data[6]=0;
	TxMessage.Data[7]=0;
	
	uint8_t box;//����״̬����ֵ
	//��������
	box=CAN_Transmit(CAN1, &TxMessage);//�з���ֵ(���ط��������)
	
	//�ȴ��������
	while(CAN_TransmitStatus(CAN1,box)==CAN_TxStatus_Failed);
	#endif
}


/* -------------------- Pitch��6020����(CAN2����) -------------------- */

/**
  * @brief  �������
  * @param  Pitch��6020����ֵ(-30000~30000)
  * @retval None
*/
void Pitch_Motor_SetSpeed(int16_t Pitch_Right)
{
	#if (MOTOR_SWITCH==1 && PITCH_MOTOR_SWITCH==1)
	//���͵����ݽṹ��
	CanTxMsg TxMessage;
	TxMessage.StdId=YAW_PITCH_MOTOR_ALL_ID;//11λ���ı�׼ID��
	TxMessage.ExtId=0;//29λ��չ����ID��
	TxMessage.IDE=CAN_Id_Standard;//ʹ�ñ�׼ID��
	TxMessage.RTR=CAN_RTR_Data;//ʹ������֡
	TxMessage.DLC=8;//����֡�ĳ���
	//���͵�����
	TxMessage.Data[0]=Pitch_Right>>8;
	TxMessage.Data[1]=Pitch_Right;
	TxMessage.Data[2]=0;
	TxMessage.Data[3]=0;
	TxMessage.Data[4]=0;
	TxMessage.Data[5]=0;
	TxMessage.Data[6]=0;
	TxMessage.Data[7]=0;
	
	
	uint8_t box;//����״̬����ֵ
	//��������
	box=CAN_Transmit(CAN2, &TxMessage);//�з���ֵ(���ط��������)
	
	//�ȴ��������
	while(CAN_TransmitStatus(CAN2,box)==CAN_TxStatus_Failed);
	#endif
}
