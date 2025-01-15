#include "GPS.h"
//gps结构体
volatile GPSData curGPSData;
//g设备1hz

uint8_t gpsdata[1000];

uint8_t initGPS(){

	
	HAL_StatusTypeDef gpsStatus = HAL_UARTEx_ReceiveToIdle_DMA(&huart3,gpsdata,1000);
	
	return gpsStatus == HAL_OK ? true : false ;

}
