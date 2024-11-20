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

//hmc采用的iic编号*******************
#define hmcI2Cx &hi2c2

//5883计算出的为磁偏角 巡航计算的需要航向角 本地差异修改
#define localDeclination -6.22
//hmc的pid参数结构体
extern PIDController hmcPid;

extern float nextHeadingAngle;

HAL_StatusTypeDef hmcInit(void);

void hmcCalibration(void);

float hmcGetHeading(void);

#endif
