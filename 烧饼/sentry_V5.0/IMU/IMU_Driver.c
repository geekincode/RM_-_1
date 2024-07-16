#include "stm32f4xx.h"                  // Device header
#include "IMU_Reg.h"
#include "IMU_SPI.h"
#include "IMU_Driver.h"
#include "IMU_Timer.h"
#include "IMU.h"
#include "kalman_filter.h"
#include "Usart.h"

//�Ĵ�����ַ & 0x7F -> ָ����ַд
//�Ĵ�����ַ | 0x80 -> ָ����ַ��


/* -------------------- ȫ�ֱ��� -------------------- */

//���ٶȼ�������
#define BMI088_ACCEL_6G_SEN 	0.00179443359375f
float BMI088_ACCEL_SEN = BMI088_ACCEL_6G_SEN;
//�����Ǽ�������
#define BMI088_GYRO_2000_SEN 	0.00106526443603169529841533860381f
float BMI088_GYRO_SEN = BMI088_GYRO_2000_SEN;

//�洢���ٶȼ�ƴ��ǰԭʼ����
uint8_t AccBuf[6];
//�洢������ƴ��ǰԭʼ����
uint8_t GyroBuf[8];

//������ԭʼ���ݽṹ��
IMU_Data_t BMI088;

//��������������
uint8_t res = 0;
static uint8_t write_reg_num = 0;
static uint8_t error = BMI088_NO_ERROR;
float gyroDiff[3], gNormDiff;


/* -------------------- SPI�м�� -------------------- */

/**
  * @brief  ���ٶȴ����־λ
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
  * @brief  �����Ǵ����־λ
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
  * @brief  ���ٶȼƶ��Ĵ���
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
  * @brief  ���ٶȼ�д�Ĵ���
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
  * @brief  �����Ǽƶ��Ĵ���
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
  * @brief  �����Ǽ�д�Ĵ���
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


/* -------------------- �����ǳ�ʼ������ -------------------- */

/**
  * @brief  ���ٶȼƳ�ʼ��
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
  * @brief  �����ǳ�ʼ��
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
  * @brief  IMU������ʼ��
  * @param  None
  * @retval None
*/
void IMU_Driver_Init(void)
{
	//imu��ʱ����ʼ��
	IMU_Timer_Init();
	//SPI��ʼ��(��ȡ���ٶȼƺ�������)
	IMU_SPI_Init();
	
	//���ٶȼƳ�ʼ��
	bmi088_accel_init();
	//�����ǳ�ʼ��
	bmi088_gyro_init();
}


/* -------------------- �����Ƕ�ȡ���� -------------------- */

/**
  * @brief  ��ȡ���ٶȼƵ�ֵ
  * @param  None
  * @retval None
*/
void Get_Acc_Num(void)
{
	IMU_SPI_Accel_Start();    //PA4��0��Ƭѡ���ٶȼ�
	IMU_SPI_SwapByte(0x12 | 0x80);    //Bit #0��Bit #1-7��Bit #0��1����ʾ��
	IMU_SPI_SwapByte(0xFF);    //Bit #8-15����Чֵ
	uint8_t i = 0;
	while (i < 6)
	{
		AccBuf[i]=IMU_SPI_SwapByte(0x55);
		i++;
	}
	IMU_SPI_Accel_Stop();    		//PA4��1��ȡ��Ƭѡ���ٶȼ�
	BMI088.Accel[0] = ((int16_t)((AccBuf[1]) << 8) | AccBuf[0]) * BMI088_ACCEL_SEN * BMI088.AccelScale;
	BMI088.Accel[1] = ((int16_t)((AccBuf[3]) << 8) | AccBuf[2]) * BMI088_ACCEL_SEN * BMI088.AccelScale;
	BMI088.Accel[2] = ((int16_t)((AccBuf[5]) << 8) | AccBuf[4]) * BMI088_ACCEL_SEN * BMI088.AccelScale;
}

/**
  * @brief  ��ȡ���ٶȼƵ�ֵ
  * @param  None
  * @retval None
*/
void Get_Gyro_Num(void)
{
	IMU_SPI_Gyro_Start();    //PB0��0��Ƭѡ������
	IMU_SPI_SwapByte(0x00 | 0x80);    //Bit #0��Bit #1-7��Bit #0��1����ʾ��
	uint8_t i = 0;
	while (i < 8)
	{
		GyroBuf[i]=IMU_SPI_SwapByte(0x55);
		i++;
	}
	IMU_SPI_Gyro_Stop();    //PB0��1��ȡ��Ƭѡ������
	if(GyroBuf[0] == 0x0F)	//GyroBuf[0]����GYRO_CHIP_ID��Ӧ��Ϊ0x0F���ж����Ƕ�ȡ�����ǲ��������ǵ�ֵ��
	{
		if(IMU_Offset_Flag) //��ƮУ׼���
		{
			BMI088.Gyro[0] = ((int16_t)((GyroBuf[3]) << 8) | GyroBuf[2]) * BMI088_GYRO_SEN - BMI088.GyroOffset[0];
			BMI088.Gyro[1] = ((int16_t)((GyroBuf[5]) << 8) | GyroBuf[4]) * BMI088_GYRO_SEN - BMI088.GyroOffset[1];
			BMI088.Gyro[2] = ((int16_t)((GyroBuf[7]) << 8) | GyroBuf[6]) * BMI088_GYRO_SEN - BMI088.GyroOffset[2];
		}
		else //��ƮУ׼δ���
		{
			BMI088.Gyro[0] = ((int16_t)((GyroBuf[3]) << 8) | GyroBuf[2]) * BMI088_GYRO_SEN;
			BMI088.Gyro[1] = ((int16_t)((GyroBuf[5]) << 8) | GyroBuf[4]) * BMI088_GYRO_SEN;
			BMI088.Gyro[2] = ((int16_t)((GyroBuf[7]) << 8) | GyroBuf[6]) * BMI088_GYRO_SEN;
		}
	}
}

/**
  * @brief  �����Ƕ�ȡ���ݽӿ�
  * @param  None
  * @retval None
*/
void IMU_Read(void)
{
	//���ٶȼ�
	Get_Acc_Num();
	//������
	Get_Gyro_Num();
}

/**
  * @brief  ��׼��Ʈ
  * @param  None
  * @retval None
*/
void Calibrate_IMU_Offset(IMU_Data_t *bmi088)
{
    static float startTime;
    static uint16_t CaliTimes = 6000; // ��Ҫ�㹻������ݲ��ܵõ���Ч��������ƫУ׼���
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

            // ��¼���ݼ���
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

            // ���ݲ��������Ϊ�յ������ţ�������У׼
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

        // ȡƽ��ֵ�õ��궨���
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

    // ���ݱ궨���У׼���ٶȼƱ������
    bmi088->AccelScale = 9.81f / bmi088->gNorm;
}
