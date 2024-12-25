#include "JY901S.h"

uint8_t poseData[JY901SDATALENGTH];

Pose curPose;

void JY901SInit(void){
	
	curPose.roll = 0 ;
	
	curPose.heading = 0 ;
	
	curPose.pitch = 0 ;
	
	curPose.yaw = 0 ;
	
	HAL_UART_Receive_DMA(&JY901SUSART,poseData,JY901SDATALENGTH);
	
}


float getAngle(uint8_t high , uint8_t low){
	
	short all = (short)((short)high<<8|low);
	
	return all*1.0/32768.0*180;
	
}

//yaw范围是-180~180转换一下量程 变成 0 - 360  以y轴正方向为当前的朝向   
void yawToHeading(){
	
	if(curPose.yaw <= 0)curPose.heading = 	fabs(curPose.yaw);
	else if(curPose.yaw > 0)curPose.heading = 360 - curPose.yaw;
	
}

void updatePoseData(){
	
	curPose.roll = getAngle(poseData[3],poseData[2]);
	
	curPose.pitch = getAngle(poseData[5],poseData[4]);
	
	curPose.yaw = getAngle(poseData[7],poseData[6]);
	
	yawToHeading();
	
}
