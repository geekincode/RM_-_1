#ifndef __IMU_TIMER_H
#define __IMU_TIMER_H

//�����Ƕ�ʱ����ʼ��
void IMU_Timer_Init(void);
//��ʼ��ʱ
void IMU_Timer_Start_Count(void);
//ֹͣ��ʱ������
uint32_t IMU_Timer_Stop_Count(void);
//������ֵ
uint32_t IMU_Timer_Get_Count(void);

#endif
