#ifndef __IMU_H
#define __IMU_H

#include "stdint.h"
#include "QuaternionEKF.h"
#include "IMU_Driver.h"
#include "IMU_Timer.h"

//��ź궨��
#define X 0
#define Y 1
#define Z 2

//��̬����ṹ��
typedef struct
{
    float q[4]; // ��Ԫ������ֵ

    float Gyro[3];  // ���ٶ�
    float Accel[3]; // ���ٶ�
    float MotionAccel_b[3]; // ����������ٶ�
    float MotionAccel_n[3]; // ����ϵ���ٶ�

    float AccelLPF; // ���ٶȵ�ͨ�˲�ϵ��

    // ���ٶ��ھ���ϵ��������ʾ
    float xn[3];
    float yn[3];
    float zn[3];

    float atanxz;
    float atanyz;

    // λ��
    float Roll;
    float Pitch;
    float Yaw;
    float YawTotalAngle;
} INS_t;

extern INS_t INS;
extern uint8_t IMU_Offset_Flag; //��ƮУ׼���

//�����ǳ�ʼ��
void IMU_Init(void);
//��̬�������
void IMU_Updata(void);
//imu��ƮУ׼�������
void IMU_Offset_thread_entry(void *parameter);

#endif
