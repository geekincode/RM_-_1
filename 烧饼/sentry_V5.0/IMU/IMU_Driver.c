#include "stm32f4xx.h"                  // Device header
#include "IMU_Reg.h"
#include "IMU_SPI.h"
#include "IMU_Driver.h"
#include "IMU_Timer.h"
#include "IMU.h"
#include "kalman_filter.h"
#include "Usart.h"

//寄存器地址 & 0x7F -> 指定地址写
//寄存器地址 | 0x80 -> 指定地址读


/* -------------------- 全局变量 -------------------- */

//加速度计灵敏度
#define BMI088_ACCEL_6G_SEN 	0.00179443359375f
float BMI088_ACCEL_SEN = BMI088_ACCEL_6G_SEN;
//陀螺仪计灵敏度
#define BMI088_GYRO_2000_SEN 	0.00106526443603169529841533860381f
float BMI088_GYRO_SEN = BMI088_GYRO_2000_SEN;

//存储加速度计拼接前原始数据
uint8_t AccBuf[6];
//存储陀螺仪拼接前原始数据
uint8_t GyroBuf[8];

//陀螺仪原始数据结构体
IMU_Data_t BMI088;

//陀螺仪驱动变量
uint8_t res = 0;
static uint8_t write_reg_num = 0;
static uint8_t error = BMI088_NO_ERROR;
float gyroDiff[3], gNormDiff;


/* -------------------- SPI中间层 -------------------- */

/**
  * @brief  加速度错误标志位
  * @param  None
  * @retval None
*/
static uint8_t write_BMI088_accel_reg_data_error[BMI088_WRITE_ACCEL_REG_NUM][3] =
{
	{BMI088_ACC_PWR_CTRL, BMI088_ACC_ENABLE_ACC_ON, BMI088_ACC_PWR_CTRL_ERROR},
	{BMI088_ACC_PWR_CONF, BMI088_ACC_PWR_ACTIVE_MODE, BMI088_ACC_PWR_CONF_ERROR},
	{BMI088_ACC_CONF, BMI088_ACC_NORMAL | BMI088_ACC_800_HZ | BMI088_ACC_CONF_MUST_Set, BMI088_ACC_CONF_ERROR},
	{BMI088_ACC_RANGE, BMI088_ACC_RANGE_6G, BMI088_ACC_RANGE_ERROR},
	{BMI088_INT1_IO_CTRL, BMI088_ACC_INT1_IO_ENABLE | BMI088_ACC_INT1_GPIO_PP | BMI088_ACC_INT1_GPIO_LOW, BMI088_INT1_IO_CTRL_ERROR},
	{BMI088_INT_MAP_DATA, BMI088_ACC_INT1_DRDY_INTERRUPT, BMI088_INT_MAP_DATA_ERROR}
};

/**
  * @brief  陀螺仪错误标志位
  * @param  None
  * @retval None
*/
static uint8_t write_BMI088_gyro_reg_data_error[BMI088_WRITE_GYRO_REG_NUM][3] =
{
	{BMI088_GYRO_RANGE, BMI088_GYRO_2000, BMI088_GYRO_RANGE_ERROR},
	{BMI088_GYRO_BANDWIDTH, BMI088_GYRO_2000_230_HZ | BMI088_GYRO_BANDWIDTH_MUST_Set, BMI088_GYRO_BANDWIDTH_ERROR},
	{BMI088_GYRO_LPM1, BMI088_GYRO_NORMAL_MODE, BMI088_GYRO_LPM1_ERROR},
	{BMI088_GYRO_CTRL, BMI088_DRDY_ON, BMI088_GYRO_CTRL_ERROR},
	{BMI088_GYRO_INT3_INT4_IO_CONF, BMI088_GYRO_INT3_GPIO_PP | BMI088_GYRO_INT3_GPIO_LOW, BMI088_GYRO_INT3_INT4_IO_CONF_ERROR},
	{BMI088_GYRO_INT3_INT4_IO_MAP, BMI088_GYRO_DRDY_IO_INT3, BMI088_GYRO_INT3_INT4_IO_MAP_ERROR}
};

