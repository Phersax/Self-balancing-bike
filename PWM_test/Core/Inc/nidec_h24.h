#include "stm32f4xx_hal.h"

//void nidec_h24_init(GPIO_TypeDef pin_dir, GPIO_TypeDef chA,GPIO_TypeDef chB); //pin pwm, pin direction, pin cha, pin chb
void nidec_h24_init(TIM_HandleTypeDef *htim, uint32_t Channel);
uint8_t nidec_h24_start(TIM_HandleTypeDef *htim, uint32_t Channel);
uint8_t nidec_h24_stop(TIM_HandleTypeDef *htim, uint32_t Channel);

void nidec_h24_SetPrescaler(TIM_HandleTypeDef *htim, uint32_t prescaler);
void nidec_h24_SetPeriod(TIM_HandleTypeDef *htim, uint32_t period);
void nidec_h24_SetCounter(TIM_HandleTypeDef *htim, uint32_t crr);
void nidec_h24_SetDutyCycle(TIM_HandleTypeDef *htim, uint32_t dutyCycle);

uint32_t nidec_h24_getCnt(TIM_HandleTypeDef *htim);
uint32_t nidec_h24_getArr(TIM_HandleTypeDef *htim);
uint32_t nidec_h24_getCrr(TIM_HandleTypeDef *htim);

uint32_t get_rate_cha();
uint32_t get_rate_chb();

