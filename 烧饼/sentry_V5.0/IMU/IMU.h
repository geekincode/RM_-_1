#ifndef __IMU_H
#define __IMU_H

#include "stdint.h"
#include "QuaternionEKF.h"
#include "IMU_Driver.h"
#include "IMU_Timer.h"

//编号宏定义
#define X 0
#define Y 1
#define Z 2

//姿态解算结构体
typedef struct
{
    float q[4]; // 四元数估计值

    float Gyro[3];  // 角速度
    float Accel[3]; // 加速度
    float MotionAccel_b[3]; // 机体坐标加速度
    float MotionAccel_n[3]; // 绝对系加速度

    float AccelLPF; // 加速度低通滤波系数

    // 加速度在绝对系的向量表示
    float xn[3];
    float yn[3];
    float zn[3];

    float atanxz;
    float atanyz;

    // 位姿
    float Roll;
    float Pitch;
    float Yaw;
    float YawTotalAngle;
} INS_t;

extern INS_t INS;
extern uint8_t IMU_Offset_Flag; //零飘校准完成

//陀螺仪初始化
void IMU_Init(void);
//姿态解算更新
void IMU_Updata(void);
//imu零飘校准任务入口
void IMU_Offset_thread_entry(void *parameter);

#endif
