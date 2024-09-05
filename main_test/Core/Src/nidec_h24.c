#include <nidec_h24.h>
#include <stm32f4xx_hal.h>
#include <main.h>
#include <stdio.h>
#include <stdint.h>

/*Default timer settings*/
extern  TIM_HandleTypeDef htim5;
static  uint32_t Channel = TIM_CHANNEL_1;
static  uint16_t direction_pin;
static  GPIO_TypeDef *direction_port;

/*Init the motor*/

void nidec_h24_init(uint16_t DIRECTION_Pin, GPIO_TypeDef *DIRECTION_GPIO_Port){

    GPIO_InitTypeDef GPIO_InitStruct = {0};  // Declare the GPIO init struct

    if (DIRECTION_GPIO_Port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (DIRECTION_GPIO_Port == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (DIRECTION_GPIO_Port == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (DIRECTION_GPIO_Port == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    } else if (DIRECTION_GPIO_Port == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    } else if (DIRECTION_GPIO_Port == GPIOF) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
    } else if (DIRECTION_GPIO_Port == GPIOG) {
        __HAL_RCC_GPIOG_CLK_ENABLE();
    } else if (DIRECTION_GPIO_Port == GPIOH) {
        __HAL_RCC_GPIOH_CLK_ENABLE();
    }

    direction_pin = DIRECTION_Pin;
    direction_port = DIRECTION_GPIO_Port;

	/*Configure GPIO pin : Direction_Pin */
	GPIO_InitStruct.Pin = DIRECTION_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DIRECTION_GPIO_Port, &GPIO_InitStruct);

	// Set the pin to low initially
	HAL_GPIO_WritePin(DIRECTION_GPIO_Port, DIRECTION_Pin, GPIO_PIN_RESET);

	// Start PWM on the specified timer and channel
	TIM5->CCR1 = 0;
	HAL_TIM_PWM_Start(&htim5, Channel);
}

void nidec_h24_Move(uint32_t dutyCycle, uint8_t dir){
	// Validate dutyCycle to be within 0 - 100%
    if (dutyCycle > 100) dutyCycle = 100;

    // Calculate the compare value
    //uint32_t ccr = (uint16_t)(dutyCycle * (float)(htim5.Instance->ARR + 1))/100;

    // Set the PWM duty cycle
    TIM5->CCR1 = dutyCycle;

	// Set direction motor
	HAL_GPIO_WritePin(direction_port, direction_pin, dir);

    // Generate an update event to reload the value immediately
    //htim5.Instance->EGR = TIM_EGR_UG;
}

/*Get function at runtime*/

// get period
uint32_t nidec_h24_GetPeriod()
{
    return __HAL_TIM_GET_AUTORELOAD(&htim5);
}

// get prescaler
uint32_t nidec_h24_GetPrescaler()
{
    return htim5.Instance->PSC;
}

// get counter
uint32_t nidec_h24_GetCounter()
{
    return __HAL_TIM_GET_COUNTER(&htim5);
}

// get duty cycle
uint32_t nidec_h24_GetDutyCycle()
{
    uint32_t ccr = __HAL_TIM_GET_COMPARE(&htim5, Channel);
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim5);
    return (ccr * 100) / (arr + 1);
}