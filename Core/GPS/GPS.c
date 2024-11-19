#include "GPS.h"
//gps结构体
GPSData curGPSData;
//gps前几位固定
const uint8_t GPSHeader[] = {0xB5,0x62,0x01,0x07,0x5c,0x00};
uint8_t gpsdata[500];
// 从环形缓冲区中读取并解析GPS数据
uint8_t updateCurrentGPSData(void) {
    // 检查环形缓冲区中是否有足够的数据
    if (ringBuffGetDataCount() < 100) {
		quickSend("no data \n");
        return false; // 数据不足，直接返回
    }

    int8_t compareGPSHeaderFlag = 0; // 改为有符号整型
    uint8_t temp;

    for (int i = 0; i < 100; i++) {
        if (ringBuffReadByte(&temp) == true) {
            // 比对读取数据，相同则递增标志位
            if (temp == GPSHeader[compareGPSHeaderFlag]) {
                compareGPSHeaderFlag++;
                // 如果成功匹配整个GPSHeader
                if (compareGPSHeaderFlag == sizeof(GPSHeader)) {
                    compareGPSHeaderFlag = -1; // 设置为 -1 表示匹配成功
                    break; // 匹配成功，退出循环
                }
            } else {
                compareGPSHeaderFlag = 0; // 匹配失败，重置标志位
            }
        }
    }

    // 检查标志位是否表示匹配成功
    if (compareGPSHeaderFlag == -1) {
		//过度前面没用的数据
        ringBuffOffset(20);

    // 解析 lockType
    if (!ringBuffReadByte(&curGPSData.lockType)) {
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

    // 解析 latitude
    curGPSData.latitude = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t temp;
        if (!ringBuffReadByte(&temp)) {
            return false;
        }
        curGPSData.latitude += ((int32_t)temp << (8 * i));
    }

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
	
	char test[300];
sprintf(test, 
    "GPS Data:\n"
    "Lock Type: %d\n"
    "Lock Flags: %d\n"
    "Satellite Count: %d\n"
    "Longitude: %d degrees\n"
    "Latitude: %d degrees\n"
    "Velocity North: %d mm/s\n"
    "Velocity East: %d mm/s\n"
    "Velocity Down: %d mm/s\n"
    "Speed Accuracy: %d mm/s\n"
    "Horizontal Accuracy: %d mm\n"
    "PDOP: %d (%.2f)\n"
	"space %d"
	"write %d"
	"read %d",
    curGPSData.lockType,
    curGPSData.lockFlags,
    curGPSData.satelliteCount,
    curGPSData.longitude,
    curGPSData.latitude,
    curGPSData.velocityNorth,
    curGPSData.velocityEast,
    curGPSData.velocityDown,
    curGPSData.speedAccuracy,
    curGPSData.horizontalAccuracy,
    curGPSData.positionDOP,
    curGPSData.positionDOP / 100.0,ringBuffGetSpace(),ringBuff.writePoint,ringBuff.readPoint);

	quickSend(test);
    return true; // 解析成功
    }

    return false; // 匹配失败
}
