#ifndef SteeringServo_h
#define SteeringServo_h
#include <tim.h>

//启动pwm输出
uint8_t steeringServoInit(void);

//p为pid算法输出的out
void turnHeading(float p);
#endif
