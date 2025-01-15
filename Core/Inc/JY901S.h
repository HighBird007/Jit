#ifndef JY901S_H
#define JY901S_H

#include "stdint.h"
#include "usart.h"
#include "math.h"

//获取一项数据长度多11 length=11*x  x为需要的一些数据

#define JY901SDATALENGTH 11

#define JY901SUSART huart2

extern volatile uint8_t poseData[];

typedef struct{
	double roll;
	double pitch;
	double yaw;
	double heading;
}Pose;

extern volatile Pose curPose;

void initJY901S(void);

void updatePoseData(void);

#endif
