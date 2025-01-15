#include "Loop.h"
//结构体存放每个hz任务的标志位
Mission m;
//小船当前模式 自动巡航 AutoMode 和 遥控模式 RemoteMode
uint8_t shipMode = RemoteMode ;

uint8_t remoteCmd[10];

extern uint32_t curPlanMarkingFlag;

volatile double remoteHeading ;

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
char debug[200];
void missionHz1(void){
	
	//1s喂一次独立看门狗
	HAL_IWDG_Refresh(&hiwdg);
	
	sprintf(debug,"lat %f , lot %f \n",curGPSData.latitude,curGPSData.longitude);
	
	HAL_UART_Transmit(&huart1,(uint8_t*)debug,strlen(debug),1000);
	
	HAL_IWDG_Refresh(&hiwdg);
	
	sprintf(debug,"cF %d  di %f \n",curPlanMarkingFlag,calculateDistance());
	
	HAL_UART_Transmit(&huart1,(uint8_t*)debug,strlen(debug),1000);
	
	HAL_IWDG_Refresh(&hiwdg);
	
	sprintf(debug,"h %f , nh%f \n",curPose.heading,calculateBearing());
	
	HAL_UART_Transmit(&huart1,(uint8_t*)debug,strlen(debug),1000);
	
	if(shipMode == AutoMode)quickSend("cm auto \n");else quickSend("curmode remote \n");
	
	HAL_IWDG_Refresh(&hiwdg);
	
}

//hz5 任务
void missionHz5(void){
	
		switch(shipMode){
	
		case AutoMode:
			
		if( curGPSData.latitude ==0 || curGPSData.longitude == 0 ){
		
		quickSend("gps not init \n");
		
		return ;
		
		}
	
		//根据当前位置考虑是否更新下一个标记点
		updatePlanMarking();
		
        /*计算出当前位置 和目标位置的的朝向*/
		turnHeading(hmcPIDController_Update(&turnPid,calculateBearing(),curPose.heading));	
		
		break;
		
		case RemoteMode:
			
		//turnHeading(hmcPIDController_Update(&turnPid,,curPose.heading));	
			
		break;
	
	}
	
}

//hz10 
void missionHz10(void){
	
	switch(shipMode){
	
		case AutoMode:
			
		
		
		case RemoteMode:
			
		
			
		break;
	
	}
	
}

//hz50 
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

