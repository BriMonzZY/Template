#include "attitude_solution.h"
#include "SEEKFREE_ICM20602.h"
#include <math.h>

#define delta_T		0.005f  /* 采样周期 5ms计算一次 */
#define M_PI		3.1425f
 
float I_ex, I_ey, I_ez;  // 误差积分


quaterInfo_t Q_info = {1, 0, 0};  // 全局四元数
eulerianAngles_t eulerAngle; //欧拉角

float param_Kp = 50.0;   // 加速度计(磁力计)的收敛速率比例增益50 
float param_Ki = 0.20;   //陀螺仪收敛速率的积分增益 0.2
float values[10];

//float Q_info;
//float eulerAngle;


float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}


/*
	数据转换
	get accel and gyro from iam20609 
	对accel一阶低通滤波(参考匿名)，对gyro转成弧度每秒(2000dps)
*/

#define new_weight           0.35f
#define old_weight           0.65f

void IMU_getValues(float *values) 
{
    static double lastaccel[3]= {0,0,0};
    int i;
    values[0] = ((float)icm_acc_x) * new_weight + lastaccel[0] * old_weight;
    values[1] = ((float)icm_acc_y) * new_weight + lastaccel[1] * old_weight;
    values[2] = ((float)icm_acc_z) * new_weight + lastaccel[2] * old_weight;
    for(i=0; i<3; i++)
    {
        lastaccel[i] = values[i];
    }
 
    values[3] = ((float)icm_gyro_x) * M_PI / 180 / 16.4f;
    values[4] = ((float)icm_gyro_y) * M_PI / 180 / 16.4f;
    values[5] = ((float)icm_gyro_z) * M_PI / 180 / 16.4f;
}


#if 1
/**
  * brief IMU_AHRSupdate_noMagnetic  姿态解算融合，是Crazepony和核心算法
  * 使用的是互补滤波算法，没有使用Kalman滤波算法
  * param float gx, float gy, float gz, float ax, float ay, float az
  *
  * return None
  */
static void IMU_AHRSupdate_noMagnetic(float gx, float gy, float gz, float ax, float ay, float az)
{
	float halfT = 0.5f * delta_T;
	float vx, vy, vz;    //当前的机体坐标系上的重力单位向量
	float ex, ey, ez;    //四元数计算值与加速度计测量值的误差
	float q0 = Q_info.q0;
	float q1 = Q_info.q1;
	float q2 = Q_info.q2;
	float q3 = Q_info.q3;
	float q0q0 = q0 * q0;
	float q0q1 = q0 * q1;
	float q0q2 = q0 * q2;
	float q0q3 = q0 * q3;
	float q1q1 = q1 * q1;
	float q1q2 = q1 * q2;
	float q1q3 = q1 * q3;
	float q2q2 = q2 * q2;
	float q2q3 = q2 * q3;
	float q3q3 = q3 * q3;
	// float delta_2 = 0;
	
	//对加速度数据进行归一化 得到单位加速度
	float norm = invSqrt(ax*ax + ay*ay + az*az);       
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;
	vx = 2*(q1q3 - q0q2);
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;

	ex = ay * vz - az * vy;
	ey = az * vx - ax * vz;
	ez = ax * vy - ay * vx;
	
	//用叉乘误差来做PI修正陀螺零偏，
	//通过调节 param_Kp，param_Ki 两个参数，
	//可以控制加速度计修正陀螺仪积分姿态的速度。
	I_ex += delta_T * ex;   // integral error scaled by Ki
	I_ey += delta_T * ey;
	I_ez += delta_T * ez;

	gx = gx+ param_Kp*ex + param_Ki*I_ex;
	gy = gy+ param_Kp*ey + param_Ki*I_ey;
	gz = gz+ param_Kp*ez + param_Ki*I_ez;
	
	
	/*数据修正完成，下面是四元数微分方程*/
	
	
	//四元数微分方程，其中halfT为测量周期的1/2，gx gy gz为陀螺仪角速度，以下都是已知量，这里使用了一阶龙哥库塔求解四元数微分方程
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
	q1 = q1 + ( q0*gx + q2*gz - q3*gy)*halfT;
	q2 = q2 + ( q0*gy - q1*gz + q3*gx)*halfT;
	q3 = q3 + ( q0*gz + q1*gy - q2*gx)*halfT;
	//    delta_2=(2*halfT*gx)*(2*halfT*gx)+(2*halfT*gy)*(2*halfT*gy)+(2*halfT*gz)*(2*halfT*gz);
	// 整合四元数率    四元数微分方程  四元数更新算法，二阶毕卡法
	//    q0 = (1-delta_2/8)*q0 + (-q1*gx - q2*gy - q3*gz)*halfT;			
	//    q1 = (1-delta_2/8)*q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	//    q2 = (1-delta_2/8)*q2 + (q0*gy - q1*gz + q3*gx)*halfT;
	//    q3 = (1-delta_2/8)*q3 + (q0*gz + q1*gy - q2*gx)*halfT;	
	// normalise quaternion
	norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	Q_info.q0 = q0 * norm;
	Q_info.q1 = q1 * norm;
	Q_info.q2 = q2 * norm;
	Q_info.q3 = q3 * norm;
}
#endif



#if 0

//---------------------------------------------------------------------------------------------------
// Definitions

#define sampleFreq	200.0f			// sample frequency in Hz
#define twoKpDef	(2.0f * 0.5f)	// 2 * proportional gain
#define twoKiDef	(2.0f * 0.0f)	// 2 * integral gain

//---------------------------------------------------------------------------------------------------
// Variable definitions

