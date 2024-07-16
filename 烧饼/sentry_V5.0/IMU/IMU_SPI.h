#ifndef __IMU_SPI_H
#define __IMU_SPI_H

//陀螺仪初始化
void IMU_SPI_Init(void);

/* SPI起始结束条件 */
//SPI读取加速度计起始条件
void IMU_SPI_Accel_Start(void);
//SPI读取加速度计结束条件
void IMU_SPI_Accel_Stop(void);
//SPI读取陀螺仪起始条件
void IMU_SPI_Gyro_Start(void);
//SPI读取陀螺仪结束条件
void IMU_SPI_Gyro_Stop(void);

/* SPI交换字节时序 */
//SPI交换字节函数
uint8_t IMU_SPI_SwapByte(uint8_t ByteSend);

#endif
