#include <math.h>
#include "mpu6050.h"

#define RAD_TO_DEG 57.295779513082320876798154814105

#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1_REG 0x6B
#define SMPLRT_DIV_REG 0x19
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_CONFIG_REG 0x1B
#define GYRO_XOUT_H_REG 0x43

// Setup MPU6050
#define MPU6050_ADDR 0xD0

//mpu采取的i2c线路
#define mpuI2Cx &hi2c1

const uint16_t i2c_timeout = 1000;
const double Accel_Z_corrector = 14418.0;
//无关紧要的参数
uint32_t timer;
//此结构体为mpu6050特定需要的  包含着所有的数据 全局通用
MPU6050_t mpuStruct;

Kalman_t KalmanX = {
    .Q_angle = 0.01f,
    .Q_bias = 0.03f,
    .R_measure = 0.1f};

Kalman_t KalmanY = {
    .Q_angle = 0.01f,
    .Q_bias = 0.03f,
    .R_measure = 0.1f,
};


uint8_t mpuInit()
{
    uint8_t check;
    uint8_t Data;


	
    HAL_I2C_Mem_Read(mpuI2Cx, MPU6050_ADDR, WHO_AM_I_REG, 1, &check, 1, i2c_timeout);
	//返回id  如果不对则return 1 初始化失败
    if (check == 104) // 0x68 
    {
        
        Data = 0;
        HAL_I2C_Mem_Write(mpuI2Cx, MPU6050_ADDR, PWR_MGMT_1_REG, 1, &Data, 1, i2c_timeout);

        
        Data = 0x07;
        HAL_I2C_Mem_Write(mpuI2Cx, MPU6050_ADDR, SMPLRT_DIV_REG, 1, &Data, 1, i2c_timeout);

        // 设置加速度寄存器
        // XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> � 2g
        Data = 0x00;
        HAL_I2C_Mem_Write(mpuI2Cx, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, i2c_timeout);

        // 设置角速度寄存器
        // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> � 250 �/s
        Data = 0x00;
        HAL_I2C_Mem_Write(mpuI2Cx, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, i2c_timeout);
        return 0;
    }
    return 1;
}

void MPU6050_Read_Accel()
{
    uint8_t Rec_Data[6];

    // Read 6 BYTES of data starting from ACCEL_XOUT_H register

    HAL_I2C_Mem_Read(mpuI2Cx, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 6, i2c_timeout);

    mpuStruct.Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
    mpuStruct.Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
    mpuStruct.Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);

    /*** convert the RAW values into acceleration in 'g'
         we have to divide according to the Full scale value set in FS_SEL
         I have configured FS_SEL = 0. So I am dividing by 16384.0
         for more details check ACCEL_CONFIG Register              ****/

    mpuStruct.Ax = mpuStruct.Accel_X_RAW / 16384.0;
    mpuStruct.Ay = mpuStruct.Accel_Y_RAW / 16384.0;
    mpuStruct.Az = mpuStruct.Accel_Z_RAW / Accel_Z_corrector;
}

void MPU6050_Read_Gyro()
{
    uint8_t Rec_Data[6];

    // Read 6 BYTES of data starting from GYRO_XOUT_H register

    HAL_I2C_Mem_Read(mpuI2Cx, MPU6050_ADDR, GYRO_XOUT_H_REG, 1, Rec_Data, 6, i2c_timeout);

    mpuStruct.Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
    mpuStruct.Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
    mpuStruct.Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);

    /*** convert the RAW values into dps (�/s)
         we have to divide according to the Full scale value set in FS_SEL
         I have configured FS_SEL = 0. So I am dividing by 131.0
         for more details check GYRO_CONFIG Register              ****/

    mpuStruct.Gx = mpuStruct.Gyro_X_RAW / 131.0;
    mpuStruct.Gy = mpuStruct.Gyro_Y_RAW / 131.0;
    mpuStruct.Gz = mpuStruct.Gyro_Z_RAW / 131.0;
}

