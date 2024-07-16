#include "IMU.h"                  // Device header


/* -------------------- 全局变量 -------------------- */

//姿态解算结构体
INS_t INS;

//机体系基向量转换到导航坐标系
const float xb[3] = {1, 0, 0};
const float yb[3] = {0, 1, 0};
const float zb[3] = {0, 0, 1};
//姿态解算时间间隔
float dt = 0;
//零飘校准完成标志位
uint8_t IMU_Offset_Flag;


/* -------------------- 函数声明 -------------------- */

//机体坐标转换为大地坐标
void BodyFrameToEarthFrame(const float *vecBF, float *vecEF, float *q);
//大地坐标转换为机体坐标
void EarthFrameToBodyFrame(const float *vecEF, float *vecBF, float *q);



/* -------------------- 姿态解算函数接口 -------------------- */

/**
  * @brief  陀螺仪初始化
  * @param  None
  * @retval None
*/
void IMU_Init(void)
{
	//陀螺仪驱动初始化
	IMU_Driver_Init();
	
	//卡尔曼滤波初始化
	IMU_QuaternionEKF_Init(10, 0.001, 10000000, 1, 0);
    INS.AccelLPF = 0.0085;
	
	//开始首次计时
	IMU_Timer_Start_Count();
}


/**
  * @brief  imu零飘校准任务入口
  * @param  None
  * @retval None
*/
void IMU_Offset_thread_entry(void *parameter)
{
	//零飘校准
	Calibrate_IMU_Offset(&BMI088);
	
	//零飘校准完成
	IMU_Offset_Flag=1;
}


/**
  * @brief  姿态解算更新
  * @param  None
  * @retval None
*/
void IMU_Updata(void)
{
	//重力加速度默认值
    const float gravity[3] = {0, 0, 9.81f};
	
	//结束计时并读数
	uint32_t temp_dt = IMU_Timer_Stop_Count();
	//计算解算间隔(加硬件补偿)
    dt = (float)(temp_dt)/1000000 + 0.000003;
	//开始计时
	IMU_Timer_Start_Count();

	//读取原始数据
	if(IMU_Offset_Flag) //零飘校准完成
	{
		IMU_Read();
	}
		
	INS.Accel[X] = BMI088.Accel[X];
	INS.Accel[Y] = BMI088.Accel[Y];
	INS.Accel[Z] = BMI088.Accel[Z];
	INS.Gyro[X] = BMI088.Gyro[X];
	INS.Gyro[Y] = BMI088.Gyro[Y];
	INS.Gyro[Z] = BMI088.Gyro[Z];

	// 核心函数,EKF更新四元数
	IMU_QuaternionEKF_Update(INS.Gyro[X], INS.Gyro[Y], INS.Gyro[Z], INS.Accel[X], INS.Accel[Y], INS.Accel[Z], dt);
	memcpy(INS.q, QEKF_INS.q, sizeof(QEKF_INS.q));

	// 机体系基向量转换到导航坐标系，本例选取惯性系为导航系
	BodyFrameToEarthFrame(xb, INS.xn, INS.q);
	BodyFrameToEarthFrame(yb, INS.yn, INS.q);
	BodyFrameToEarthFrame(zb, INS.zn, INS.q);

	// 将重力从导航坐标系n转换到机体系b,随后根据加速度计数据计算运动加速度
	float gravity_b[3];
	EarthFrameToBodyFrame(gravity, gravity_b, INS.q);
	for (uint8_t i = 0; i < 3; i++) // 同样过一个低通滤波
	{
		INS.MotionAccel_b[i] = (INS.Accel[i] - gravity_b[i]) * dt / (INS.AccelLPF + dt) + INS.MotionAccel_b[i] * INS.AccelLPF / (INS.AccelLPF + dt);
	}
	BodyFrameToEarthFrame(INS.MotionAccel_b, INS.MotionAccel_n, INS.q); // 转换回导航系n

	// 获取最终数据
	INS.Yaw = QEKF_INS.Yaw;
	INS.Roll = QEKF_INS.Pitch;
	INS.Pitch = QEKF_INS.Roll;
	INS.YawTotalAngle = QEKF_INS.YawTotalAngle;
}


/**
 * @brief          机体坐标转换为大地坐标
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
 * @brief          大地坐标转换为机体坐标
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
