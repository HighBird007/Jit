#include "SteeringServo.h"
//1500 转向90 500舵机0   2500舵机180

HAL_StatusTypeDef mpuStatus;

HAL_StatusTypeDef steeringServoInit(void){
	
	mpuStatus = HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
	
	__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,1500);
	
	return mpuStatus;
}
//传入转弯的系数  p是负数左转  反之正数右转  左转舵机的pwm应处于500-1500越小幅度越大 右转 1500-2500越大幅度越大
void turnHeading(float p){
	//右转
	if(p>0){
		__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,1500+p*100.0);
	}else
	//左转
	if(p<0)
    {
		__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,1500+p*100.0);
	}
	
	if(p==0){
		__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,1500);
	}
}