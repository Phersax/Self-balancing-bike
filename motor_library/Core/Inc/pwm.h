/*
 * pwm.h
 *
 *  Created on: Aug 23, 2024
 *      Author: simy
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#endif /* INC_PWM_H_ */

#include "stm32f4xx_hal.h"

uint8_t init(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t prescaler, uint32_t CounterMode, uint32_t period);
uint8_t start(TIM_HandleTypeDef *htim, uint32_t Channel);
uint8_t stop(TIM_HandleTypeDef *htim, uint32_t Channel);

void SetPrescaler(TIM_HandleTypeDef *htim, uint32_t prescaler);
void SetPeriod(TIM_HandleTypeDef *htim, uint32_t period);
void SetCounter(TIM_HandleTypeDef *htim, uint32_t crr);
void SetDutyCycle(TIM_HandleTypeDef *htim, uint32_t dutyCycle);

uint32_t getCnt(TIM_HandleTypeDef *htim);
uint32_t getArr(TIM_HandleTypeDef *htim);
uint32_t getCrr(TIM_HandleTypeDef *htim);
