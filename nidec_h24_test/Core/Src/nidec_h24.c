#include <nidec_h24.h>
#include <main.h>
#include <stdio.h>
#include <stdint.h>

/*Default timer settings*/
static TIM_HandleTypeDef htim5;
static uint32_t TIM_CHANNEL_1;
TIM5->CCR1 = 0;

typedef struct {
  GPIO_TypeDef * pGPIO;
  uint32_t       pin;
} gpio_pin_t;

/*Init the motor*/
void nidec_h24_init(uint16_t Direction_Pin, GPIO_TypeDef *Direction_GPIO_Port){

	/*Configure GPIO pin : Direction_Pin */
	GPIO_InitStruct.Pin = Direction_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(Direction_GPIO_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(Direction_GPIO_Port, Direction_Pin, GPIO_PIN_RESET);

	HAL_TIM_PWM_Start(htim5, TIM_CHANNEL_1);
	// Start PWM

}

void nidec_h24_Move(uint32_t dutyCycle, uint8_t dir){
	// Validate dutyCycle to be within 0 - 100%
    if (dutyCycle > 100) dutyCycle = 100;

    // Calculate the compare value
    uint32_t ccr = (uint16_t)(dutyCycle * (float)(htim5.Instance->ARR + 1))/100;

    // Set the PWM duty cycle
    //__HAL_TIM_SET_COMPARE(htim, Channel, ccr);
    TIM5->CCR1 = ccr;

	// Set direction motor
	HAL_GPIO_WritePin(Direction_GPIO_Port, Direction_Pin, dir);

    // Generate an update event to reload the value immediately
    htim5.Instance->EGR = TIM_EGR_UG;
}

/*Get function at runtime*/

// get period
uint32_t nidec_h24_GetPeriod(TIM_HandleTypeDef *htim)
{
    return __HAL_TIM_GET_AUTORELOAD(htim);
}

// get prescaler
uint32_t nidec_h24_GetPrescaler(TIM_HandleTypeDef *htim)
{
    return htim->Instance->PSC;
}

// get counter
uint32_t nidec_h24_GetCounter(TIM_HandleTypeDef *htim)
{
    return __HAL_TIM_GET_COUNTER(htim);
}

// get duty cycle
uint32_t nidec_h24_GetDutyCycle(TIM_HandleTypeDef *htim, uint32_t Channel)
{
    uint32_t ccr = __HAL_TIM_GET_COMPARE(htim, Channel);
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(htim);
    return (ccr * 100) / (arr + 1);
}
