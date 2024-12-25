#include "Loop.h"
//结构体存放每个hz任务的标志位
Mission m;
//小船当前模式 自动巡航 AutoMode 和 遥控模式 RemoteMode
uint8_t shipMode = AutoMode ;

extern uint32_t curPlanMarkingFlag;

//全部赋值为0
void initLoop(){
	m.Hz1 = 0;
	m.Hz5 = 0;
	m.Hz10 = 0;
	m.Hz15 = 0;
	m.Hz50 = 0;
}


//loop 分为遥控模式 和 自动巡航模式
void loopExec(void){
	//hz1任务 发送gps数据
		if(m.Hz1){
	
		missionHz1();
		m.Hz1 = 0;
		
	}
	//hz10 任务 使用 舵机转向 此任务负责巡航
	if(m.Hz10){
	
		missionHz10();
		m.Hz10 = 0;
		
	}	
	//hz50 读取当前的mpu6050 hmc5883采集数据 
		if(m.Hz50){
			
		missionHz50();
		m.Hz50 = 0;
			
	}
	
	if(m.Hz5){
	
		missionHz5();
		m.Hz5 = 0;
		
	}
	
	if(m.Hz15){
	
		missionHz15();
		m.Hz15 = 0;
		
	}
	
}

//hz1任务 发送gps数据
void missionHz1(void){
	
	//1s喂一次独立看门狗
	HAL_IWDG_Refresh(&hiwdg);
	
}

//hz5 任务
void missionHz5(void){
	
	
	
}

//hz10 任务 使用 舵机转向 此任务负责巡航
void missionHz10(void){
	
	switch(shipMode){
	
		case AutoMode:
			
		
		//如果环形缓冲区数据不够return 或者gps还在启动中  则中断
		if( updateCurrentGPSData() == false )return ;
	
		
		//根据当前位置考虑是否更新下一个标记点
		updatePlanMarking();
		
        /*计算出当前位置 和目标位置的的朝向*/
		turnHeading(hmcPIDController_Update(&turnPid,calculateBearing(),curPose.yaw));	
		
		streamThrustStart();
		
		break;
		
		case RemoteMode:
			
		break;
	
	}
	
}

//hz50 读取当前的mpu6050 hmc5883采集数据 
void missionHz50(void){

	switch(shipMode){
	
		case AutoMode:
			
		break;
		
		case RemoteMode:
			
		break;
	
	}
	
}
//为用到
void missionHz15(void){



}

