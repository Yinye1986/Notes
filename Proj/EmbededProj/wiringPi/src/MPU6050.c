#include "MPU6050.h"

void MPU6050_iicInit(void) {
    MPU6050_FD = wiringPiI2CSetup(0x68);
}

void MPU6050_regInit(void) {
    wiringPiI2CWriteReg8(MPU6050_FD, MPU6050_PWR_MGMT_1, 0x01); // 取消睡眠模式，使用X轴陀螺仪作为时钟源
    wiringPiI2CWriteReg8(MPU6050_FD, MPU6050_PWR_MGMT_2, 0x00); // 所有轴使能，无待机
    wiringPiI2CWriteReg8(MPU6050_FD, MPU6050_SMPLRT_DIV, 0x09); // 采样率分频
    wiringPiI2CWriteReg8(MPU6050_FD, MPU6050_CONFIG, 0x06); // DLPF配置
    wiringPiI2CWriteReg8(MPU6050_FD, MPU6050_GYRO_CONFIG, 0x18); // 陀螺仪满量程范围±2000°/s
    wiringPiI2CWriteReg8(MPU6050_FD, MPU6050_ACCEL_CONFIG, 0x18); // 加速度计满量程范围±16g

}

uint8_t MPU6050_readReg(uint8_t RegAddress)
{
    return wiringPiI2CReadReg8(MPU6050_FD, RegAddress);
}

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    wiringPiI2CWriteReg8(MPU6050_FD, RegAddress, Data);
}

uint8_t MPU6050_getID(void)
{
    return MPU6050_readReg(0x75); // 0x75:设备识别寄存器,通常用于验证 MPU6050 的存在
}

/**
 * @brief 从MPU6050获取加速度和陀螺仪数据.
 * 
 * @param AccX 指向存储X轴加速度数据的指针.
 * @param AccY 指向存储Y轴加速度数据的指针.
 * @param AccZ 指向存储Z轴加速度数据的指针.
 * @param GyroX 指向存储X轴陀螺仪数据的指针.
 * @param GyroY 指向存储Y轴陀螺仪数据的指针.
 * @param GyroZ 指向存储Z轴陀螺仪数据的指针.
 */
void MPU6050_getData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
    uint8_t DataH, DataL;

    DataH = MPU6050_readReg(MPU6050_ACCEL_XOUT_H);
    DataL = MPU6050_readReg(MPU6050_ACCEL_XOUT_L);
    *AccX = (DataH << 8) | DataL;

    DataH = MPU6050_readReg(MPU6050_ACCEL_YOUT_H);
    DataL = MPU6050_readReg(MPU6050_ACCEL_YOUT_L);
    *AccY = (DataH << 8) | DataL;

    DataH = MPU6050_readReg(MPU6050_ACCEL_ZOUT_H);
    DataL = MPU6050_readReg(MPU6050_ACCEL_ZOUT_L);
    *AccZ = (DataH << 8) | DataL;

    DataH = MPU6050_readReg(MPU6050_GYRO_XOUT_H);
    DataL = MPU6050_readReg(MPU6050_GYRO_XOUT_L);
    *GyroX = (DataH << 8) | DataL;

    DataH = MPU6050_readReg(MPU6050_GYRO_YOUT_H);
    DataL = MPU6050_readReg(MPU6050_GYRO_YOUT_L);
    *GyroY = (DataH << 8) | DataL;

    DataH = MPU6050_readReg(MPU6050_GYRO_ZOUT_H);
    DataL = MPU6050_readReg(MPU6050_GYRO_ZOUT_L);
    *GyroZ = (DataH << 8) | DataL;
}
