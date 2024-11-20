#ifndef KALMAN_H
#define KALMAN_H
#include "main.h"

#define DT_k 0.004  // Passo temporale (in secondi)
#define PROCESS_NOISE (DT_k * DT_k * 4 * 4)  // Rumore di processo
#define MEASURE_NOISE (3 * 3)  // Rumore di misura

typedef struct {
    float angle;            // Stato angolare (ad esempio Roll o Pitch)
    float uncertainty;      // Incertezza associata allo stato
    float kalmanGain;       // Guadagno di Kalman (calcolato internamente)
} Kalman;

// Initialize the Kalman filter
void kalman_init(Kalman* k, float angle);

// Update estimated angle
void kalmanUpdate(Kalman *k, float input, float measurement);


#endif // KALMAN_H
