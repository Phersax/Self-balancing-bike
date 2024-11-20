#include <nidec_h24.h>
#include <stm32f4xx_hal.h>
#include <main.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

/*Default timer settings*/
extern TIM_HandleTypeDef htim5;

int32_t out_ccr;

/* To initialize the PWM and direction pins of the motor */
void nidec_h24_init() {

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };  // Declare the GPIO init struct

	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin : Direction_Pin */
	GPIO_InitStruct.Pin = DIRECTION_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DIRECTION_PORT, &GPIO_InitStruct);

	/*Configure GPIO pin : Brake_Pin */
	GPIO_InitStruct.Pin = BRAKE_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BRAKE_PORT, &GPIO_InitStruct);

	// Set the direction pin to low initially
	HAL_GPIO_WritePin(DIRECTION_PORT, DIRECTION_PIN, GPIO_PIN_RESET);

	// Set the brake pin to low initially
	HAL_GPIO_WritePin(BRAKE_PORT, BRAKE_PIN, GPIO_PIN_RESET);

	// Start PWM on the specified timer and channel
	TIM5->CCR1 = 0;
	HAL_TIM_Base_Start(&htim5);
	HAL_TIM_PWM_Start(&htim5, CHANNEL);
	nidec_h24_Move(0, 0, 0);
}

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min,
		int32_t out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/* To make the motor turn based on the input */
void nidec_h24_Move(float modv, float max_modv, uint8_t brk) {

	if (brk == 0) {
		// brk -> 0: brakes
		HAL_GPIO_WritePin(BRAKE_PORT, BRAKE_PIN, GPIO_PIN_RESET);
		TIM5->CCR1 = 0;
	}

	else {
		// brk -> 0: brakes
		HAL_GPIO_WritePin(BRAKE_PORT, BRAKE_PIN, GPIO_PIN_SET);

		if (modv > 0) {
			// counter-clockwise
			HAL_GPIO_WritePin(DIRECTION_PORT, DIRECTION_PIN, GPIO_PIN_SET);
			out_ccr = map(modv, 0, max_modv, 3938, 0);
		} else {
			// clockwise
			HAL_GPIO_WritePin(DIRECTION_PORT, DIRECTION_PIN, GPIO_PIN_RESET);
			out_ccr = map(modv, 0, -max_modv, 3938, 0);
		}

		if (out_ccr > 0 && out_ccr < htim5.Instance->ARR)
			// Set the PWM duty cycle
			//TIM5->CCR1 = htim5.Instance->ARR*(1 - fabs(dutyCycle)/100);
			TIM5->CCR1 = out_ccr;
		else
			TIM5->CCR1 = 0;

		// Generate an update event to reload the value immediately
	}

}

/*Get function at runtime*/

uint32_t nidec_h24_GetPeriod() {
	return __HAL_TIM_GET_AUTORELOAD(&htim5);
}

uint32_t nidec_h24_GetPrescaler() {
	return htim5.Instance->PSC;
}

uint32_t nidec_h24_GetCounter() {
	return __HAL_TIM_GET_COUNTER(&htim5);
}

uint32_t nidec_h24_GetDutyCycle() {
	uint32_t ccr = __HAL_TIM_GET_COMPARE(&htim5, CHANNEL);
	uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim5);
	return (ccr * 100) / (arr + 1);
}
