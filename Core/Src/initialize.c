#include "initialize.h"

void initPeripheral(void){
	
	//舵机初始化
	while(steeringServoInit() == false ){
		
		quickSend("#######steeringServo Init Failed \n");
		HAL_Delay(1000);
		
	}
	
	//mpu初始化
	while(	mpuInit() == false ){
		
		quickSend("#######mpu6050 init Failed \n");
		HAL_Delay(1000);
		
	}
	
	//hmc初始化
	while(	hmcInit() == false ){
		
		quickSend("#######hmc6050 init Failed \n");
		HAL_Delay(1000);
		
	}
	
	//hmc校准
	hmcCalibration();
	
	//初始化路线规划@@必要
	planMarkingInit();
	
	//初始化环形缓冲区
    ringBuffInit();
	
	//启动gpsdma
	HAL_UART_Receive_DMA(&huart3,gpsdata,100);
	
	loopInit();
	
	//初始化看门狗 看门狗线路40khz  分频器 32 计数器3800 1s 触发 计数1250 超过三秒则卡死重启 1s喂一次
	HAL_IWDG_Init(&hiwdg);
	
	//启动任务调度器定时器
	HAL_TIM_Base_Start_IT(&htim4);
	
}
