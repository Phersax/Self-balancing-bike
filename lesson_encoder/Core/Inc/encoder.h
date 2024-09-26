#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "main.h"

typedef enum {
    ENCODER_RES_2 = 2,
    ENCODER_RES_4 = 4
} encoder_resolution_t;

typedef struct {
    TIM_HandleTypeDef *tim;
    uint32_t last_sampling_t;
    uint32_t last_count;
    uint32_t ppr;
    encoder_resolution_t resolution;
    float velocity_pps;
} encoder_t;

HAL_StatusTypeDef encoder_init(encoder_t *e, encoder_resolution_t resolution, TIM_HandleTypeDef *htim, uint32_t ppr);

float encoder_get_velocity_rps(encoder_t *encoder);
float encoder_get_velocity_rpm(encoder_t *encoder);
float encoder_get_velocity_rads(encoder_t *encoder);

float encoder_get_position_deg(encoder_t *encoder);
float encoder_get_position_rads(encoder_t *encoder);

#endif
