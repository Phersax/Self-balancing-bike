#include "stm32f4xx_hal.h"

uint8_t mot_pwm_driver_init(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t prescaler, uint32_t CounterMode, uint32_t period);
uint8_t mot_pwm_driver_start(TIM_HandleTypeDef *htim, uint32_t Channel);
uint8_t mot_pwm_driver_stop(TIM_HandleTypeDef *htim, uint32_t Channel);

void mot_pwm_driver_SetPrescaler(TIM_HandleTypeDef *htim, uint32_t prescaler);
void mot_pwm_driver_SetPeriod(TIM_HandleTypeDef *htim, uint32_t period);
void mot_pwm_driver_SetCounter(TIM_HandleTypeDef *htim, uint32_t crr);
void mot_pwm_driver_SetDutyCycle(TIM_HandleTypeDef *htim, uint32_t dutyCycle);

uint32_t mot_pwm_driver_getCnt(TIM_HandleTypeDef *htim);
uint32_t mot_pwm_driver_getArr(TIM_HandleTypeDef *htim);
uint32_t mot_pwm_driver_getCrr(TIM_HandleTypeDef *htim);
