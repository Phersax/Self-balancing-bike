#ifndef INC_KALMAN_FNS_H_
#define INC_KALMAN_FNS_H_

#define ALPHA 0.9 // complementary filter coefficient
#define DT  0.005 // sampling time

typedef struct {
  float pitch;
  float roll;
} Orientation;

typedef struct {
  float pitchGyro;
  float rollGyro;
} angleGyro;

typedef struct {
  float pitchAcc;
  float rollAcc;
} angleAcc;

/*Compute angles with complementary filter*/
void updateOrientation(float accX, float accY, float accZ, float gyroX, float gyroY, float gyroZ);

/*Compute angles with accelerometer*/
void updateAngleAcc(float accX, float accY, float accZ);

/*Compute angles with gyroscope filter*/
void updateAngleGyro(float gyroX, float gyroY);

/*Get angles estimations from complementary filter*/
float getPitch();
float getRoll();
float getYaw();

/*Get angles estimations from accelerometer*/
float getPitchAcc();
float getRollAcc();

/*Get angles estimations from gyroscope*/
float getPitchGyro();
float getRollGyro();

#endif /* INC_KALMAN_FNS_H_ */
