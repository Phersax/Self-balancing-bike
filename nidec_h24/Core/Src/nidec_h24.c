#include <nidec_h24.h>
#include <main.h>
#include <stdio.h>
#include <stdint.h>


/*Init the motor*/

void nidec_h24_init(TIM_HandleTypeDef *htim, uint32_t Channel) 
{
	
	HAL_TIM_PWM_Init(htim);
	HAL_TIM_PWM_Start(htim, Channel);
}

/*Modify the timer's parameters in case */
/*
uint8_t nidec_h24_init(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t prescaler, uint32_t CounterMode, uint32_t period) 
{
	if (htim == NULL || Channel==NULL)
		return 0;
	else
	{	
		htim->Init.Prescaler = prescaler;
		htim->Init.CounterMode = CounterMode;
		htim->Init.Period = period;
		//htim->Init.ClockDivision = ClockDivision;
		//htim->Init.RepetitionCounter = RepetitionCounter;

		HAL_TIM_PWM_Init(htim,Channel);
		return 1;
	}
}


uint8_t mot_pwm_driver_start(TIM_HandleTypeDef *htim, uint32_t Channel) //start the motor
{
	if (htim == NULL || Channel ==NULL )
		return 0;
	else
	{
		HAL_TIM_PWM_START(htim,Channel);
		return 1;
	}
}

uint8_t mot_pwm_driver_stop(TIM_HandleTypeDef *htim, uint32_t Channel) //stop the motor
{
	if (htim == NULL || Channel ==NULL )
		return 0;
	else
	{
		HAL_TIM_PWM_Stop(htim,Channel);
		return 1;
	}
}

*/

/*Set function at runtime*/

//set the prescaler
void nidec_h24_SetPrescaler(TIM_HandleTypeDef *htim, uint32_t prescaler){
	__HAL_TIM_SET_PRESCALER(htim,prescaler);
	htim->Instance->EGR = TIM_EGR_UG;
}

//set the period
void nidec_h24_SetPeriod(TIM_HandleTypeDef *htim, uint32_t period){
	__HAL_TIM_SET_AUTORELOAD(htim,period);
	htim->Instance->EGR = TIM_EGR_UG;
}

//set the pulse
void nidec_h24_SetCounter(TIM_HandleTypeDef *htim, uint32_t crr){
	__HAL_TIM_SET_COUNTER(htim,crr);
	htim->Instance->EGR = TIM_EGR_UG;
}

//set the duty cycle
void nidec_h24_SetDutyCycle(TIM_HandleTypeDef *htim, uint32_t dutyCycle){
	uint16_t ccr = (uint16_t)(dutyCycle * (float)((htim->Instance->ARR) + 1));
	__HAL_TIM_SET_COMPARE(htim,htim->Channel,ccr);
	htim->Instance->EGR = TIM_EGR_UG;
}