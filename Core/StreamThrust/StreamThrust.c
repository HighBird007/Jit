#include "StreamThrust.h"

//此类 原本打算控制喷水前进装置  目前暂时不需要 不删除作为保留
uint8_t streamThrustInit(void){
	
	HAL_StatusTypeDef stStatus = HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);
	
	return stStatus == HAL_OK ? true : false ;
}

void streamThrustStart(void){
	
     __HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,2000);
	
}

void streamThrustStop(void){
	
     __HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,0);
	
}