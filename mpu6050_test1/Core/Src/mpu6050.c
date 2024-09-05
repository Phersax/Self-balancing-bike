#include <mpu6050.h>
#include <main.h>
#include <stdio.h>
#include <stdint.h>

extern I2C_HandleTypeDef hi2c1;

/* Define global variables */
short gx_bias = 0;
short gy_bias = 0;
short gz_bias = 0;

void calculate_gyroscope_bias() {
    int num_samples = 1000;  // Number of samples to average
    long gx_sum = 0;
    long gy_sum = 0;
    long gz_sum = 0;

    for (int i = 0; i < num_samples; i++) {
        gx_sum += mpu6050_gyrox();
        gy_sum += mpu6050_gyroy();
        gz_sum += mpu6050_gyroz();
        HAL_Delay(1);  // Small delay to allow time between samples
    }

    gx_bias = gx_sum / num_samples;
    gy_bias = gy_sum / num_samples;
    gz_bias = gz_sum / num_samples;
}

void mpu6050_init() {
    HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(&hi2c1, MPU6050_IMU_ADDR, 1, 100);
    if (status == HAL_OK) {
        printf("MPU6050 is ready \n");
        printf("MPU6050 configuration... \n");

        uint8_t data;

        // Power management register 0X6B must be all 0s to wake the sensor up
        data = 0x00;
        HAL_I2C_Mem_Write(&hi2c1, MPU6050_IMU_ADDR, MPU6050_PWR_MGMT_1_REG, 1, &data, 1, 100);

        // Set DATA RATE of 1KHz by writing SMPLRT_DIV register
        data = 0x07;
        HAL_I2C_Mem_Write(&hi2c1, MPU6050_IMU_ADDR, MPU6050_SMPLRT_DIV_REG, 1, &data, 1, 100);

        // Set accelerometer configuration in ACCEL_CONFIG Register
        data = MPU6050_FS_ACC_2G;
        HAL_I2C_Mem_Write(&hi2c1, MPU6050_IMU_ADDR, MPU6050_ACCEL_CONFIG_REG, 1, &data, 1, 100);

        // Set Gyroscope configuration in GYRO_CONFIG Register
        data = MPU6050_FS_GYRO_250;
        HAL_I2C_Mem_Write(&hi2c1, MPU6050_IMU_ADDR, MPU6050_GYRO_CONFIG_REG, 1, &data, 1, 100);
    } else {
        printf("MPU6050 error, check connection cables \n");
    }
}

// Read MPU6050 Data
void mpu6050_read_data(uint8_t* buffer) {
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_IMU_ADDR, MPU6050_ACCEL_XOUT_H_REG, I2C_MEMADD_SIZE_8BIT, buffer, 14, 200);
}

// Accelerometer readings
int16_t mpu6050_raw_accx() {
    uint8_t buffer[14];
    mpu6050_read_data(buffer);
    return (buffer[0] << 8) | buffer[1];
}

int16_t mpu6050_raw_accy() {
    uint8_t buffer[14];
    mpu6050_read_data(buffer);
    return (buffer[2] << 8) | buffer[3];
}

int16_t mpu6050_raw_accz() {
    uint8_t buffer[14];
    mpu6050_read_data(buffer);
    return (buffer[4] << 8) | buffer[5];
}

int16_t mpu6050_accx() {
    return mpu6050_raw_accx() / 16384.0;
}

int16_t mpu6050_accy() {
	return mpu6050_raw_accy() / 16384.0;
}

int16_t mpu6050_accz() {
	return mpu6050_raw_accz() / 14418.0;
}

// Temperature reading
short mpu6050_raw_temp() {
    uint8_t buffer[14];
    mpu6050_read_data(buffer);
    return (buffer[6] << 8) | buffer[7];
}


// Gyroscope readings
int16_t mpu6050_raw_gyrox() {
    uint8_t buffer[14];
    mpu6050_read_data(buffer);
    return (buffer[8] << 8) | buffer[9];
}

int16_t mpu6050_raw_gyroy() {
    uint8_t buffer[14];
    mpu6050_read_data(buffer);
    return (buffer[10] << 8) | buffer[11];
}

int16_t mpu6050_raw_gyroz() {
    uint8_t buffer[14];
    mpu6050_read_data(buffer);
    return (buffer[12] << 8) | buffer[13];
}

int16_t mpu6050_gyrox() {
	return mpu6050_raw_gyrox() / 131.0;
}

int16_t mpu6050_gyroy() {
	return mpu6050_raw_gyroy() / 131.0;
}

int16_t mpu6050_gyroz() {
	return mpu6050_raw_gyroz() / 131.0;
}



float mpu6050_temp_celsius() {
    short raw_temp = mpu6050_raw_temp();
    return (raw_temp / 340.0) + 36.53;
}

// Kalman filter variables
float Q_angle = 0.001;   // Process noise variance (Q)
float R_measure = 0.03;  // Measurement noise variance (R)

float angle = 0;         // Current angle estimate (state)
float bias = 0;          // Gyroscope bias
float rate = 0;          // Angular rate
float P[2][2] = {{0, 0}, {0, 0}};  // Error covariance matrix

float kalman_filter(float new_angle, float new_rate, float dt) {
    // Prediction step
    rate = new_rate - bias;
    // Subtract the gyroscope bias from the angular rate
    angle += dt * rate;
    // Update the predicted angle

    // Update the error covariance matrix
    P[0][0] += dt * (dt * P[1][1] - P[0][1] - P[1][0] + Q_angle);
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += Q_angle * dt;

    // Calculate the Kalman gain
    float S = P[0][0] + R_measure;   // Measurement error
    float K[2];                      // Kalman gain
    K[0] = P[0][0] / S;
    K[1] = P[1][0] / S;

    // Update the angle with the measurement
    float y = new_angle - angle;
    // Difference between measurement and prediction
    angle += K[0] * y;
    bias += K[1] * y;

    // Update the error covariance matrix
    float P00_temp = P[0][0];
    float P01_temp = P[0][1];

    P[0][0] -= K[0] * P00_temp;
    P[0][1] -= K[0] * P01_temp;
    P[1][0] -= K[1] * P00_temp;
    P[1][1] -= K[1] * P01_temp;

    return angle;
}
