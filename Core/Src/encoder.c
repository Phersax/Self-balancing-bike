#include "encoder.h"
#include "main.h"
#include <math.h>

uint32_t cur_cnt;

HAL_StatusTypeDef encoder_init(encoder_t *e, channel ch, TIM_HandleTypeDef *htim, uint32_t ppr)
{
    HAL_StatusTypeDef ret;
    ret = HAL_OK;
    if(htim == NULL){
        return HAL_ERROR;
    }

    e -> tim = htim;
    ret = HAL_TIM_Encoder_Start(htim, ch);
    if(ret != HAL_OK){
        return ret;
    }

    if (ch == A || ch == B) e -> resolution = 2;
    else e -> resolution = 4;

    e -> velocity_pps = 0.0;
    e -> last_sampling_t = HAL_GetTick();
    e -> last_count = htim -> Instance -> CNT;
    e -> ppr = ppr;

    return ret;
}

 static void __encoder_update(encoder_t *e)
{
    uint32_t now;
    int32_t diff, cur_velocity;
    float dt;

    now = HAL_GetTick();
    cur_cnt = e -> tim -> Instance -> CNT;

    diff = (int) cur_cnt - (int) e -> last_count;
    if (diff > (65535/2)){
    	diff -= 65535; // underflow 0->65535
    } else if (diff < -(65535/2)){
    	diff += 65535; // overflow 65535->0
    }

    dt = (float) (now - e -> last_sampling_t) / 1000.0;
    
    cur_velocity = (float) diff / dt / (float) e -> resolution;

    e -> velocity_pps = 0.95 * e -> velocity_pps + 0.05 * cur_velocity;
    
    e -> last_count = cur_cnt;
    e -> last_sampling_t = now;
    e -> tim -> Instance -> CNT = 0;
}

float encoder_get_velocity_rps(encoder_t *e)
{
    __encoder_update(e);
    return e -> velocity_pps / (float) e -> ppr;
}


float encoder_get_velocity_rpm(encoder_t *e)
{
    __encoder_update(e);
    return (e->velocity_pps / (float) e->ppr) * 60.0;
}


float encoder_get_velocity_rads(encoder_t *e)
{
    __encoder_update(e);
    return e->velocity_pps / (float) e -> ppr / (2.0*M_PI);
}

float encoder_get_position_deg(encoder_t *e)
{
    __encoder_update(e);
    return (float) cur_cnt / ((float) (2 * e -> ppr * e -> resolution)) * 360.0;
}

float encoder_get_position_rad(encoder_t *e)
{
    __encoder_update(e);
    return (float) cur_cnt / ((float) (2 * e -> ppr * e -> resolution)) * M_PI / 180.0;
}
