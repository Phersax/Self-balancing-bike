#include "kalman_filter.h"

// Filter initialization function
void kalman_init(Kalman* k, float angle) {
    k->angle = angle;
    k->uncertainty = 2*2;
    k->kalmanGain = 0; // Initial gain
}

// Kalman filter update function
inline void kalmanUpdate(Kalman *k, float input, float measurement) {
    // State prediction (adding speed multiplied by time)
    k->angle += DT_k * input;  //0.004

    // Uncertainty update
    k->uncertainty += PROCESS_NOISE;

    // Kalman gain calculation
    k->kalmanGain = k->uncertainty / (k->uncertainty + MEASURE_NOISE);

    // State correction with measurement (measurement weighted by Kalman gain)
    k->angle += k->kalmanGain * (measurement - k->angle);

    // Uncertainty update
    k->uncertainty = (1 - k->kalmanGain) * k->uncertainty;
}
