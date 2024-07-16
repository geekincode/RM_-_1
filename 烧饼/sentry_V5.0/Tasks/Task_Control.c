/**
  ******************************************************************************
  * Description : ����һ���߳̿��ƵĴ���
  *  
  ******************************************************************************
  * @attention	
  *
  *
  ******************************************************************************
*/


#include "Shared_Warehouse.h"


/* -------------------- ȫ�ֱ��� -------------------- */

//ִ�����ڼ��ṹ��
Execution_Count_Struct Execution_Count_t;



/* -------------------- ���ܳ�ʼ�� -------------------- */

//���ܳ�ʼ�����񷵻�ֵ
static rt_thread_t Fun_Init_thread;
//imu��ƮУ׼���񷵻�ֵ
static rt_thread_t IMU_Offset_thread;
//�������񷵻�ֵ
static rt_thread_t Sampling_thread;


/**
  * @brief  ���ܳ�ʼ��
  * @param  None
  * @retval None
*/
void Function_Init(void)
{
	/* ���ܳ�ʼ������ */
	Fun_Init_thread = rt_thread_create(
		"Fun_Init",						//�߳�����
		 Fun_Init_thread_entry,			//��ں���
		 RT_NULL,						//�̲߳���
		 500,							//��ջ��С
		 FUN_INIT_PRIORITY, 			//���ȼ�(ԽСԽ��)
		 1								//ʱ��Ƭ
	);
	
	/* IMU��ƮУ׼���� */
	IMU_Offset_thread = rt_thread_create(
		"IMU_Offset",					//�߳�����
		 IMU_Offset_thread_entry,		//��ں���
		 RT_NULL,						//�̲߳���
		 384,							//��ջ��С
		 IMU_OFFSET_PRIORITY, 			//���ȼ�(ԽСԽ��)
		 1								//ʱ��Ƭ
	);
	
	/* �������� */
	Sampling_thread = rt_thread_create(
		"Sampling",						//�߳�����
		 Sampling_thread_entry,			//��ں���
		 RT_NULL,						//�̲߳���
		 1500,							//��ջ��С
		 SAMPLING_PRIORITY, 			//���ȼ�(ԽСԽ��)
		 1								//ʱ��Ƭ
	);
	
	//���߳���ӵ�����������
	if(Fun_Init_thread != RT_NULL){rt_thread_startup(Fun_Init_thread);}
	if(IMU_Offset_thread != RT_NULL){rt_thread_startup(IMU_Offset_thread);}
	if(Sampling_thread != RT_NULL){rt_thread_startup(Sampling_thread);}
}



/* -------------------- �̳߳�ʼ�� -------------------- */

//�������񷵻�ֵ
static rt_thread_t Base_thread;
//��̨���񷵻�ֵ
static rt_thread_t Gimbal_thread;
//��λ��ͨ�����񷵻�ֵ
static rt_thread_t PC_Communicate_thread;

//���߳����񷵻�ֵ
static rt_thread_t Start_Main_thread;
//��������ģʽ���񷵻�ֵ
static rt_thread_t Turn_On_Standby_thread;
//����������񷵻�ֵ
static rt_thread_t Inspection_thread;

//���߳�������ں�������
void Start_Main_thread_entry(void *parameter);
//��������ģʽ��ں�������
void Turn_On_Standby_thread_entry(void *parameter);


/**
  * @brief  �̳߳�ʼ������
  * @param  None
  * @retval None
*/
void Task_Init(void)
{	
	/* ����������� */
	Inspection_thread = rt_thread_create(
		"Inspection",					//�߳�����
		 Inspection_thread_entry,		//��ں���
		 RT_NULL,						//�̲߳���
		 1500,							//��ջ��С
		 INSPECTION_PRIORITY, 			//���ȼ�(ԽСԽ��)
		 1								//ʱ��Ƭ
	);
	
	/* �������� */
	Base_thread = rt_thread_create(
		"Base",							//�߳�����
		 Base_thread_entry,				//��ں���
		 RT_NULL,						//�̲߳���
		 1500,							//��ջ��С
		 BASE_PRIORITY, 				//���ȼ�(ԽСԽ��)
		 1								//ʱ��Ƭ
	);
	
	/* ��̨���� */
	Gimbal_thread = rt_thread_create(
		"Gimbal",						//�߳�����
		 Gimbal_thread_entry,			//��ں���
		 RT_NULL,						//�̲߳���
		 1500,							//��ջ��С
		 GIMBAL_PRIORITY, 				//���ȼ�(ԽСԽ��)
		 1								//ʱ��Ƭ
	);
	
	/* ��λ��ͨ������ */
	PC_Communicate_thread = rt_thread_create(
		"PC_Communicate",				//�߳�����
		 PC_Communicate_thread_entry,	//��ں���
		 RT_NULL,						//�̲߳���
		 1500,							//��ջ��С
		 PC_COMMUNICATE_PRIORITY, 		//���ȼ�(ԽСԽ��)
		 1								//ʱ��Ƭ
	);
	
	/* ��������ģʽ���� */
	Turn_On_Standby_thread = rt_thread_create(
		"Turn_On_Standby",				//�߳�����
		 Turn_On_Standby_thread_entry,	//��ں���
		 RT_NULL,						//�̲߳���
		 384,							//��ջ��С
		 TURN_ON_STANDBY_PRIORITY, 		//���ȼ�(ԽСԽ��)
		 1								//ʱ��Ƭ
	);
}



/* -------------------- �߳̿��� -------------------- */

/**
  * @brief  ��������ģʽ
  * @param  None
  * @retval None
*/
void Turn_On_Standby_Mode(void)
{	
	//�ָ������߳�
	rt_thread_resume(Turn_On_Standby_thread);
}

/**
  * @brief  �������߳�
  * @param  None
  * @retval None
*/
void Start_Thread(void)
{
	/* �������߳����� */
	Start_Main_thread = rt_thread_create(
		"Start_Main",					//�߳�����
		 Start_Main_thread_entry,		//��ں���
		 RT_NULL,						//�̲߳���
		 256,							//��ջ��С
		 START_MAIN_PRIORITY, 			//���ȼ�(ԽСԽ��)
		 1								//ʱ��Ƭ
	);
	
	//���߳���ӵ�����������
	if(Start_Main_thread != RT_NULL){rt_thread_startup(Start_Main_thread);}
}



/* -------------------- �߳̿�����ں��� -------------------- */

//���߳�������ں���
void Start_Main_thread_entry(void *parameter)
{
	//���߳���ӵ�����������
	if(Inspection_thread != RT_NULL){rt_thread_startup(Inspection_thread);}
	if(PC_Communicate_thread != RT_NULL){rt_thread_startup(PC_Communicate_thread);}
	if(Base_thread != RT_NULL){rt_thread_startup(Base_thread);}
	if(Gimbal_thread != RT_NULL){rt_thread_startup(Gimbal_thread);}
	if(Turn_On_Standby_thread != RT_NULL){rt_thread_startup(Turn_On_Standby_thread);}
	//�����������
	rt_thread_suspend(Turn_On_Standby_thread);
}

//��������ģʽ��ں���
void Turn_On_Standby_thread_entry(void *parameter)
{
	while(1)
	{			
		//�ֶ��˳�����ģʽ
		if(rc_data.rc.s1!=1){rt_thread_suspend(Turn_On_Standby_thread);rt_schedule();}
	}
}