void MPU6050_Read_Temp()
{
    uint8_t Rec_Data[2];
    int16_t temp;

    // Read 2 BYTES of data starting from TEMP_OUT_H_REG register

    HAL_I2C_Mem_Read(mpuI2Cx, MPU6050_ADDR, TEMP_OUT_H_REG, 1, Rec_Data, 2, i2c_timeout);

    temp = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
    mpuStruct.Temperature = (float)((int16_t)temp / (float)340.0 + (float)36.53);
}
//函数读取所有的属性 加速度 角速度 温度 并且通过kalman计算出 翻滚 横滚角
void MPU6050_Read_All()
{
    uint8_t Rec_Data[14];
    int16_t temp;

    // Read 14 BYTES of data starting from ACCEL_XOUT_H register

    HAL_I2C_Mem_Read(mpuI2Cx, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 14, i2c_timeout);

    mpuStruct.Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
    mpuStruct.Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
    mpuStruct.Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);
    temp = (int16_t)(Rec_Data[6] << 8 | Rec_Data[7]);
    mpuStruct.Gyro_X_RAW = (int16_t)(Rec_Data[8] << 8 | Rec_Data[9]);
    mpuStruct.Gyro_Y_RAW = (int16_t)(Rec_Data[10] << 8 | Rec_Data[11]);
    mpuStruct.Gyro_Z_RAW = (int16_t)(Rec_Data[12] << 8 | Rec_Data[13]);

    mpuStruct.Ax = mpuStruct.Accel_X_RAW / 16384.0;
    mpuStruct.Ay = mpuStruct.Accel_Y_RAW / 16384.0;
    mpuStruct.Az = mpuStruct.Accel_Z_RAW / Accel_Z_corrector;
    mpuStruct.Temperature = (float)((int16_t)temp / (float)340.0 + (float)36.53);
    mpuStruct.Gx = mpuStruct.Gyro_X_RAW / 131.0;
    mpuStruct.Gy = mpuStruct.Gyro_Y_RAW / 131.0;
    mpuStruct.Gz = mpuStruct.Gyro_Z_RAW / 131.0;

    // Kalman angle solve
    double dt = (double)(HAL_GetTick() - timer) / 1000;
    timer = HAL_GetTick();
    double roll;
    double roll_sqrt = sqrt(
        mpuStruct.Accel_X_RAW * mpuStruct.Accel_X_RAW + mpuStruct.Accel_Z_RAW * mpuStruct.Accel_Z_RAW);
    if (roll_sqrt != 0.0)
    {
        roll = atan(mpuStruct.Accel_Y_RAW / roll_sqrt) * RAD_TO_DEG;
    }
    else
    {
        roll = 0.0;
    }
    double pitch = atan2(-mpuStruct.Accel_X_RAW, mpuStruct.Accel_Z_RAW) * RAD_TO_DEG;
    if ((pitch < -90 && mpuStruct.KalmanAngleY > 90) || (pitch > 90 && mpuStruct.KalmanAngleY < -90))
    {
        KalmanY.angle = pitch;
        mpuStruct.KalmanAngleY = pitch;
    }
    else
    {
        mpuStruct.KalmanAngleY = Kalman_getAngle(&KalmanY, pitch, mpuStruct.Gy, dt);
    }
    if (fabs(mpuStruct.KalmanAngleY) > 90)
        mpuStruct.Gx = -mpuStruct.Gx;
    mpuStruct.KalmanAngleX = Kalman_getAngle(&KalmanX, roll, mpuStruct.Gx, dt);
}

double Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt)
{
    double rate = newRate - Kalman->bias;
    Kalman->angle += dt * rate;

    Kalman->P[0][0] += dt * (dt * Kalman->P[1][1] - Kalman->P[0][1] - Kalman->P[1][0] + Kalman->Q_angle);
    Kalman->P[0][1] -= dt * Kalman->P[1][1];
    Kalman->P[1][0] -= dt * Kalman->P[1][1];
    Kalman->P[1][1] += Kalman->Q_bias * dt;

    double S = Kalman->P[0][0] + Kalman->R_measure;
    double K[2];
    K[0] = Kalman->P[0][0] / S;
    K[1] = Kalman->P[1][0] / S;

    double y = newAngle - Kalman->angle;
    Kalman->angle += K[0] * y;
    Kalman->bias += K[1] * y;

    double P00_temp = Kalman->P[0][0];
    double P01_temp = Kalman->P[0][1];

    Kalman->P[0][0] -= K[0] * P00_temp;
    Kalman->P[0][1] -= K[0] * P01_temp;
    Kalman->P[1][0] -= K[1] * P00_temp;
    Kalman->P[1][1] -= K[1] * P01_temp;

    return Kalman->angle;
};
