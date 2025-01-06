#include "StreamThrust.h"

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