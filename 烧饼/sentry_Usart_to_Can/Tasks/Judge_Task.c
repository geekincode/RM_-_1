#include "stm32f4xx.h"                  // Device header
#include "rtthread.h"
#include "Judge_Task.h"
#include "Judge.h"



/* -------------------- ȫ�ֱ��� -------------------- */

//����ϵͳ��Ϣ�ṹ��
Judge_Struct Judge_t;



/* -------------------- �������� -------------------- */

//���ͻ�����״̬
void Send_Robot_State(void);
//���͹�����Ϣ
void Send_Rule_Information(void);
//���ͱ���״̬��Ϣ
void Send_Game_Progress(void);



/* -------------------- ����ϵͳ��Ϣ����������� -------------------- */

/**
  * @brief  ����ϵͳ��Ϣ�����������
  * @param  None
  * @retval None
*/
void UI_Protract_thread_entry(void *parameter)
{
	while(1)
	{
		//���Ͳ���ϵͳ����
		if(Judge_t.Judge_Receive_Flag==1)
		{
			//���ͻ�����״̬
			Send_Robot_State();
			
			//���͹�����Ϣ
			Send_Rule_Information();
			
			//��������״̬��Ϣ
			Send_Game_Progress();
		}
		
		//���ձ�־λ����
		Judge_t.Judge_Receive_Flag=0;
		
		//������ʱ
		rt_thread_mdelay(1);
	}
}


/* -------------------- ���Ͳ���ϵͳ���� -------------------- */

/**
  * @brief  ���ͻ�����״̬
  * @param  None
  * @retval None
*/
void Send_Robot_State(void)
{
	//���͵����ݽṹ��
	CanTxMsg TxMessage;
	TxMessage.StdId=0x001;//11λ���ı�׼ID��
	TxMessage.ExtId=0;//29λ��չ����ID��
	TxMessage.IDE=CAN_Id_Standard;//ʹ�ñ�׼ID��
	TxMessage.RTR=CAN_RTR_Data;//ʹ������֡
	TxMessage.DLC=8;//����֡�ĳ���
	//���͵�����
	TxMessage.Data[0]=(uint16_t)(Judge.power_heat_data.chassis_power*100)>>8;//���̹���*100
	TxMessage.Data[1]=(uint16_t)(Judge.power_heat_data.chassis_power*100);
	TxMessage.Data[2]=Judge.power_heat_data.chassis_volt>>8;//���̵�ѹ
	TxMessage.Data[3]=Judge.power_heat_data.chassis_volt;
	TxMessage.Data[4]=Judge.power_heat_data.chassis_power_buffer>>8;//������������
	TxMessage.Data[5]=Judge.power_heat_data.chassis_power_buffer;
	TxMessage.Data[6]=Judge.power_heat_data.shooter_id1_17mm_cooling_heat>>8;//ǹ������
	TxMessage.Data[7]=Judge.power_heat_data.shooter_id1_17mm_cooling_heat;
	
	uint8_t box;//����״̬����ֵ
	//��������
	box=CAN_Transmit(CAN2, &TxMessage);//�з���ֵ(���ط��������)
	
	//�ȴ��������
	while(CAN_TransmitStatus(CAN2,box)==CAN_TxStatus_Failed);
}


/**
  * @brief  ���͹�����Ϣ
  * @param  None
  * @retval None
*/
void Send_Rule_Information(void)
{
	//���͵����ݽṹ��
	CanTxMsg TxMessage;
	TxMessage.StdId=0x002;//11λ���ı�׼ID��
	TxMessage.ExtId=0;//29λ��չ����ID��
	TxMessage.IDE=CAN_Id_Standard;//ʹ�ñ�׼ID��
	TxMessage.RTR=CAN_RTR_Data;//ʹ������֡
	TxMessage.DLC=8;//����֡�ĳ���
	//���͵�����
	TxMessage.Data[0]=Judge.game_robot_status.shooter_id1_17mm_speed_limit>>8;//���̹�������
	TxMessage.Data[1]=Judge.game_robot_status.shooter_id1_17mm_speed_limit;
	TxMessage.Data[2]=Judge.game_robot_status.shooter_id1_17mm_speed_limit>>8;//ǹ����������
	TxMessage.Data[3]=Judge.game_robot_status.shooter_id1_17mm_speed_limit;
	TxMessage.Data[4]=Judge.game_robot_status.shooter_id1_17mm_cooling_limit>>8;//ǹ����������
	TxMessage.Data[5]=Judge.game_robot_status.shooter_id1_17mm_cooling_limit;
	TxMessage.Data[6]=Judge.game_robot_status.shooter_id1_17mm_cooling_rate>>8;//ǹ������ÿ����ȴֵ
	TxMessage.Data[7]=Judge.game_robot_status.shooter_id1_17mm_cooling_rate;
	
	uint8_t box;//����״̬����ֵ
	//��������
	box=CAN_Transmit(CAN2, &TxMessage);//�з���ֵ(���ط��������)
	
	//�ȴ��������
	while(CAN_TransmitStatus(CAN2,box)==CAN_TxStatus_Failed);
}


/**
  * @brief  ���ͱ���״̬��Ϣ
  * @param  None
  * @retval None
*/
void Send_Game_Progress(void)
{
	//���͵����ݽṹ��
	CanTxMsg TxMessage;
	TxMessage.StdId=0x003;//11λ���ı�׼ID��
	TxMessage.ExtId=0;//29λ��չ����ID��
	TxMessage.IDE=CAN_Id_Standard;//ʹ�ñ�׼ID��
	TxMessage.RTR=CAN_RTR_Data;//ʹ������֡
	TxMessage.DLC=8;//����֡�ĳ���
	//���͵�����
	TxMessage.Data[0]=Judge.game_status.game_progress;//����״̬
	TxMessage.Data[1]=0;
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
}
