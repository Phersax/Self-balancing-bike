#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

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

void mpu6050_init();

/* Gyroscope reading MPU6050 */
short mpu6050_accx();
short mpu6050_accy();
short mpu6050_accz();

/* Accelerometer reading MPU6050 */
short mpu6050_gyrox();
short mpu6050_gyroy();
short mpu6050_gyroz();

/* Temperature reading MPU6050 */
short mpu6050_temp();

#endif /* INC_MPU6050_H_ */
