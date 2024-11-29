#ifndef HMC5883_h
#define HMC5883_h

#include "usart.h"
#include "i2c.h"
#include "stdio.h"
#include "math.h"
#include "PID.h"
#include "mpu6050.h"
#include "iwdg.h"
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

//planpath使用这个
extern float nextHeadingAngle;

extern float curHeadingAngle;

uint8_t hmcInit(void);
//校准
void hmcCalibration(void);
//获取当前的头朝向 
float hmcGetHeading(void);

#endif
