#include "attitude_solution.h"
#include "SEEKFREE_ICM20602.h"
#include <math.h>

#define delta_T		0.005f  /* �������� 5ms����һ�� */
#define M_PI		3.1425f
 
float I_ex, I_ey, I_ez;  // ������


quaterInfo_t Q_info = {1, 0, 0};  // ȫ����Ԫ��
eulerianAngles_t eulerAngle; //ŷ����

float param_Kp = 50.0;   // ���ٶȼ�(������)���������ʱ�������50 
float param_Ki = 0.20;   //�������������ʵĻ������� 0.2
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
	����ת��
	get accel and gyro from iam20609 
	��accelһ�׵�ͨ�˲�(�ο�����)����gyroת�ɻ���ÿ��(2000dps)
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
  * brief IMU_AHRSupdate_noMagnetic  ��̬�����ںϣ���Crazepony�ͺ����㷨
  * ʹ�õ��ǻ����˲��㷨��û��ʹ��Kalman�˲��㷨
  * param float gx, float gy, float gz, float ax, float ay, float az
  *
  * return None
  */
static void IMU_AHRSupdate_noMagnetic(float gx, float gy, float gz, float ax, float ay, float az)
{
	float halfT = 0.5f * delta_T;
	float vx, vy, vz;    //��ǰ�Ļ�������ϵ�ϵ�������λ����
	float ex, ey, ez;    //��Ԫ������ֵ����ٶȼƲ���ֵ�����
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
	
	//�Լ��ٶ����ݽ��й�һ�� �õ���λ���ٶ�
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
	
	//�ò���������PI����������ƫ��
	//ͨ������ param_Kp��param_Ki ����������
	//���Կ��Ƽ��ٶȼ����������ǻ�����̬���ٶȡ�
	I_ex += delta_T * ex;   // integral error scaled by Ki
	I_ey += delta_T * ey;
	I_ez += delta_T * ez;

	gx = gx+ param_Kp*ex + param_Ki*I_ex;
	gy = gy+ param_Kp*ey + param_Ki*I_ey;
	gz = gz+ param_Kp*ez + param_Ki*I_ez;
	
	
	/*����������ɣ���������Ԫ��΢�ַ���*/
	
	
	//��Ԫ��΢�ַ��̣�����halfTΪ�������ڵ�1/2��gx gy gzΪ�����ǽ��ٶȣ����¶�����֪��������ʹ����һ��������������Ԫ��΢�ַ���
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
	q1 = q1 + ( q0*gx + q2*gz - q3*gy)*halfT;
	q2 = q2 + ( q0*gy - q1*gz + q3*gx)*halfT;
	q3 = q3 + ( q0*gz + q1*gy - q2*gx)*halfT;
	//    delta_2=(2*halfT*gx)*(2*halfT*gx)+(2*halfT*gy)*(2*halfT*gy)+(2*halfT*gz)*(2*halfT*gz);
	// ������Ԫ����    ��Ԫ��΢�ַ���  ��Ԫ�������㷨�����ױϿ���
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
    // ֻ�ڼ��ٶȼ�������Чʱ�Ž�������
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
        // �����ٶȼƵõ���ʵ���������ٶ�����v��λ��
		recipNorm = invSqrt(ax * ax + ay * ay + az * az); //�ú�������ƽ�����ĵ���
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;        

		// Estimated direction of gravity
        // ͨ����Ԫ���õ������������ٶ�����g 
        // ע�⣬����ʵ�����Ǿ��������*1/2���ڿ�ͷ��Kp Ki�ĺ궨���Ϊ2*����
        // ��������Ŀ���Ǽ��ٳ˷�������
		halfvx = q[1] * q[3] - q[0] * q[2];
		halfvy = q[0] * q[1] + q[2] * q[3];
		halfvz = q[0] * q[0] - 0.5f + q[3] * q[3];
	
		// Error is sum of cross product between estimated and measured direction of gravity
        // ��ʵ���������ٶ�����v�������������ٶ�����g�����
		halfex = (ay * halfvz - az * halfvy);
		halfey = (az * halfvx - ax * halfvz);
		halfez = (ax * halfvy - ay * halfvx);

		// Compute and apply integral feedback if enabled
        // ��PI�������л�����ʹ������¼��㲢Ӧ�û�����
		if(twoKi > 0.0f) {
			// integral error scaled by Ki
			// ���ֹ���
			integralFBx += twoKi * halfex * (1.0f / sampleFreq);	
			integralFBy += twoKi * halfey * (1.0f / sampleFreq);
			integralFBz += twoKi * halfez * (1.0f / sampleFreq);
			
			// apply integral feedback
			// Ӧ�������еĻ�����
			gx += integralFBx;	
			gy += integralFBy;
			gz += integralFBz;
		}
		else {
            // prevent integral windup
            // ����Ϊ��ֵ��Kiʱ�����쳣����
			integralFBx = 0.0f;	
			integralFBy = 0.0f;
			integralFBz = 0.0f;
		}

		// Apply proportional feedback
        // Ӧ�������еı�����
		gx += twoKp * halfex;
		gy += twoKp * halfey;
		gz += twoKp * halfez;
	}
	
	// Integrate rate of change of quaternion
    // ΢�ַ��̵������
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
    // ��λ����Ԫ�� ��֤��Ԫ���ڵ��������б��ֵ�λ����
	recipNorm = invSqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
	q[0] *= recipNorm;
	q[1] *= recipNorm;
	q[2] *= recipNorm;
	q[3] *= recipNorm;
    
    //Mahony�ٷ����򵽴˽�����ʹ��ʱֻ���ں����������Ԫ������ŷ���Ǽ������ȫ����̬�������
}

#endif


/**
 * @brief	����Ԫ��ת����ŷ����
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
 
	/*���Բ�������̬�޶ȵ�����*/
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
