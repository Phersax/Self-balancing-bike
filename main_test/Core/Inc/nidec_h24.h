#include "stm32f4xx_hal.h"

void nidec_h24_init(uint16_t DIRECTION_Pin, GPIO_TypeDef *DIRECTION_GPIO_Port);

void nidec_h24_Move(int32_t dutyCycle);

uint32_t nidec_h24_getCnt(TIM_HandleTypeDef *htim);
uint32_t nidec_h24_getArr(TIM_HandleTypeDef *htim);
uint32_t nidec_h24_getCrr(TIM_HandleTypeDef *htim);

uint32_t get_rate_cha();
uint32_t get_rate_chb();

