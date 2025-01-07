#include "GPS.h"
//gps结构体
GPSData curGPSData;
//gps前几位固定

uint8_t gpsdata[1000];

uint8_t initGPS(){

	
	HAL_StatusTypeDef gpsStatus = HAL_UARTEx_ReceiveToIdle_DMA(&huart3,gpsdata,1000);
	
	return gpsStatus == HAL_OK ? true : false ;

}
