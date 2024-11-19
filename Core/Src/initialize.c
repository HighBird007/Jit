#include "initialize.h"
void initPeripheral(void){
	//舵机初始化
	steeringServoInit();
	//mpu初始化

	while(	mpuInit() == 1){
		quickSend("mpu init error");
		HAL_Delay(1000);
	}
	//hmc初始化
	hmcInit();
	//hmc校准
	hmcCalibration();
}
