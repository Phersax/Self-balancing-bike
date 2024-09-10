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
#define FS_GYRO_500         8
#define FS_GYRO_1000        9
#define FS_GYRO_2000        10

/* Accelerometer Full-Scale Range */
#define FS_ACC_2G         0
#define FS_ACC_4G         8
#define FS_ACC_8G         9
#define FS_ACC_16G        10

/* Default value of the WHO_AM_I register */
#define WHO_AM_I_VAL		0x68

/* IMU Address */
#define IMU_ADDR			0xD0

/* Accelerometer Scaling */
#define ACC_SCALE_2G			16384
#define ACC_SCALE_4G			8192
#define ACC_SCALE_8G			4096
#define ACC_SCALE_16G			2048

/* Gyroscope Scaling */
#define GYRO_SCALE_250         131
#define GYRO_SCALE_100         65.5
#define GYRO_SCALE_1000        32.8
#define GYRO_SCALE_2000        16.4

HAL_StatusTypeDef mpu6050_init();

void calculate_gyroscope_bias();
extern short gx_bias;
extern short gy_bias;
extern short gz_bias;

typedef struct {
    short data;
    HAL_StatusTypeDef status;
} result;

/* Gyroscope reading MPU6050 */
result mpu6050_accx();
result mpu6050_accy();
result mpu6050_accz();

/* Accelerometer reading MPU6050 */
result mpu6050_gyrox();
result mpu6050_gyroy();
result mpu6050_gyroz();

/* Temperature reading MPU6050 */
short mpu6050_temp();
/* Temperature in Celsius */
float mpu6050_temp_celsius();

float kalman_filter(float new_angle, float new_rate, float dt);

#endif /* INC_MPU6050_H_ */
