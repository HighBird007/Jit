#include "SteeringServo.h"
//1500 转向90 500舵机0   2500舵机180

uint8_t initSteeringServo(void){
	
    HAL_StatusTypeDef mpuStatus = HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
	
	__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,1500);
	
	return mpuStatus == HAL_OK ? true : false ;
	
}

//传入转弯的系数  p是负数左转  反之正数右转  左转舵机的pwm应处于500-1500越小幅度越大 右转 1500-2500越大幅度越大
//这里的1MS对应着1000 例如 0.5MS高电平意味着 0度 那么应该设置比较寄存器 500  隔壁的水流驱动器也是如此
void turnHeading(float p){

		__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,1500+p*90.0);
	
}
