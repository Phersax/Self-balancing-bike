#include "encoder.h"
#include "main.h"
#include <math.h>

HAL_StatusTypeDef encoder_init(encoder_t *e, channel ch, TIM_HandleTypeDef *htim, uint32_t ppr)
{
    HAL_StatusTypeDef ret;
    ret = HAL_OK;
    if(htim == NULL) return HAL_ERROR;

    e -> tim = htim;
    htim -> Instance -> CNT = 0;
    ret = HAL_TIM_Encoder_Start(htim, ch);
    if(ret != HAL_OK) return ret;

    if (ch == A || ch == B) e -> resolution = 2;
    else e -> resolution = 4;

    e -> velocity_pps = 0.0;
    e -> last_count = htim -> Instance -> CNT;
    e -> ppr = ppr;

    return ret;
}

inline static void __encoder_update(encoder_t *e)
{
    int32_t diff, cur_velocity;

    uint32_t cur_cnt = e -> tim -> Instance -> CNT;

    diff = cur_cnt - e -> last_count;

    // Handle overflow and underflow
    if (diff > (65535 / 2)){
    	diff -= 65535; // underflow 0->65535
    } else if (diff < -(65535 / 2)){
    	diff += 65535; // overflow 65535->0
    }
    
    // velocity in pulses per second
    cur_velocity = (float) diff / DT / (float) e -> resolution;

    // Filtering velocity
    e -> velocity_pps = ALPHA * e -> velocity_pps + (1.0 - ALPHA) * cur_velocity;

    e -> tim -> Instance -> CNT = 0;
    e -> last_count = 0;
}

float encoder_get_velocity_rps(encoder_t *e)
{
    __encoder_update(e);
    return e -> velocity_pps / (float) e -> ppr;
}


float encoder_get_velocity_rpm(encoder_t *e)
{
    return encoder_get_velocity_rps(e) * 60.0;
}


float encoder_get_velocity_rads(encoder_t *e)
{
    return encoder_get_velocity_rps(e) / (2.0*M_PI);
}
