#ifndef __FILTER_H
#define __FILTER_H

//һ�׵�ͨ�˲��ṹ��
typedef struct 
{
	float Last_num;	//�ϴβ�������
	
	float KL;		//�˲�����
	
	float OutPut;	//�˲����
	
}LowPass_Struct;


//һ�׵�ͨ�˲���ʽ
float Low_Pass_Filter(LowPass_Struct *filter,float num);
//һ�׵�ͨ�˲���ʼ��
void Low_Pass_Filter_Init(void);
//һ�׵�ͨ�˲�����
void Low_Pass_Filter_Clear(LowPass_Struct *filter);

#endif
