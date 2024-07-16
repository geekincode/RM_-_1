#ifndef __IMU_SPI_H
#define __IMU_SPI_H

//�����ǳ�ʼ��
void IMU_SPI_Init(void);

/* SPI��ʼ�������� */
//SPI��ȡ���ٶȼ���ʼ����
void IMU_SPI_Accel_Start(void);
//SPI��ȡ���ٶȼƽ�������
void IMU_SPI_Accel_Stop(void);
//SPI��ȡ��������ʼ����
void IMU_SPI_Gyro_Start(void);
//SPI��ȡ�����ǽ�������
void IMU_SPI_Gyro_Stop(void);

/* SPI�����ֽ�ʱ�� */
//SPI�����ֽں���
uint8_t IMU_SPI_SwapByte(uint8_t ByteSend);

#endif
