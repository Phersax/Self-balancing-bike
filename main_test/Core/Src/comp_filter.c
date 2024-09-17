#include <comp_filter.h>
#include <math.h>

Orientation or;
angleGyro A_gyro;
angleAcc A_acc;

void updateOrientation(float accX, float accY, float accZ, float gyroX, float gyroY, float gyroZ) 
{
  // compute pitch and roll from accelerometer
  updateAngleAcc(accX, accY,accZ);

  // compute pitch and roll from gyroscope
  updateAngleGyro(gyroX, gyroY);

  // include complementary filter
  or.pitch = ALPHA * (or.pitch + A_gyro.pitchGyro) + (1 - ALPHA) * A_acc.pitchAcc;
  or.roll = ALPHA * (or.roll + A_acc.rollAcc) + (1 - ALPHA) * A_acc.rollAcc;
}

void updateAngleAcc(float accX, float accY, float accZ){
	// compute pitch and roll using trigonometric formulas
	A_acc.pitchAcc = atan(accX / sqrt(accY*accY + accZ*accZ)) * 180 / M_PI;
	A_acc.rollAcc = atan(-accY / sqrt(accX*accX + accZ*accZ)) * 180 / M_PI;
}

void updateAngleGyro(float gyroX, float gyroY){
	// compute pitch and roll by multiplying the sampling time by the angular velocity
	A_gyro.pitchGyro = gyroX * DT;
	A_gyro.pitchGyro = gyroY * DT;
}

/* Useful getter functions */
float getPitchAcc() {
  return A_acc.pitchAcc;
}
float getRollAcc() {
  return A_acc.rollAcc;
}

float getPitchGyro() {
  return A_gyro.pitchGyro;
}
float getRollGyro() {
  return A_gyro.rollGyro;
}

float getPitch() {
  return or.pitch;
}
float getRoll() {
  return or.roll;
}
