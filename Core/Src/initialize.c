#include "initialize.h"

void initPeripheral(void){
	

	//舵机初始化
	while(initSteeringServo() == false ){
		
		quickSend("#######steeringServo Init Failed \n");
		
		HAL_Delay(1000);
		
	}

	//初始化路线规划@@必要
	planMarkingInit();

	//启动gpsdma
	while(initGPS() == false ){
		
		quickSend("#######gps Init Failed \n");
		HAL_Delay(1000);
		
	}
	
	initLoop();

	//初始化遥控器模式
 	HAL_UART_Receive_DMA(&huart1,remoteCmd,2);
	

	//启动任务调度器定时器
	HAL_TIM_Base_Start_IT(&htim4);
	
	initJY901S();
	
		//初始化看门狗 看门狗线路40khz  分频器 32 计数器3800 1s 触发 计数1250 超过三秒则卡死重启 1s喂一次
	HAL_IWDG_Init(&hiwdg);
	
	
}
