#ifndef HMC5883_h
#define HMC5883_h

#include "usart.h"
#include "i2c.h"
#include "stdio.h"
#include "math.h"
#include "PID.h"
#include "mpu6050.h"
#define HMC5883L_ADDRESS 0x3C

#define CONFIGURATION_A  0x00
#define CONFIGURATION_B  0x01
#define CONFIGURATION_C  0x02


extern PIDController hmcPid;

void hmcInit(void);

void hmcCalibration(void);

float hmcGetHeading(void);

#endif
