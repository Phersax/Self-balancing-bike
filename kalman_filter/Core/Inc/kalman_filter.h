#ifndef KALMAN_H
#define KALMAN_H

#define DT 0.005

typedef struct {
    float Q_angle;
    float Q_bias;
    float R_measure;

    float angle;
    float bias;
    float rate;

    float P[2][2];
} Kalman;

// Initialize the Kalman filter
void Kalman_init(Kalman* k);

// Get the estimated angle
float Kalman_getAngle(Kalman* k, float newAngle, float newRate);

// Set the initial angle
void Kalman_setAngle(Kalman* k, float angle);

// Return the unbiased rate of change
float Kalman_getRate(Kalman* k);

// Functions to adjust the Kalman filter parameters
void Kalman_setQangle(Kalman* k, float Q_angle);
void Kalman_setQbias(Kalman* k, float Q_bias);
void Kalman_setRmeasure(Kalman* k, float R_measure);

// Functions to get the Kalman filter parameters
float Kalman_getQangle(Kalman* k);
float Kalman_getQbias(Kalman* k);
float Kalman_getRmeasure(Kalman* k);

#endif // KALMAN_H