/**
  * @brief  加速度计读寄存器
  * @param  None
  * @retval None
*/
void BMI088_accel_read_single_reg(uint8_t reg)
{
	IMU_SPI_Accel_Start();
    IMU_SPI_SwapByte(reg | 0x80);
	IMU_SPI_SwapByte(0x55);
	res = IMU_SPI_SwapByte(0x55);
    IMU_SPI_Accel_Stop();
}

/**
  * @brief  加速度计写寄存器
  * @param  None
  * @retval None
*/
void BMI088_accel_write_single_reg(uint8_t reg, uint8_t data)
{
	IMU_SPI_Accel_Start();
    IMU_SPI_SwapByte(reg & 0x7F);
	IMU_SPI_SwapByte(data);
    IMU_SPI_Accel_Stop();
}

/**
  * @brief  陀螺仪计读寄存器
  * @param  None
  * @retval None
*/
void BMI088_gyro_read_single_reg(uint8_t reg)
{
	IMU_SPI_Gyro_Start();
    IMU_SPI_SwapByte(reg | 0x80);
	res = IMU_SPI_SwapByte(0x55);
    IMU_SPI_Gyro_Stop();
}

/**
  * @brief  陀螺仪计写寄存器
  * @param  None
  * @retval None
*/
void BMI088_gyro_write_single_reg(uint8_t reg, uint8_t data)
{
	IMU_SPI_Gyro_Start();
    IMU_SPI_SwapByte(reg & 0x7F);
	IMU_SPI_SwapByte(data);
    IMU_SPI_Gyro_Stop();
}


/* -------------------- 陀螺仪初始化配置 -------------------- */

/**
  * @brief  加速度计初始化
  * @param  None
  * @retval None
*/
uint8_t bmi088_accel_init(void)
{
    // check commiunication
    BMI088_accel_read_single_reg(BMI088_ACC_CHIP_ID);
    rt_thread_mdelay(1);
    BMI088_accel_read_single_reg(BMI088_ACC_CHIP_ID);
    rt_thread_mdelay(1);

    // accel software reset
    BMI088_accel_write_single_reg(BMI088_ACC_SOFTRESET, BMI088_ACC_SOFTRESET_VALUE);
    rt_thread_mdelay(BMI088_LONG_DELAY_TIME);

    // check commiunication is normal after reset
    BMI088_accel_read_single_reg(BMI088_ACC_CHIP_ID);
    rt_thread_mdelay(1);
    BMI088_accel_read_single_reg(BMI088_ACC_CHIP_ID);
    rt_thread_mdelay(1);

    // check the "who am I"
    if (res != BMI088_ACC_CHIP_ID_VALUE)
        return BMI088_NO_SENSOR;

    // set accel sonsor config and check
    for (write_reg_num = 0; write_reg_num < BMI088_WRITE_ACCEL_REG_NUM; write_reg_num++)
    {

        BMI088_accel_write_single_reg(write_BMI088_accel_reg_data_error[write_reg_num][0], write_BMI088_accel_reg_data_error[write_reg_num][1]);
        rt_thread_mdelay(1);

        BMI088_accel_read_single_reg(write_BMI088_accel_reg_data_error[write_reg_num][0]);
        rt_thread_mdelay(1);

        if (res != write_BMI088_accel_reg_data_error[write_reg_num][1])
        {
            // write_reg_num--;
            // return write_BMI088_accel_reg_data_error[write_reg_num][2];
            error |= write_BMI088_accel_reg_data_error[write_reg_num][2];
        }
    }
    return BMI088_NO_ERROR;
}

