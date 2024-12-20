#include <mpu6050.h>
#include <main.h>
#include <stdio.h>
#include <stdint.h>

extern I2C_HandleTypeDef hi2c1;

float ax_bias = 0;
float ay_bias = 0;
float az_bias = 0;
int16_t gx_bias = 0;
int16_t gy_bias = 0;
int16_t gz_bias = 0;

/*mpu6050 initialization*/
HAL_StatusTypeDef mpu6050_init() {

	uint8_t whoami;
	uint8_t data;

	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR, WHO_AM_I_REG,
	I2C_MEMADD_SIZE_8BIT, &whoami, 1, 100);

	if (status == HAL_OK) {
		if (whoami == WHO_AM_I_VAL) {
			HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(&hi2c1, IMU_ADDR,
					1, 100);
			if (status == HAL_OK) {

				/* power management register 0X6B must be all 0s to wake the sensor up */
				data = 0;
				HAL_I2C_Mem_Write(&hi2c1, IMU_ADDR, PWR_MGMT_1_REG, 1, &data, 1,
						100);

				/* Set DATA RATE of 1KHz by writing SMPLRT_DIV register */
				data = 0x07;
				HAL_I2C_Mem_Write(&hi2c1, IMU_ADDR, SMPLRT_DIV_REG, 1, &data, 1,
						100);

				/*  Set accelerometer configuration in ACCEL_CONFIG Register */
				/* XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 ->  2g */
				data = 0x10; //
				HAL_I2C_Mem_Write(&hi2c1, IMU_ADDR, ACCEL_CONFIG_REG, 1, &data,
						1, 100);

				/* Set Gyroscope configuration in GYRO_CONFIG Register */
				/* XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 ->  250 deg/s */
				data = 0x08; //
				HAL_I2C_Mem_Write(&hi2c1, IMU_ADDR, GYRO_CONFIG_REG, 1, &data,
						1, 100);

				calculate_gyroscope_bias();
				//calculate_accelerometer_bias();
			}
		}
	}
	return status;
}
/*Gyroscope bias calculate*/
void calculate_gyroscope_bias() {
	// Number of samples to average
	int num_samples = 2000;
	float gx_sum = 0;
	float gy_sum = 0;
	float gz_sum = 0;

	for (int i = 0; i < num_samples; i++) {
		uint8_t buffer[14];
		HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR, ACCEL_XOUT_H_REG,
		I2C_MEMADD_SIZE_8BIT, buffer, 14, 100);
		gx_sum += (int16_t) (buffer[8] << 8 | buffer[9]);
		gy_sum += (int16_t) (buffer[10] << 8 | buffer[11]);
		gz_sum += (int16_t) (buffer[12] << 8 | buffer[13]);
		HAL_Delay(1);
	}

	gx_bias = gx_sum / num_samples;
	gy_bias = gy_sum / num_samples;
	gz_bias = gz_sum / num_samples;
}

/*Accelerometer bias calculate*/
void calculate_accelerometer_bias() {
	// Number of samples to average
	int num_samples = 2000;
	float ax_sum = 0;
	float ay_sum = 0;
	float az_sum = 0;

	for (int i = 0; i < num_samples; i++) {
		uint8_t buffer[14];
		HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR, ACCEL_XOUT_H_REG,
		I2C_MEMADD_SIZE_8BIT, buffer, 14, 100);
		ax_sum += (float) (int16_t) (buffer[0] << 8 | buffer[1]) / ACC_SCALE;
		ay_sum += (float) (int16_t) (buffer[2] << 8 | buffer[3]) / ACC_SCALE;
		az_sum += (float) (int16_t) (buffer[4] << 8 | buffer[5]) / ACC_SCALE;
		HAL_Delay(1);
	}

	ax_bias = ax_sum / num_samples;
	ay_bias = ay_sum / num_samples;
	az_bias = az_sum / num_samples;
}

/* Accelerometer reading MPU6050 */
result mpu6050_accx() {
	uint8_t buffer[14];
	result res;
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR,
	ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, buffer, 14, 100);
	float imu_data = buffer[0] << 8 | buffer[1];
	res.status = status;
	res.data = imu_data / ACC_SCALE;
	return res;
}

result mpu6050_accy() {
	uint8_t buffer[14];
	result res;
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR,
	ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, buffer, 14, 100);
	float imu_data = buffer[2] << 8 | buffer[3];
	res.status = status;
	res.data = imu_data / ACC_SCALE;
	return res;
}

result mpu6050_accz() {
	result res;
	uint8_t buffer[14];

	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR,
	ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, buffer, 14, 100);
	float imu_data = buffer[4] << 8 | buffer[5];
	res.status = status;
	res.data = imu_data / ACC_SCALE;

	return res;
}

/* Gyroscope reading MPU6050 */
result mpu6050_gyrox() {
	result res;
	uint8_t buffer[14];
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR,
	ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, buffer, 14, 100);
	float imu_data = buffer[8] << 8 | buffer[9];
	res.status = status;
	res.data = imu_data / GYRO_SCALE;
	return res;
}

result mpu6050_gyroy() {
	result res;
	uint8_t buffer[14];
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR,
	ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, buffer, 14, 100);
	float imu_data = buffer[10] << 8 | buffer[11];
	res.status = status;
	res.data = imu_data / GYRO_SCALE;
	return res;
}

result mpu6050_gyroz() {
	result res;
	uint8_t buffer[14];
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR,
	ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, buffer, 14, 100);
	float imu_data = buffer[12] << 8 | buffer[13];
	res.status = status;
	res.data = imu_data / GYRO_SCALE;
	return res;
}

/* All data reading MPU6050 */
mpu_data mpu6050_data() {
	uint8_t buffer[14];
	HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR, ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT,
			buffer, 14, 100);
	mpu_data data;
	data.ax = (float) (int16_t) (buffer[0] << 8 | buffer[1]) / ACC_SCALE;
	data.ay = (float) (int16_t) (buffer[2] << 8 | buffer[3]) / ACC_SCALE;
	data.az = (float) (int16_t) (buffer[4] << 8 | buffer[5]) / ACC_SCALE;
	data.gx = (float) (int16_t) ((buffer[8] << 8 | buffer[9]) - gx_bias)
			/ GYRO_SCALE;
	data.gy = (float) (int16_t) ((buffer[10] << 8 | buffer[11]) - gy_bias)
			/ GYRO_SCALE;
	data.gz = (float) (int16_t) ((buffer[12] << 8 | buffer[13]) - gz_bias)
			/ GYRO_SCALE;
	return data;
}

/* Temperature reading MPU6050 */
short mpu6050_temp() {
	uint8_t buffer[14];
	HAL_I2C_Mem_Read(&hi2c1, IMU_ADDR, ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT,
			buffer, 14, 100);
	int16_t raw_temp = buffer[6] << 8 | buffer[7];
	return (raw_temp / 340.0) + 36.53;
}
