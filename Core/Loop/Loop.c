#include "Loop.h"

//结构体存放每个hz任务的标志位
Mission m;
//小船当前模式 自动巡航 AutoMode 和 遥控模式 RemoteMode
uint8_t shipMode = AutoMode ;

//全部赋值为0
void loopInit(){
	m.Hz1 = 0;
	m.Hz5 = 0;
	m.Hz10 = 0;
	m.Hz50 = 0;
}
//loop 分为遥控模式 和 自动巡航模式

void loopExec(void){
	
		if(m.Hz1){
	
		missionHz1();
		m.Hz1 = 0;
		
	}
	
	if(m.Hz10){
	
		missionHz10();
		m.Hz10 = 0;
		
	}	
	
		if(m.Hz50){
			
		//missionHz50();
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
extern uint32_t curPlanMarkingFlag;
//hz1任务 发送gps数据
void missionHz1(void){
	
	//1s喂一次独立看门狗
	HAL_IWDG_Refresh(&hiwdg);
	
	
	quickSendDouble("latitude-------",curGPSData.latitude);
	quickSendDouble("longitude-------",curGPSData.longitude);
	quickSendDouble("nextLatitudde-------",nextPlanMarking.Latitude);
	quickSendDouble("nextLongitude-------",nextPlanMarking.Longitude);
	quickSendDouble("curhmc------",curHeadingAngle);
	quickSendDouble("nexthmc----------",nextHeadingAngle);
	quickSendNum("flag-----------",curPlanMarkingFlag);
	quickSend("<----------------------------------------------->\n");
	
}

//hz5 任务
void missionHz5(void){
	
	
	
}

//hz10 任务 使用 舵机转向 此任务负责巡航
void missionHz10(void){
	
	switch(shipMode){
	
		case AutoMode:

		//如果环形缓冲区数据不够return 或者gps还在启动中  则中断
		//if( updateCurrentGPSData() == false )return ;
		
		//根据当前位置考虑是否更新下一个标记点
		updatePlanMarking();
		
        /*计算出当前位置 和目标位置的的朝向*/
        nextHeadingAngle = calculateBearing();
		
		//传入hmcPid控制参数 并且使能舵机
	    turnHeading(hmcPIDController_Update(&hmcPid,nextHeadingAngle,curHeadingAngle));
		
		
		break;
		
		case RemoteMode:
			
		break;
	
	}
	
}

//hz50 任务 获取当前的姿态 以及当前的朝向  由于设备只能15hz 修改成
void missionHz50(void){


	
}

void missionHz15(void){
	
		switch(shipMode){
	
		case AutoMode:
			
		MPU6050_Read_All();
		
		curHeadingAngle = hmcGetHeading();
		
		break;
		
		case RemoteMode:
			
		break;
	
	}
}