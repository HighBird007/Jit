#ifndef Loop_h
#define Loop_h
#include "stdio.h"
#include "tim.h"
#include "GPS.h"
#include "usart.h"
#include "hmc5883.h"
#include "mpu6050.h"
#include "PID.h"
#include "SteeringServo.h"
#include "PlanPath.h"
#include "iwdg.h"
//自动巡航模式
#define AutoMode  0
//遥控模式
#define RemoteMode 1

extern uint8_t systemFlag;

typedef struct{
	uint8_t Hz1;
	uint8_t Hz5;
	uint8_t Hz10;
	uint8_t Hz15;
	uint8_t Hz50;
}Mission;

extern Mission m;

//初始化
void loopInit(void);

//开启事件循环
void loopExec(void);

//hz1 任务
void missionHz1(void);

//hz5 任务
void missionHz5(void);

//hz10 任务
void missionHz10(void);

//hz15 任务
void missionHz15(void);

//hz50 任务
void missionHz50(void);


#endif

