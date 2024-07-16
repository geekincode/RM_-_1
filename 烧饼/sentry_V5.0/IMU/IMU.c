#include "IMU.h"                  // Device header


/* -------------------- ȫ�ֱ��� -------------------- */

//��̬����ṹ��
INS_t INS;

//����ϵ������ת������������ϵ
const float xb[3] = {1, 0, 0};
const float yb[3] = {0, 1, 0};
const float zb[3] = {0, 0, 1};
//��̬����ʱ����
float dt = 0;
//��ƮУ׼��ɱ�־λ
uint8_t IMU_Offset_Flag;


/* -------------------- �������� -------------------- */

//��������ת��Ϊ�������
void BodyFrameToEarthFrame(const float *vecBF, float *vecEF, float *q);
//�������ת��Ϊ��������
void EarthFrameToBodyFrame(const float *vecEF, float *vecBF, float *q);



/* -------------------- ��̬���㺯���ӿ� -------------------- */

/**
  * @brief  �����ǳ�ʼ��
  * @param  None
  * @retval None
*/
void IMU_Init(void)
{
	//������������ʼ��
	IMU_Driver_Init();
	
	//�������˲���ʼ��
	IMU_QuaternionEKF_Init(10, 0.001, 10000000, 1, 0);
    INS.AccelLPF = 0.0085;
	
	//��ʼ�״μ�ʱ
	IMU_Timer_Start_Count();
}


/**
  * @brief  imu��ƮУ׼�������
  * @param  None
  * @retval None
*/
void IMU_Offset_thread_entry(void *parameter)
{
	//��ƮУ׼
	Calibrate_IMU_Offset(&BMI088);
	
	//��ƮУ׼���
	IMU_Offset_Flag=1;
}


/**
  * @brief  ��̬�������
  * @param  None
  * @retval None
*/
void IMU_Updata(void)
{
	//�������ٶ�Ĭ��ֵ
    const float gravity[3] = {0, 0, 9.81f};
	
	//������ʱ������
	uint32_t temp_dt = IMU_Timer_Stop_Count();
	//���������(��Ӳ������)
    dt = (float)(temp_dt)/1000000 + 0.000003;
	//��ʼ��ʱ
	IMU_Timer_Start_Count();

	//��ȡԭʼ����
	if(IMU_Offset_Flag) //��ƮУ׼���
	{
		IMU_Read();
	}
		
	INS.Accel[X] = BMI088.Accel[X];
	INS.Accel[Y] = BMI088.Accel[Y];
	INS.Accel[Z] = BMI088.Accel[Z];
	INS.Gyro[X] = BMI088.Gyro[X];
	INS.Gyro[Y] = BMI088.Gyro[Y];
	INS.Gyro[Z] = BMI088.Gyro[Z];

	// ���ĺ���,EKF������Ԫ��
	IMU_QuaternionEKF_Update(INS.Gyro[X], INS.Gyro[Y], INS.Gyro[Z], INS.Accel[X], INS.Accel[Y], INS.Accel[Z], dt);
	memcpy(INS.q, QEKF_INS.q, sizeof(QEKF_INS.q));

	// ����ϵ������ת������������ϵ������ѡȡ����ϵΪ����ϵ
	BodyFrameToEarthFrame(xb, INS.xn, INS.q);
	BodyFrameToEarthFrame(yb, INS.yn, INS.q);
	BodyFrameToEarthFrame(zb, INS.zn, INS.q);

	// �������ӵ�������ϵnת��������ϵb,�����ݼ��ٶȼ����ݼ����˶����ٶ�
	float gravity_b[3];
	EarthFrameToBodyFrame(gravity, gravity_b, INS.q);
	for (uint8_t i = 0; i < 3; i++) // ͬ����һ����ͨ�˲�
	{
		INS.MotionAccel_b[i] = (INS.Accel[i] - gravity_b[i]) * dt / (INS.AccelLPF + dt) + INS.MotionAccel_b[i] * INS.AccelLPF / (INS.AccelLPF + dt);
	}
	BodyFrameToEarthFrame(INS.MotionAccel_b, INS.MotionAccel_n, INS.q); // ת���ص���ϵn

	// ��ȡ��������
	INS.Yaw = QEKF_INS.Yaw;
	INS.Roll = QEKF_INS.Pitch;
	INS.Pitch = QEKF_INS.Roll;
	INS.YawTotalAngle = QEKF_INS.YawTotalAngle;
}


/**
 * @brief          ��������ת��Ϊ�������
 * @param[1]       vector in BodyFrame
 * @param[2]       vector in EarthFrame
 * @param[3]       quaternion
 */
void BodyFrameToEarthFrame(const float *vecBF, float *vecEF, float *q)
{
    vecEF[0] = 2.0f * ((0.5f - q[2] * q[2] - q[3] * q[3]) * vecBF[0] +
                       (q[1] * q[2] - q[0] * q[3]) * vecBF[1] +
                       (q[1] * q[3] + q[0] * q[2]) * vecBF[2]);

    vecEF[1] = 2.0f * ((q[1] * q[2] + q[0] * q[3]) * vecBF[0] +
                       (0.5f - q[1] * q[1] - q[3] * q[3]) * vecBF[1] +
                       (q[2] * q[3] - q[0] * q[1]) * vecBF[2]);

    vecEF[2] = 2.0f * ((q[1] * q[3] - q[0] * q[2]) * vecBF[0] +
                       (q[2] * q[3] + q[0] * q[1]) * vecBF[1] +
                       (0.5f - q[1] * q[1] - q[2] * q[2]) * vecBF[2]);
}

/**
 * @brief          �������ת��Ϊ��������
 * @param[1]       vector in EarthFrame
 * @param[2]       vector in BodyFrame
 * @param[3]       quaternion
 */
void EarthFrameToBodyFrame(const float *vecEF, float *vecBF, float *q)
{
    vecBF[0] = 2.0f * ((0.5f - q[2] * q[2] - q[3] * q[3]) * vecEF[0] +
                       (q[1] * q[2] + q[0] * q[3]) * vecEF[1] +
                       (q[1] * q[3] - q[0] * q[2]) * vecEF[2]);

    vecBF[1] = 2.0f * ((q[1] * q[2] - q[0] * q[3]) * vecEF[0] +
                       (0.5f - q[1] * q[1] - q[3] * q[3]) * vecEF[1] +
                       (q[2] * q[3] + q[0] * q[1]) * vecEF[2]);

    vecBF[2] = 2.0f * ((q[1] * q[3] + q[0] * q[2]) * vecEF[0] +
                       (q[2] * q[3] - q[0] * q[1]) * vecEF[1] +
                       (0.5f - q[1] * q[1] - q[2] * q[2]) * vecEF[2]);
}
