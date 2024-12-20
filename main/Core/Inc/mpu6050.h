#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include <stdint.h>

/* MPU6050 registers' addresses */
#define WHO_AM_I_REG 		0x75
#define PWR_MGMT_1_REG 		0x6B
#define SMPLRT_DIV_REG 		0x19
#define ACCEL_CONFIG_REG 	0x1C
#define ACCEL_XOUT_H_REG 	0x3B
#define TEMP_OUT_H_REG 		0x41
#define GYRO_CONFIG_REG 	0x1B
#define GYRO_XOUT_H_REG 	0x43

/* Gyroscope Full-Scale Range */
#define FS_GYRO_250         0
#define FS_GYRO_500         0x08
#define FS_GYRO_1000        9
#define FS_GYRO_2000        10

/* Accelerometer Full-Scale Range */
#define FS_ACC_2G         0
#define FS_ACC_4G         8
#define FS_ACC_8G         0x10
#define FS_ACC_16G        24

/* Default value of the WHO_AM_I register */
#define WHO_AM_I_VAL		0x68

/* IMU Address */
#define IMU_ADDR			0xD0

/* Accelerometer Scaling */
#define ACC_SCALE_2G			16384.0
#define ACC_SCALE_4G			8192
#define ACC_SCALE_8G			4096
#define ACC_SCALE_16G			2048
#define ACC_SCALE               ACC_SCALE_8G

/* Gyroscope Scaling */
#define GYRO_SCALE_250         131
#define GYRO_SCALE_500         65.5
#define GYRO_SCALE_1000        32.8
#define GYRO_SCALE_2000        16.4
#define GYRO_SCALE             GYRO_SCALE_500

extern float ax_bias;
extern float ay_bias;
extern float az_bias;
extern int16_t gx_bias;
extern int16_t gy_bias;
extern int16_t gz_bias;

typedef struct {
	float data;
	HAL_StatusTypeDef status;
} result;

typedef struct {
	float ax;
	float ay;
	float az;
	float gx;
	float gy;
	float gz;
} mpu_data;

/*mpu6050 initialization*/
HAL_StatusTypeDef mpu6050_init();

/*Gyroscope bias calculate*/
void calculate_gyroscope_bias();

/*Accelerometer bias calculate*/
void calculate_accelerometer_bias();

/* Accelerometer reading MPU6050 */
result mpu6050_accx();
result mpu6050_accy();
result mpu6050_accz();

/* Gyroscope reading MPU6050 */
result mpu6050_gyrox();
result mpu6050_gyroy();
result mpu6050_gyroz();

/* All data reading MPU6050 */
mpu_data mpu6050_data();

/* Temperature reading MPU6050 */
short mpu6050_temp();

#endif /* INC_MPU6050_H_ */
