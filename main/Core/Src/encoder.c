#include "encoder.h"
#include "main.h"
#include <math.h>

HAL_StatusTypeDef encoder_init(encoder_t *e, channel ch, TIM_HandleTypeDef *htim, uint32_t ppr) {

	HAL_StatusTypeDef ret = HAL_OK;

	if (htim == NULL)
		return HAL_ERROR;

	e->tim = htim;
	htim->Instance->CNT = 0;
	ret = HAL_TIM_Encoder_Start(htim, ch);

	if (ret != HAL_OK)
		return ret;

	//if the encoder is in quadrature, the resolution is 4
	if (ch == A || ch == B)
		e->resolution = 2;
	else
		e->resolution = 4;

	e->velocity_pps = 0.0;
	e->last_count = htim->Instance->CNT;
	e->ppr = ppr;

	return ret;
}

inline static void __encoder_update(encoder_t *e) {
	uint32_t cur_cnt;
	int32_t diff, cur_velocity;

	cur_cnt = e->tim->Instance->CNT;

	// Handle overflow and underflow
	if (__HAL_TIM_IS_TIM_COUNTING_DOWN(e->tim)) {
		if ((uint32_t)cur_cnt < (uint32_t)e->last_count) // underflow
			diff = (uint32_t)e->last_count - (uint32_t)cur_cnt;
		else
			diff = ((uint32_t)e->tim->Instance->ARR - (uint32_t)cur_cnt) + (uint32_t)e->last_count;
	} else {
		if (cur_cnt > e->last_count) // overflow
			diff = (uint32_t)e->last_count - (uint32_t)cur_cnt;
		else
			diff = ((uint32_t)e->tim->Instance->ARR - (uint32_t)e->last_count) + (uint32_t)cur_cnt;
	}

	// velocity in pulses per second
	if ((uint32_t)e->last_count == (uint32_t)cur_cnt)
		diff = 0;

	cur_velocity = (float) diff / DT_enc / (float) e->resolution;

	// Filtering velocity
	e->velocity_pps = BETA * e->velocity_pps + (1.0 - BETA) * cur_velocity;
	e->last_count = cur_cnt;
}

float encoder_get_velocity_rps(encoder_t *e) {
	__encoder_update(e);
	return e->velocity_pps / (float) e->ppr;
}

float encoder_get_velocity_rpm(encoder_t *e) {
	return encoder_get_velocity_rps(e) * 60.0;
}

float encoder_get_pps(encoder_t *e) {
	__encoder_update(e);
	return e->velocity_pps;
}
