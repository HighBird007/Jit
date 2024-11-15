#include "initialize.h"
void initPeripheral(void){
	//舵机初始化
	steeringServoInit();
	//mpu初始化

	while(	mpuInit(&hi2c1) == 1){
		quickSend("error");
		HAL_Delay(100);
	}
	//hmc初始化
	hmcInit();
	//hmc校准
	hmcCalibration();
}
