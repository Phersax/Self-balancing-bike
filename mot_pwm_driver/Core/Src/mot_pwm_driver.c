#include <mot_pwm_driver.h>
#include <main.h>
#include <stdio.h>
#include <stdint.h>

/*Modify the timer's parameters in case */
uint8_t mot_pwm_driver_init(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t prescaler, uint32_t CounterMode, uint32_t period) 
{
	if (htim == NULL || Channel==NULL)
		return 0;
	else
	{	
		htim->Init.Prescaler = prescaler;
		htim->Init.CounterMode = CounterMode;
		htim->Init.Period = period;
		//htim->Init.ClockDivision = ClockDivision;
		//htim->Init.RepetitionCounter = RepetiotionCounter;

		HAL_StatusTypeDef HAL_TIM_PWM_Init(htim,Channel);
		return 1;
	}
}


uint8_t mot_pwm_driver_start(TIM_HandleTypeDef *htim, uint32_t Channel) //start the motor
{
	if (htim == NULL || Channel ==NULL )
		return 0;
	else
	{
		HAL_StatusTypeDef HAL_TIM_PWM_START(htim,Channel);
		return 1;
	}
}

uint8_t mot_pwm_driver_stop(TIM_HandleTypeDef *htim, uint32_t Channel) //stop the motor
{
	if (htim == NULL || Channel ==NULL )
		return 0;
	else
	{
		HAL_StatusTypeDef HAL_TIM_PWM_Stop(htim,Channel);
		return 1;
	}
}



/*Set function at runtime*/

//set the prescaler
void mot_pwm_driver_SetPrescaler(TIM_HandleTypeDef *htim, uint32_t prescaler){
	__HAL_TIM_SET_PRESCALER(htim,prescaler);
	htim->Instance->EGR = TIM_EGR_UG;
}

//set the period
void mot_pwm_driver_SetPeriod(TIM_HandleTypeDef *htim, uint32_t period){
	__HAL_TIM_SET_AUTORELOAD(htim,period);
	htim->Instance->EGR = TIM_EGR_UG;
}

//set the pulse
void mot_pwm_driver_SetCounter(TIM_HandleTypeDef *htim, uint32_t crr){
	__HAL_TIM_SET_COUNTER(htim,crr);
	htim->Instance->EGR = TIM_EGR_UG;
}

//set the duty cycle
void mot_pwm_driver_SetDutyCycle(TIM_HandleTypeDef *htim, uint32_t dutyCycle){
	uint16_t ccr=(uint16_t)(dutyCycle*((htim->Instance->ARR)+1));
	__HAL_TIM_SET_COMPARE(htim,htim->Channel,ccr);
	htim->Instance->EGR = TIM_EGR_UG;

}





/*Get function*/
uint32_t mot_pwm_driver_getCnt(TIM_HandleTypeDef *htim){ // get CNT
	 return __HAL_TIM_GET_COUNTER(htim);


}

uint32_t mot_pwm_driver_getCrr(TIM_HandleTypeDef *htim){ // get CNT
	if ((htim->Channel) == TIM_CHANNEL_1)
		return htim->Instance->CCR1;
	if ((htim->Channel) == TIM_CHANNEL_2)
		return htim->Instance->CCR2;
	if ((htim->Channel) == TIM_CHANNEL_3)
		return htim->Instance->CCR3;
	else return htim->Instance->CCR4;


}
uint32_t mot_pwm_driver_getArr(TIM_HandleTypeDef *htim){ // get ARR
	return __HAL_TIM_GET_AUTORELOAD(htim);

}