volatile float twoKp = twoKpDef;											// 2 * proportional gain (Kp)
volatile float twoKi = twoKiDef;											// 2 * integral gain (Ki)
//volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;					// quaternion of sensor frame relative to auxiliary frame
float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};
volatile float integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;	// integral error terms scaled by Ki

void MahonyAHRSupdateIMU(float q[4], float gx, float gy, float gz, float ax, float ay, float az)
{
	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;
	float qa, qb, qc;

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    // 只在加速度计数据有效时才进行运算
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
        // 将加速度计得到的实际重力加速度向量v单位化
		recipNorm = invSqrt(ax * ax + ay * ay + az * az); //该函数返回平方根的倒数
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;        

		// Estimated direction of gravity
        // 通过四元数得到理论重力加速度向量g 
        // 注意，这里实际上是矩阵第三列*1/2，在开头对Kp Ki的宏定义均为2*增益
        // 这样处理目的是减少乘法运算量
		halfvx = q[1] * q[3] - q[0] * q[2];
		halfvy = q[0] * q[1] + q[2] * q[3];
		halfvz = q[0] * q[0] - 0.5f + q[3] * q[3];
	
		// Error is sum of cross product between estimated and measured direction of gravity
        // 对实际重力加速度向量v与理论重力加速度向量g做外积
		halfex = (ay * halfvz - az * halfvy);
		halfey = (az * halfvx - ax * halfvz);
		halfez = (ax * halfvy - ay * halfvx);

		// Compute and apply integral feedback if enabled
        // 在PI补偿器中积分项使能情况下计算并应用积分项
		if(twoKi > 0.0f) {
			// integral error scaled by Ki
			// 积分过程
			integralFBx += twoKi * halfex * (1.0f / sampleFreq);	
			integralFBy += twoKi * halfey * (1.0f / sampleFreq);
			integralFBz += twoKi * halfez * (1.0f / sampleFreq);
			
			// apply integral feedback
			// 应用误差补偿中的积分项
			gx += integralFBx;	
			gy += integralFBy;
			gz += integralFBz;
		}
		else {
            // prevent integral windup
            // 避免为负值的Ki时积分异常饱和
			integralFBx = 0.0f;	
			integralFBy = 0.0f;
			integralFBz = 0.0f;
		}

		// Apply proportional feedback
        // 应用误差补偿中的比例项
		gx += twoKp * halfex;
		gy += twoKp * halfey;
		gz += twoKp * halfez;
	}
	
	// Integrate rate of change of quaternion
    // 微分方程迭代求解
	gx *= (0.5f * (1.0f / sampleFreq));		// pre-multiply common factors
	gy *= (0.5f * (1.0f / sampleFreq));
	gz *= (0.5f * (1.0f / sampleFreq));
	qa = q[0];
	qb = q[1];
	qc = q[2];
	q[0] += (-qb * gx - qc * gy - q[3] * gz); 
	q[1] += (qa * gx + qc * gz - q[3] * gy);
	q[2] += (qa * gy - qb * gz + q[3] * gx);
	q[3] += (qa * gz + qb * gy - qc * gx); 
	
	// Normalise quaternion
    // 单位化四元数 保证四元数在迭代过程中保持单位性质
	recipNorm = invSqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
	q[0] *= recipNorm;
	q[1] *= recipNorm;
	q[2] *= recipNorm;
	q[3] *= recipNorm;
    
    //Mahony官方程序到此结束，使用时只需在函数外进行四元数反解欧拉角即可完成全部姿态解算过程
}

#endif


/**
 * @brief	将四元数转换成欧拉角
 * @param	None
 * @retval	None
 */
void IMU_quaterToEulerianAngles(void)
{
	IMU_getValues(values);
	IMU_AHRSupdate_noMagnetic(values[3], values[4], values[5], values[0], values[1], values[2]);
//	MahonyAHRSupdateIMU(q, values[3], values[4], values[5], values[0], values[1], values[2]);
	float q0 = Q_info.q0;
	float q1 = Q_info.q1;
	float q2 = Q_info.q2;
	float q3 = Q_info.q3;
	eulerAngle.pitch = asin(-2*q1*q3 + 2*q0*q2) * 180/M_PI; // pitch
	eulerAngle.roll = atan2(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2 + 1) * 180/M_PI; // roll
	eulerAngle.yaw = atan2(2*q1*q2 + 2*q0*q3, -2*q2*q2 - 2*q3*q3 + 1) * 180/M_PI; // yaw
//	eulerAngle.pitch = asin(-2*q[1]*q[3] + 2*q[0]*q[2]) * 180/M_PI; // pitch
//	eulerAngle.roll = atan2(2*q[2]*q[3] + 2*q[0]*q[1], -2*q[1]*q[1] - 2*q[2]*q[2] + 1) * 180/M_PI; // roll
//	eulerAngle.yaw = atan2(2*q[1]*q[2] + 2*q[0]*q[3], -2*q[2]*q[2] - 2*q[3]*q[3] + 1) * 180/M_PI; // yaw
 
	/*可以不用作姿态限度的限制*/
#if 0
    if(eulerAngle.roll>90 || eulerAngle.roll<-90)
    {
        if(eulerAngle.pitch > 0)
        {
            eulerAngle.pitch = 180-eulerAngle.pitch;
        }
        if(eulerAngle.pitch < 0)
        {
            eulerAngle.pitch = -(180+eulerAngle.pitch);
        }
    }
    if(eulerAngle.yaw > 180)
    {
        eulerAngle.yaw -=360;
    }
    else if(eulerAngle.yaw <-180)
    {
        eulerAngle.yaw +=360;
    }
#endif
}
