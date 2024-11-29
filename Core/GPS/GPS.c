#include "GPS.h"
//gps结构体
GPSData curGPSData;
//gps前几位固定
const uint8_t GPSHeader[] = {0xB5,0x62,0x01,0x07,0x5c,0x00};
uint8_t gpsdata[500];
// 从环形缓冲区中读取并解析GPS数据 存放于  -> curGPSData <- gps结构体
uint8_t updateCurrentGPSData(void) {
    // 检查环形缓冲区中是否有足够的数据
   if (ringBuffGetDataCount() < 100)
	   {
		//quickSend("gps cant read  ringBuff no data \n");
        return false; // 数据不足，直接返回
    }

    int8_t compareGPSHeaderFlag = 0; 
	
    uint8_t temp;
	
    for (int i = 0; i < 100; i++) {
		
        if (ringBuffReadByte(&temp) == true) {
			
            // 比对读取数据，相同则递增标志位
            if (temp == GPSHeader[compareGPSHeaderFlag++]) {
                // 如果成功匹配整个GPSHeader
                if (compareGPSHeaderFlag == sizeof(GPSHeader)) {
					compareGPSHeaderFlag = -1;
                    break; // 匹配成功，退出循环
                }
            } else break; 
			
        }
    }
	//quickSendNum("header length 111",compareGPSHeaderFlag);
    // 检查标志位是否表示匹配成功
    if (compareGPSHeaderFlag == -1) {
		
		quickSend("start gps working\n");
		
		//过度前面没用的数据
        ringBuffOffset(20);

    // 解析 lockType
    if (!ringBuffReadByte(&curGPSData.lockType)) {
		//ringBuffOffset(73);
        return false;
    }

    // 解析 lockFlags
    if (!ringBuffReadByte(&curGPSData.lockFlags)) {
        return false;
    }

    // 跳过 1 字节（无用数据）
    ringBuffOffset(1);

    // 解析 satelliteCount
    if (!ringBuffReadByte(&curGPSData.satelliteCount)) {
        return false;
    }

    // 解析 longitude
    curGPSData.longitude = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t temp;
        if (!ringBuffReadByte(&temp)) {
            return false;
        }
        curGPSData.longitude += ((int32_t)temp << (8 * i));
    }
	quickSendNum("yuanshiLongtitude " , curGPSData.longitude);
	curGPSData.longitude/=10000000.0f;
	
    // 解析 latitude
    curGPSData.latitude = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t temp;
        if (!ringBuffReadByte(&temp)) {
            return false;
        }
        curGPSData.latitude += ((int32_t)temp << (8 * i));
    }
	quickSendNum("yuanshiLatitude " , curGPSData.latitude);
	curGPSData.latitude/=10000000.0f;
	
    // 跳过 8 字节（无用数据）
    ringBuffOffset(8);

    // 解析 horizontalAccuracy
    curGPSData.horizontalAccuracy = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t temp;
        if (!ringBuffReadByte(&temp)) {
            return false;
        }
        curGPSData.horizontalAccuracy += ((uint32_t)temp << (8 * i));
    }

    // 跳过 4 字节（无用数据）
    ringBuffOffset(4);

    // 解析 velocityEast
    curGPSData.velocityEast = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t temp;
        if (!ringBuffReadByte(&temp)) {
            return false;
        }
        curGPSData.velocityEast += ((int32_t)temp << (8 * i));
    }

    // 解析 velocityNorth
    curGPSData.velocityNorth = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t temp;
        if (!ringBuffReadByte(&temp)) {
            return false;
        }
        curGPSData.velocityNorth += ((int32_t)temp << (8 * i));
    }

    // 解析 velocityDown
    curGPSData.velocityDown = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t temp;
        if (!ringBuffReadByte(&temp)) {
            return false;
        }
        curGPSData.velocityDown += ((int32_t)temp << (8 * i));
    }

    // 跳过 8 字节（无用数据）
    ringBuffOffset(8);

    // 解析 speedAccuracy
    curGPSData.speedAccuracy = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t temp;
        if (!ringBuffReadByte(&temp)) {
            return false;
        }
        curGPSData.speedAccuracy += ((uint32_t)temp << (8 * i));
    }

    // 跳过 4 字节（无用数据）
    ringBuffOffset(4);

    // 解析 positionDOP
    curGPSData.positionDOP = 0;
    for (int i = 0; i < 2; i++) {
        uint8_t temp;
        if (!ringBuffReadByte(&temp)) {
            return false;
        }
        curGPSData.positionDOP += ((uint16_t)temp << (8 * i));
    }

    // 缩小 PDOP 比例
    curGPSData.positionDOP /= 100;
	
	//去除末尾的多余数据
	ringBuffOffset(16);
	
    return true; // 解析成功
    }

    return false; // 匹配失败
}
