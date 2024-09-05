#include <stdio.h>
#include <stdint.h>

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

/* MPU6050 registers' addresses */
#define MPU6050_WHO_AM_I_REG     0x75
#define MPU6050_PWR_MGMT_1_REG   0x6B
#define MPU6050_SMPLRT_DIV_REG   0x19
#define MPU6050_ACCEL_CONFIG_REG 0x1C
#define MPU6050_ACCEL_XOUT_H_REG 0x3B
#define MPU6050_TEMP_OUT_H_REG   0x41
#define MPU6050_GYRO_CONFIG_REG  0x1B
#define MPU6050_GYRO_XOUT_H_REG  0x43

/* Gyroscope Full-Scale Range */
#define MPU6050_FS_GYRO_250      0
#define MPU6050_FS_GYRO_500      8
#define MPU6050_FS_GYRO_1000     10
#define MPU6050_FS_GYRO_2000     9

/* Accelerometer Full-Scale Range */
#define MPU6050_FS_ACC_2G        0
#define MPU6050_FS_ACC_4G        8
#define MPU6050_FS_ACC_8G        10
#define MPU6050_FS_ACC_16G       9

/* Default value of the WHO_AM_I register */
#define MPU6050_WHO_AM_I_VAL     0x68

/* IMU Address */
#define MPU6050_IMU_ADDR         0xD0

/* Function prototypes */
void mpu6050_init();

void calculate_gyroscope_bias();

extern short gx_bias;
extern short gy_bias;
extern short gz_bias;

/* Accelerometer reading MPU6050 */
int16_t mpu6050_accx();
int16_t mpu6050_accy();
int16_t mpu6050_accz();
int16_t mpu6050_raw_accx();
int16_t mpu6050_raw_accy();
int16_t mpu6050_raw_accz();

/* Gyroscope reading MPU6050 */
int16_t mpu6050_gyrox();
int16_t mpu6050_gyroy();
int16_t mpu6050_gyroz();
int16_t mpu6050_raw_gyrox();
int16_t mpu6050_raw_gyroy();
int16_t mpu6050_raw_gyroz();

/* Temperature reading MPU6050 */
short mpu6050_raw_temp();

/* Temperature in Celsius */
float mpu6050_temp_celsius();

float kalman_filter(float new_angle, float new_rate, float dt);

#endif /* INC_MPU6050_H_ */
