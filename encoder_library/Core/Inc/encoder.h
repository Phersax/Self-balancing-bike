#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "main.h"

#define BETA 0.95
#define DT 0.01

typedef enum {
    A = TIM_CHANNEL_1,
    B = TIM_CHANNEL_2,
	AB = TIM_CHANNEL_ALL
} channel;

typedef struct {
    TIM_HandleTypeDef *tim;
    uint32_t last_count;
    uint32_t ppr;  // pulses per revolution
    int resolution;  // counts per pulses
    uint32_t velocity_pps;
} encoder_t;

HAL_StatusTypeDef encoder_init(encoder_t *e, channel ch, TIM_HandleTypeDef *htim, uint32_t ppr);

float encoder_get_velocity_rps(encoder_t *encoder);
float encoder_get_velocity_rpm(encoder_t *encoder);
float encoder_get_velocity_rads(encoder_t *encoder);

#endif
