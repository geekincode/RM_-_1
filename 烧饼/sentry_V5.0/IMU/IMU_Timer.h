#ifndef __IMU_TIMER_H
#define __IMU_TIMER_H

//陀螺仪定时器初始化
void IMU_Timer_Init(void);
//开始计时
void IMU_Timer_Start_Count(void);
//停止计时并读数
uint32_t IMU_Timer_Stop_Count(void);
//读计数值
uint32_t IMU_Timer_Get_Count(void);

#endif
