#ifndef _I2C_H
#define _I2C_H
#include "stm32f10x.h"                  // Device header

#define MPU6050_CONFIG			0x1A
#define MPU6050_GYRO_CONFIG		0x1B
#define	MPU6050_ACCEL_CONFIG	0x1C
#define	MPU6050_SMPLRT_DIV		0x19
#define MPU6050_PWR_MGMT_1		0x6B

#define MPU6050_WHO_AM_I		0x75
#define	MPU_ADDRESS				0xD0

#define	MPU_ACCEL_XOUT_H		0x3B
#define	MPU_ACCEL_XOUT_L		0x3C
#define	MPU_ACCEL_YOUT_H		0x3D
#define	MPU_ACCEL_YOUT_L		0x3E
#define	MPU_ACCEL_ZOUT_H		0x3F
#define	MPU_ACCEL_ZOUT_L		0x40

#define	MPU_GYRO_XOUT_H			0x43
#define	MPU_GYRO_XOUT_L			0x44
#define	MPU_GYRO_YOUT_H			0x45
#define	MPU_GYRO_YOUT_L			0x46
#define	MPU_GYRO_ZOUT_H			0x47
#define	MPU_GYRO_ZOUT_L			0x48

#define MPU_TEMP_OUT_H			0x41
#define MPU_TEMP_OUT_L			0x42

void I2C1_Config(void);
void I2C2_Config(void);

uint8_t I2C_WriteByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t data);

uint8_t I2C_ReadByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t* buffer);

uint8_t I2C_ReadMultiByteFromSlave(uint8_t device,uint8_t reg,uint8_t length,uint8_t* data);

uint8_t I2C_WriteMultiByteToSlave(uint8_t device,uint8_t reg,uint8_t length,uint8_t* data);

int16_t MPU6050_GetData(uint8_t regAddr);
void MPU6050_Configuration(void);
#endif