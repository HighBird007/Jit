#ifndef GPS_h
#define GPS_h
#include "RingBuff.h"
#include <stdio.h>
#include "usart.h"
//此位gps的数据结构体 
typedef struct {    
    uint8_t lockType;         // GPS定位锁定类型
    uint8_t lockFlags;        // GPS锁定标志位
    uint8_t satelliteCount;   // 用于导航解算的卫星数量
   volatile double longitude;        // 经度（单位：degree）
   volatile double latitude;         // 纬度（单位：degree）
    int32_t velocityNorth;    // 北方向速度（单位：mm/s）
    int32_t velocityEast;     // 东方向速度（单位：mm/s）
    int32_t velocityDown;     // 向地速度（单位：mm/s）
    uint32_t speedAccuracy;   // 速度精度（单位：mm/s）
    uint32_t horizontalAccuracy; // 水平精度（单位：mm）
    uint16_t positionDOP;     // 定位精度因子（PDOP）：pdop<4，精度很好；4<pdop<7，精度可接受；pdop>7，精度较差
} GPSData;

//此位通用gps数据结构体 存放当前的位置信息
extern GPSData curGPSData;

//更新当前gps数据结构体curGPSData
uint8_t updateCurrentGPSData(void);

//dma暂存数组 最后写入环形缓冲区里
extern uint8_t gpsdata[];

#endif

