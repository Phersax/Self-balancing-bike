#include "kalman_filter.h"

Kalman filter;

void Kalman_init(Kalman* k) {
    /* We will set the variables like so, these can also be tuned by the user */
    k->Q_angle = 0.001f;
    k->Q_bias = 0.003f;
    k->R_measure = 0.03f;

    k->angle = 0.0f; // Reset the angle
    k->bias = 0.0f;  // Reset bias

    k->P[0][0] = 0.0f; // Since we assume that the bias is 0 and we know the starting angle, the error covariance matrix is set like so
    k->P[0][1] = 0.0f;
    k->P[1][0] = 0.0f;
    k->P[1][1] = 0.0f;
}

// The angle should be in degrees and the rate should be in degrees per second
float Kalman_getAngle(Kalman* k, float newAngle, float newRate) {
    // Discrete Kalman filter time update equations - Time Update ("Predict")
    // Update xhat - Project the state ahead
    /* Step 1 */
    k->rate = newRate - k->bias;
    k->angle += DT * k->rate;

    // Update estimation error covariance - Project the error covariance ahead
    /* Step 2 */
    k->P[0][0] += DT * (DT * k->P[1][1] - k->P[0][1] - k->P[1][0] + k->Q_angle);
    k->P[0][1] -= DT * k->P[1][1];
    k->P[1][0] -= DT * k->P[1][1];
    k->P[1][1] += k->Q_bias * DT;

    // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
    // Calculate Kalman gain - Compute the Kalman gain
    /* Step 4 */
    float S = k->P[0][0] + k->R_measure; // Estimate error
    /* Step 5 */
    float K[2]; // Kalman gain - This is a 2x1 vector
    K[0] = k->P[0][0] / S;
    K[1] = k->P[1][0] / S;

    // Calculate angle and bias - Update estimate with measurement zk (newAngle)
    /* Step 3 */
    float y = newAngle - k->angle; // Angle difference
    /* Step 6 */
    k->angle += K[0] * y;
    k->bias += K[1] * y;

    // Calculate estimation error covariance - Update the error covariance
    /* Step 7 */
    float P00_temp = k->P[0][0];
    float P01_temp = k->P[0][1];

    k->P[0][0] -= K[0] * P00_temp;
    k->P[0][1] -= K[0] * P01_temp;
    k->P[1][0] -= K[1] * P00_temp;
    k->P[1][1] -= K[1] * P01_temp;

    return k->angle;
}

void Kalman_setAngle(Kalman* k, float angle) { k->angle = angle; }
// Used to set angle, this should be set as the starting angle
float Kalman_getRate(Kalman* k) { return k->rate; }
// Return the unbiased rate

/* These are used to tune the Kalman filter */
void Kalman_setQangle(Kalman* k, float Q_angle) { k->Q_angle = Q_angle; }
void Kalman_setQbias(Kalman* k, float Q_bias) { k->Q_bias = Q_bias; }
void Kalman_setRmeasure(Kalman* k, float R_measure) { k->R_measure = R_measure; }

float Kalman_getQangle(Kalman* k) { return k->Q_angle; }
float Kalman_getQbias(Kalman* k) { return k->Q_bias; }
float Kalman_getRmeasure(Kalman* k) { return k->R_measure; }
