#ifndef initialize_h
#define initialize_h
#include "hmc5883.h"
#include "mpu6050.h"
#include "SteeringServo.h"
#include "usart.h"
#include "PID.h"
#include "RingBuff.h"
#include "GPS.h"
#include "Loop.h"
#include "iwdg.h"
void initPeripheral(void);

#endif
