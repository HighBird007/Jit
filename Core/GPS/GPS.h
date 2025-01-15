#ifndef GPS_h
#define GPS_h

#include <stdio.h>
#include "usart.h"
#include "NMEA0183.h"

//此位gps的数据结构体 
typedef struct {    

    double longitude;        // 经度（单位：degree）
    double latitude;         // 纬度（单位：degree）

} GPSData;

//此位通用gps数据结构体 存放当前的位置信息
extern volatile GPSData curGPSData;

uint8_t initGPS(void);

//dma暂存数组 最后写入环形缓冲区里
extern uint8_t gpsdata[];

#endif

