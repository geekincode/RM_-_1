#ifndef __PID_H
#define __PID_H

//PID�ṹ��
typedef struct 
{
	float Bias[3];	//��ǰ���ϴ������ϴ����
	
	float Pout;		//P���
	float Iout;		//I���
	float Dout;		//D���

	float Max_Iout;	//�����޷�
	float Min_Iout;
	float Max_out;	//����޷�
	float Min_out;
	
	float Out_Put;	//���

	float Kp;		//PID����
	float Ki;
	float Kd;
	
	//����ȫ΢�ֵ�ͨ�˲���
	LowPass_Struct HSPID_Struct;
	
	//����
	float Blind_Spot;
	
}PID_Struct;


//PID����
void PID_clear(PID_Struct *pid);
//PID��ʼ��
void PID_Init(PID_Struct *pid,float kp,float ki,float kd,float Max_out,float Max_Iout,float HSPID_kl,float blind_spot);
//λ��ʽPID
float Pos_PID (PID_Struct *pid,float Target,float Now);
//����ʽPID
float Inc_PID(PID_Struct *pid,float Target,float Now);
//�ϵ��ʼ��ʱ��ֵ
void PID_All_Init(void);

#endif
