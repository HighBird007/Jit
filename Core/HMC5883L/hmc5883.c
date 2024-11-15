#include "hmc5883.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
int16_t Xoffest = 0, Yoffest = 0;  // 使用int16_t类型处理有符号数据
int16_t GaX, GaY;  // 修改为有符号类型
uint8_t hmcData[6];

HAL_StatusTypeDef hmcStatus;

PIDController hmcPid= { 0.5f, 0.1f, 0.01f,
                         0.1f,
                          PID_LIM_MIN, PID_LIM_MAX,
			              PID_LIM_MIN_INT, PID_LIM_MAX_INT,
                          0.1f };
//初始化
void hmcInit(I2C_HandleTypeDef *I2Cx){
    uint8_t data = 0x70;
    
    hmcStatus = HAL_I2C_Mem_Write(I2Cx, HMC5883L_ADDRESS, CONFIGURATION_A, 1, &data, 1, 1000); 
    if(hmcStatus != HAL_OK){
        quickSend("CONFIGURATION_A error");
    }
    
    data = 0x20;
    hmcStatus = HAL_I2C_Mem_Write(I2Cx, HMC5883L_ADDRESS, CONFIGURATION_B, 1, &data, 1, 1000); 
    if(hmcStatus != HAL_OK){
        quickSend("CONFIGURATION_B error");
    }

    data = 0x0;    
    hmcStatus = HAL_I2C_Mem_Write(I2Cx, HMC5883L_ADDRESS, CONFIGURATION_C, 1, &data, 1, 1000);
    if(hmcStatus != HAL_OK){
        quickSend("CONFIGURATION_C error");
    }
	   //实验室校准好的数值  下面校准函数无需调用
	    Xoffest = (376 -300) / 2;
        Yoffest = (-70-759) / 2;
	    PIDController_Init(&hmcPid);
}
//获取当前的头朝方向
float hmcGetHeading(I2C_HandleTypeDef *I2Cx) {
	
    hmcStatus = HAL_I2C_Mem_Read(I2Cx, HMC5883L_ADDRESS, 0x03, 1, hmcData, 6, 1000); //连续读取
    
    if(hmcStatus != HAL_OK) {
        quickSend("read failed\n");
        return -1;
    }

    // 合并并校准数据
    GaX = (((int16_t)(hmcData[0]) << 8) | hmcData[1]) - Xoffest;
    GaY = (((int16_t)(hmcData[4]) << 8) | hmcData[5]) - Yoffest;
	
     float Magangle = atan2(GaY, GaX) * 180.0 / M_PI;
	
     if (Magangle < 0) {
     Magangle += 360.0;
	 }
	 
	 char test[100];
	 sprintf(test, "x %d y %d   ma %f \n", GaX, GaY,Magangle);
     quickSend(test);
	 return Magangle;
}

//校准
void hmcCalibration(I2C_HandleTypeDef *I2Cx){
	
    int count = 100;
    int xmax = -32768, xmin = 32767, ymax = -32768, ymin = 32767, zmax = -32768, zmin = 32767;
    char messhhee[100];
    while (count != 0) {
        HAL_I2C_Mem_Read(I2Cx, HMC5883L_ADDRESS, 0x03, I2C_MEMADD_SIZE_8BIT, hmcData, 6, 1000);
        int16_t x = (int16_t)(hmcData[0] << 8 | hmcData[1]);
        int16_t y = (int16_t)(hmcData[4] << 8 | hmcData[5]);
        if (x > xmax) xmax = x;	
        if (x < xmin) xmin = x;
        if (y > ymax) ymax = y;
        if (y < ymin) ymin = y;
    sprintf(messhhee, "init %d,%d  %d\n",x, y,count);
    HAL_UART_Transmit(&huart1, (uint8_t*)messhhee, strlen(messhhee), 1000);
        count--;
        HAL_Delay(100);
    }

    Xoffest = (xmax + xmin) / 2;
    Yoffest = (ymax + ymin) / 2;
	
		sprintf(messhhee, "init %d,%d  ||%d,%d \n",xmax,xmin,ymax,ymin);
      HAL_UART_Transmit(&huart1, (uint8_t*)messhhee, strlen(messhhee), 1000);
}
