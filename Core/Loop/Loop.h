#ifndef Loop_h
#define Loop_h

#include "stdio.h"
#include "tim.h"
#include "GPS.h"
#include "usart.h"
#include "PID.h"
#include "SteeringServo.h"
#include "PlanPath.h"
#include "iwdg.h"
#include "StreamThrust.h"
#include "JY901S.h"
//自动巡航模式
#define AutoMode  0
//遥控模式
#define RemoteMode 1

extern uint8_t systemFlag;

extern uint8_t remoteCmd[];

typedef struct{
	uint8_t Hz1;
	uint8_t Hz5;
	uint8_t Hz10;
	uint8_t Hz15;
	uint8_t Hz50;
}Mission;

extern Mission m;

extern uint8_t shipMode;

//初始化
void initLoop(void);

//开启事件循环
void loopExec(void);



#endif

