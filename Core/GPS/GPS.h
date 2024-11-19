#ifndef GPS_h
#define GPS_h
#include "RingBuff.h"
#include <stdio.h>
typedef struct {    
    uint8_t lockType;         // GPS定位锁定类型
    uint8_t lockFlags;        // GPS锁定标志位
    uint8_t satelliteCount;   // 用于导航解算的卫星数量
    int32_t longitude;        // 经度（单位：degree）
    int32_t latitude;         // 纬度（单位：degree）
    int32_t velocityNorth;    // 北方向速度（单位：mm/s）
    int32_t velocityEast;     // 东方向速度（单位：mm/s）
    int32_t velocityDown;     // 向地速度（单位：mm/s）
    uint32_t speedAccuracy;   // 速度精度（单位：mm/s）
    uint32_t horizontalAccuracy; // 水平精度（单位：mm）
    uint16_t positionDOP;     // 定位精度因子（PDOP）：pdop<4，精度很好；4<pdop<7，精度可接受；pdop>7，精度较差
} GPSData;

uint8_t updateCurrentGPSData(void);

extern uint8_t gpsdata[];
#endif

