#include "stm32f4xx_hal.h"

/* To initialize the PWM and direction pins of the motor */
void nidec_h24_init(uint16_t DIRECTION_Pin, GPIO_TypeDef *DIRECTION_GPIO_Port);

/* To make the motor turn based on the input */
void nidec_h24_Move(float dutyCycle);

/* Useful getter functions to access the parameters of the PWM signal */
uint32_t nidec_h24_GetPeriod();
uint32_t nidec_h24_GetPrescaler();
uint32_t nidec_h24_GetCounter();
uint32_t nidec_h24_GetDutyCycle();
