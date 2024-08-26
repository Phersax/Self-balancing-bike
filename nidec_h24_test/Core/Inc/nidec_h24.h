#include "stm32f4xx_hal.h"

uint8_t nidec_h24_init(uint16_t Direction_Pin, GPIO_TypeDef *Direction_GPIO_Port);

void nidec_h24_Move(uint32_t dutyCycle, uint8_t dir);

uint32_t nidec_h24_getCnt(TIM_HandleTypeDef *htim);
uint32_t nidec_h24_getArr(TIM_HandleTypeDef *htim);
uint32_t nidec_h24_getCrr(TIM_HandleTypeDef *htim);

uint32_t get_rate_cha();
uint32_t get_rate_chb();