/**
  * @brief  陀螺仪初始化
  * @param  None
  * @retval None
*/
uint8_t bmi088_gyro_init(void)
{
    // check commiunication
    BMI088_gyro_read_single_reg(BMI088_GYRO_CHIP_ID);
    rt_thread_mdelay(1);
    BMI088_gyro_read_single_reg(BMI088_GYRO_CHIP_ID);
    rt_thread_mdelay(1);

    // reset the gyro sensor
    BMI088_gyro_write_single_reg(BMI088_GYRO_SOFTRESET, BMI088_GYRO_SOFTRESET_VALUE);
    rt_thread_mdelay(BMI088_LONG_DELAY_TIME);
    // check commiunication is normal after reset
    BMI088_gyro_read_single_reg(BMI088_GYRO_CHIP_ID);
    rt_thread_mdelay(1);
    BMI088_gyro_read_single_reg(BMI088_GYRO_CHIP_ID);
    rt_thread_mdelay(1);

    // check the "who am I"
    if (res != BMI088_GYRO_CHIP_ID_VALUE)
        return BMI088_NO_SENSOR;

    // set gyro sonsor config and check
    for (write_reg_num = 0; write_reg_num < BMI088_WRITE_GYRO_REG_NUM; write_reg_num++)
    {

        BMI088_gyro_write_single_reg(write_BMI088_gyro_reg_data_error[write_reg_num][0], write_BMI088_gyro_reg_data_error[write_reg_num][1]);
        rt_thread_mdelay(1);

        BMI088_gyro_read_single_reg(write_BMI088_gyro_reg_data_error[write_reg_num][0]);
        rt_thread_mdelay(1);

        if (res != write_BMI088_gyro_reg_data_error[write_reg_num][1])
        {
            write_reg_num--;
            // return write_BMI088_gyro_reg_data_error[write_reg_num][2];
            error |= write_BMI088_accel_reg_data_error[write_reg_num][2];
        }
    }

    return BMI088_NO_ERROR;
}

/**
  * @brief  IMU驱动初始化
  * @param  None
  * @retval None
*/
void IMU_Driver_Init(void)
{
	//imu定时器初始化
	IMU_Timer_Init();
	//SPI初始化(读取加速度计和陀螺仪)
	IMU_SPI_Init();
	
	//加速度计初始化
	bmi088_accel_init();
	//陀螺仪初始化
	bmi088_gyro_init();
}


/* -------------------- 陀螺仪读取数据 -------------------- */

/**
  * @brief  读取加速度计的值
  * @param  None
  * @retval None
*/
void Get_Acc_Num(void)
{
	IMU_SPI_Accel_Start();    //PA4置0，片选加速度计
	IMU_SPI_SwapByte(0x12 | 0x80);    //Bit #0和Bit #1-7，Bit #0是1，表示读
	IMU_SPI_SwapByte(0xFF);    //Bit #8-15，无效值
	uint8_t i = 0;
	while (i < 6)
	{
		AccBuf[i]=IMU_SPI_SwapByte(0x55);
		i++;
	}
	IMU_SPI_Accel_Stop();    		//PA4置1，取消片选加速度计
	BMI088.Accel[0] = ((int16_t)((AccBuf[1]) << 8) | AccBuf[0]) * BMI088_ACCEL_SEN * BMI088.AccelScale;
	BMI088.Accel[1] = ((int16_t)((AccBuf[3]) << 8) | AccBuf[2]) * BMI088_ACCEL_SEN * BMI088.AccelScale;
	BMI088.Accel[2] = ((int16_t)((AccBuf[5]) << 8) | AccBuf[4]) * BMI088_ACCEL_SEN * BMI088.AccelScale;
}

/**
  * @brief  读取加速度计的值
  * @param  None
  * @retval None
*/
void Get_Gyro_Num(void)
{
	IMU_SPI_Gyro_Start();    //PB0置0，片选陀螺仪
	IMU_SPI_SwapByte(0x00 | 0x80);    //Bit #0和Bit #1-7，Bit #0是1，表示读
	uint8_t i = 0;
	while (i < 8)
	{
		GyroBuf[i]=IMU_SPI_SwapByte(0x55);
		i++;
	}
	IMU_SPI_Gyro_Stop();    //PB0置1，取消片选陀螺仪
	if(GyroBuf[0] == 0x0F)	//GyroBuf[0]储存GYRO_CHIP_ID，应该为0x0F，判断我们读取到的是不是陀螺仪的值。
	{
		if(IMU_Offset_Flag) //零飘校准完成
		{
			BMI088.Gyro[0] = ((int16_t)((GyroBuf[3]) << 8) | GyroBuf[2]) * BMI088_GYRO_SEN - BMI088.GyroOffset[0];
			BMI088.Gyro[1] = ((int16_t)((GyroBuf[5]) << 8) | GyroBuf[4]) * BMI088_GYRO_SEN - BMI088.GyroOffset[1];
			BMI088.Gyro[2] = ((int16_t)((GyroBuf[7]) << 8) | GyroBuf[6]) * BMI088_GYRO_SEN - BMI088.GyroOffset[2];
		}
		else //零飘校准未完成
		{
			BMI088.Gyro[0] = ((int16_t)((GyroBuf[3]) << 8) | GyroBuf[2]) * BMI088_GYRO_SEN;
			BMI088.Gyro[1] = ((int16_t)((GyroBuf[5]) << 8) | GyroBuf[4]) * BMI088_GYRO_SEN;
			BMI088.Gyro[2] = ((int16_t)((GyroBuf[7]) << 8) | GyroBuf[6]) * BMI088_GYRO_SEN;
		}
	}
}

