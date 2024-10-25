#include "stm32f4xx_hal.h"

#define DIRECTION_PIN GPIO_PIN_10
#define DIRECTION_PORT ((GPIO_TypeDef *) GPIOB_BASE)

#define BRAKE_PIN GPIO_PIN_4
#define BRAKE_PORT ((GPIO_TypeDef *) GPIOB_BASE)

#define CHANNEL TIM_CHANNEL_1

/* To initialize the PWM and direction pins of the motor */
void nidec_h24_init();

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min,
		int32_t out_max);

/* To make the motor turn based on the input */
void nidec_h24_Move(float modv, float max_modv, uint8_t brk);

/* Useful getter functions to access the parameters of the PWM signal */
uint32_t nidec_h24_GetPeriod();
uint32_t nidec_h24_GetPrescaler();
uint32_t nidec_h24_GetCounter();
uint32_t nidec_h24_GetDutyCycle();
