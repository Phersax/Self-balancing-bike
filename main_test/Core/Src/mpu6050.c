#include <mpu6050.h>
#include <main.h>
#include <stdio.h>
#include <stdint.h>

extern I2C_HandleTypeDef hi2c1;

void mpu6050_init(){
	HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(&hi2c1, IMU_ADDR, 1, 100);
		if (status == HAL_OK) {
			printf("mpu6050 is ready \n");
			printf("mpu6050 configuration... \n");

			uint8_t data;

			/* power management register 0X6B must be all 0s to wake the sensor up */
			data = 0;
			HAL_I2C_Mem_Write(&hi2c1, IMU_ADDR, PWR_MGMT_1_REG, 1, &data, 1, 100);

			/* Set DATA RATE of 1KHz by writing SMPLRT_DIV register */
			data = 0x07;
			HAL_I2C_Mem_Write(&hi2c1, IMU_ADDR, SMPLRT_DIV_REG, 1, &data, 1, 100);

			/*  Set accelerometer configuration in ACCEL_CONFIG Register */
			/* XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 ->  2g */
			data = FS_ACC_2G;
			HAL_I2C_Mem_Write(&hi2c1, IMU_ADDR, ACCEL_CONFIG_REG, 1, &data, 1, 100);

			/* Set Gyroscope configuration in GYRO_CONFIG Register */
			/* XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 ->  250 deg/s */
			data = FS_GYRO_250;
			HAL_I2C_Mem_Write(&hi2c1, IMU_ADDR, GYRO_CONFIG_REG, 1, &data, 1, 100);
			//return HAL_OK;
		} else {
			printf("mpu6050 error, check connection cables \n");
			//return HAL_ERROR;
		}
}

/* Gyroscope reading MPU6050 */
short mpu6050_accx(){
	uint8_t buffer[14];
	HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR, ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, buffer, 14, 200);
	short imu_data = buffer[0] << 8  | buffer[1];
	return imu_data;
}

short mpu6050_accy(){
	uint8_t buffer[14];
		HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR, ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, buffer, 14, 200);
		short imu_data = buffer[2] << 8  | buffer[3];
		return imu_data;
}

short mpu6050_accz(){
	uint8_t buffer[14];
		HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR, ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, buffer, 14, 200);
		short imu_data = buffer[4] << 8  | buffer[5];
		return imu_data;
}

/* Accelerometer reading MPU6050 */
short mpu6050_gyrox(){
	uint8_t buffer[14];
	HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR, ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, buffer, 14, 200);
	short imu_data = buffer[6] << 8  | buffer[7];
	return imu_data;
}

short mpu6050_gyroy(){
	uint8_t buffer[14];
	HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR, ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, buffer, 14, 200);
	short imu_data = buffer[8] << 8  | buffer[9];
	return imu_data;
}

short mpu6050_gyroz(){
	uint8_t buffer[14];
	HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR, ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, buffer, 14, 200);
	short imu_data = buffer[10] << 8  | buffer[11];
	return imu_data;
}

/* Temperature reading MPU6050 */
short mpu6050_temp(){
	uint8_t buffer[14];
	HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR, ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, buffer, 14, 200);
	short imu_data = buffer[12] << 8  | buffer[13];
	return imu_data;
}
