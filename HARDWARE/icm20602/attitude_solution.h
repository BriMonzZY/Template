#ifndef __ATTITUDE_H
#define __ATTITUDE_H


typedef struct{
	float q0;
	float q1;
	float q2;
	float q3;
}quaterInfo_t;

typedef struct{
	float pitch;
	float roll;
	float yaw;
}eulerianAngles_t;

#if 0
	typedef struct{
		float values[10];
	}IMU;
#endif
	


extern float values[10];
extern eulerianAngles_t eulerAngle;

void IMU_getValues(float * values);
void IMU_quaterToEulerianAngles(void);

#endif
