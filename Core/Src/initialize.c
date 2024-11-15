#include "initialize.h"
void initPeripheral(void){
	//舵机初始化
	steeringServoInit();
	//hmc初始化
	hmcInit(&hi2c2);
	//mpu初始化
	mpuInit(&hi2c1);
	quickSend("mpu init defeat \n ");

	
}