/**
  * @brief  陀螺仪读取数据接口
  * @param  None
  * @retval None
*/
void IMU_Read(void)
{
	//加速度计
	Get_Acc_Num();
	//陀螺仪
	Get_Gyro_Num();
}

/**
  * @brief  较准零飘
  * @param  None
  * @retval None
*/
void Calibrate_IMU_Offset(IMU_Data_t *bmi088)
{
    static float startTime;
    static uint16_t CaliTimes = 6000; // 需要足够多的数据才能得到有效陀螺仪零偏校准结果
    uint8_t buf[8] = {0, 0, 0, 0, 0, 0};
    int16_t bmi088_raw_temp;
    float gyroMax[3], gyroMin[3];
    float gNormTemp, gNormMax, gNormMin;
	bmi088->AccelScale=1;

    do
    {
        rt_thread_mdelay(5);
        bmi088->gNorm = 0;
        bmi088->GyroOffset[0] = 0;
        bmi088->GyroOffset[1] = 0;
        bmi088->GyroOffset[2] = 0;

        for (uint16_t i = 0; i < CaliTimes; i++)
        {
            Get_Acc_Num();
            gNormTemp = sqrtf(bmi088->Accel[0] * bmi088->Accel[0] +
                              bmi088->Accel[1] * bmi088->Accel[1] +
                              bmi088->Accel[2] * bmi088->Accel[2]);
            bmi088->gNorm += gNormTemp;
			
			Get_Gyro_Num();
			bmi088->GyroOffset[0] += bmi088->Gyro[0];
			bmi088->GyroOffset[1] += bmi088->Gyro[1];
			bmi088->GyroOffset[2] += bmi088->Gyro[2];

            // 记录数据极差
            if (i == 0)
            {
                gNormMax = gNormTemp;
                gNormMin = gNormTemp;
                for (uint8_t j = 0; j < 3; j++)
                {
                    gyroMax[j] = bmi088->Gyro[j];
                    gyroMin[j] = bmi088->Gyro[j];
                }
            }
            else
            {
                if (gNormTemp > gNormMax)
                    gNormMax = gNormTemp;
                if (gNormTemp < gNormMin)
                    gNormMin = gNormTemp;
                for (uint8_t j = 0; j < 3; j++)
                {
                    if (bmi088->Gyro[j] > gyroMax[j])
                        gyroMax[j] = bmi088->Gyro[j];
                    if (bmi088->Gyro[j] < gyroMin[j])
                        gyroMin[j] = bmi088->Gyro[j];
                }
            }

            // 数据差异过大认为收到外界干扰，需重新校准
            gNormDiff = gNormMax - gNormMin;
            for (uint8_t j = 0; j < 3; j++)
                gyroDiff[j] = gyroMax[j] - gyroMin[j];
            if (gNormDiff > 0.5f ||
                gyroDiff[0] > 0.15f ||
                gyroDiff[1] > 0.15f ||
                gyroDiff[2] > 0.15f)
                break;
            rt_thread_mdelay(1);
        }

        // 取平均值得到标定结果
        bmi088->gNorm /= (float)CaliTimes;
        for (uint8_t i = 0; i < 3; i++)
            bmi088->GyroOffset[i] /= (float)CaliTimes;

    } while (gNormDiff > 0.5f ||
             fabsf(bmi088->gNorm - 9.8f) > 0.5f ||
             gyroDiff[0] > 0.15f ||
             gyroDiff[1] > 0.15f ||
             gyroDiff[2] > 0.15f ||
             fabsf(bmi088->GyroOffset[0]) > 0.01f ||
             fabsf(bmi088->GyroOffset[1]) > 0.01f ||
             fabsf(bmi088->GyroOffset[2]) > 0.01f);

    // 根据标定结果校准加速度计标度因数
    bmi088->AccelScale = 9.81f / bmi088->gNorm;
}
