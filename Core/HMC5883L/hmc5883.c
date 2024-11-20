#include "hmc5883.h"

int16_t Xoffest = 0, Yoffest = 0, Zoffest;  // 使用int16_t类型处理有符号数据

int16_t GaX, GaY,GaZ;  // 修改为有符号类型

//存放hmc的寄存器读取数据
uint8_t hmcData[6];

//初始化 受PlanPath控制
float nextHeadingAngle;

HAL_StatusTypeDef hmcStatus;

//hmc模块的pid结构体部分参数模块化
PIDController hmcPid= { 0.5f, 0.1f, 0.01f,
                         0.1f,
                          PID_LIM_MIN, PID_LIM_MAX,
			              PID_LIM_MIN_INT, PID_LIM_MAX_INT,
                          0.1f };
						  
//初始化
HAL_StatusTypeDef hmcInit(){
    uint8_t data = 0x70;
    
    hmcStatus = HAL_I2C_Mem_Write(hmcI2Cx, HMC5883L_ADDRESS, CONFIGURATION_A, 1, &data, 1, 1000); 
	
    if(hmcStatus != HAL_OK){
        quickSend("CONFIGURATION_A error");
		return hmcStatus;
    }
    
    data = 0x20;
    hmcStatus = HAL_I2C_Mem_Write(hmcI2Cx, HMC5883L_ADDRESS, CONFIGURATION_B, 1, &data, 1, 1000); 
    if(hmcStatus != HAL_OK){
        quickSend("CONFIGURATION_B error");
		return hmcStatus;
    }

    data = 0x0;    
    hmcStatus = HAL_I2C_Mem_Write(hmcI2Cx, HMC5883L_ADDRESS, CONFIGURATION_C, 1, &data, 1, 1000);
    if(hmcStatus != HAL_OK){
        quickSend("CONFIGURATION_C error");
		return hmcStatus;
    }
	   //实验室校准好的数值  下面校准函数无需调用
	  //  Xoffest = (376 -300) / 2;
      //  Yoffest = (-70-759) / 2;
	
	//初始化hmc模块的pid结构体参数
	    PIDController_Init(&hmcPid);
	
	return hmcStatus;
}

//获取当前的头朝方向
float hmcGetHeading() {
	
    hmcStatus = HAL_I2C_Mem_Read(hmcI2Cx, HMC5883L_ADDRESS, 0x03, 1, hmcData, 6, 1000); //连续读取
    
    if(hmcStatus != HAL_OK) {
        quickSend("read failed\n");
        return -1;
    }

    // 合并并校准数据
    GaX = (((int16_t)(hmcData[0]) << 8) | hmcData[1]) - Xoffest;
    GaY = (((int16_t)(hmcData[4]) << 8) | hmcData[5]) - Yoffest;
	GaZ = (((int16_t)(hmcData[2]) << 8) | hmcData[3]) - Zoffest;
	//平面算法
    float Magangle = atan2(GaY, GaX) * 180.0 / M_PI;
	
	MPU6050_Read_All();
//	
//	float P = mpu.KalmanAngleX;
//	float R = mpu.KalmanAngleY;
//	float XH = GaX*cos(P)+GaY*sin(R)*sin(P)-GaZ*cos(R)*sin(P);
//    float YH = GaY*cos(R)+GaZ*sin(R);
//		
//	float Magangle = atan2(YH,XH) * 180.0 / M_PI;
	
	Magangle += localDeclination;
	
    if (Magangle >= 360.0) Magangle -= 360.0; // 确保在0-360范围内
    if (Magangle < 0.0) Magangle += 360.0;
	 
	char test[100];
	sprintf(test, "x %d y %d z %d  ma %f  p %f  r %f \n", GaX, GaY,GaZ,Magangle,mpuStruct.KalmanAngleX,mpuStruct.KalmanAngleY);
    quickSend(test);
	 
	return Magangle;
}

//校准
void hmcCalibration(){
	
    int count = 100;
    int xmax = -32768, xmin = 32767, ymax = -32768, ymin = 32767, zmax = -32768, zmin = 32767;
	
    char messhhee[100];
	
    while (count != 0) {
		
        HAL_I2C_Mem_Read(hmcI2Cx, HMC5883L_ADDRESS, 0x03, I2C_MEMADD_SIZE_8BIT, hmcData, 6, 1000);
        int16_t x = (int16_t)(hmcData[0] << 8 | hmcData[1]);
        int16_t y = (int16_t)(hmcData[4] << 8 | hmcData[5]);
		int16_t z = (int16_t)(hmcData[2] << 8 | hmcData[3]);
        if (x > xmax) xmax = x;	
        if (x < xmin) xmin = x;
        if (y > ymax) ymax = y;
        if (y < ymin) ymin = y;
		if (z > zmax) zmax = z;
		if (z < zmin) zmin = z;
		
		sprintf(messhhee, "init %d,%d,%d  %d\n",x, y,z,count);
		quickSend(messhhee);
		
        count--;
        HAL_Delay(100);
    }
		
		Xoffest = (xmax + xmin) / 2;
		Yoffest = (ymax + ymin) / 2;
		Zoffest = (zmax + zmin) / 2;
	
	  sprintf(messhhee, "init %d,%d  ||%d,%d ||%d,%d  ||offest %d %d %d\n",xmax,xmin,ymax,ymin,zmax,zmin,Xoffest,Yoffest,Zoffest);
      quickSend(messhhee);
}
