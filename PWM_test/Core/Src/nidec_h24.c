#include <nidec_h24.h>
#include <main.h>
#include <stdio.h>
#include <stdint.h>


/*Init the motor*/



/*Modify the timer's parameters in case */

uint8_t nidec_h24_start_pwm(TIM_HandleTypeDef *htim, uint32_t Channel)
{
    // Validate input parameters
    if (htim == NULL)
        return 0; // Return 0 if the timer handle is NULL

    // Validate the channel number
    if (Channel != TIM_CHANNEL_1 && Channel != TIM_CHANNEL_2 &&
        Channel != TIM_CHANNEL_3 && Channel != TIM_CHANNEL_4)
        return 0; // Return 0 if the channel is invalid

    // Start PWM
    if (HAL_TIM_PWM_Start(htim, Channel) != HAL_OK)
        return 0; // Return 0 if PWM start failed

    return 1; // Return 1 if PWM start succeeded
}

uint8_t nidec_h24_stop_pwm(TIM_HandleTypeDef *htim, uint32_t Channel)
{
    // Validate input parameters
    if (htim == NULL)
        return 0; // Return 0 if the timer handle is NULL

    // Validate the channel number
    if (Channel != TIM_CHANNEL_1 && Channel != TIM_CHANNEL_2 &&
        Channel != TIM_CHANNEL_3 && Channel != TIM_CHANNEL_4)
        return 0; // Return 0 if the channel is invalid

    // Stop PWM
    if (HAL_TIM_PWM_Stop(htim, Channel) != HAL_OK)
        return 0; // Return 0 if PWM stop failed

    return 1; // Return 1 if PWM stop succeeded
}

/*Set function at runtime*/

//set the prescaler
void nidec_h24_SetPrescaler(TIM_HandleTypeDef *htim, uint32_t prescaler)
{
    // Set the new prescaler value
    __HAL_TIM_SET_PRESCALER(htim, prescaler);

    // Generate an update event to ensure the prescaler takes effect immediately
    htim->Instance->EGR = TIM_EGR_UG;
}

//set the period
void nidec_h24_SetPeriod(TIM_HandleTypeDef *htim, uint32_t period)
{
    // Set the auto-reload register (ARR) to define the new period
    __HAL_TIM_SET_AUTORELOAD(htim, period);

    // Generate an update event to ensure the new period takes effect immediately
    htim->Instance->EGR = TIM_EGR_UG;
}

//set the pulse
void nidec_h24_SetCounter(TIM_HandleTypeDef *htim, uint32_t crr)
{
    // Set the counter value
    __HAL_TIM_SET_COUNTER(htim, crr);

    // Generate an update event to ensure the counter is updated immediately
    htim->Instance->EGR = TIM_EGR_UG;
}

//set the duty cycle
void nidec_h24_SetDutyCycle(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t dutyCycle)
{
    // Validate dutyCycle to be within 0 - 100%
    if (dutyCycle > 100) dutyCycle = 100;

    // Calculate the compare value
    uint32_t ccr = dutyCycle;
    // uint32_t ccr = (uint16_t)(dutyCycle * (float)(htim->Instance->ARR + 1));

    // Set the PWM duty cycle
    //__HAL_TIM_SET_COMPARE(htim, Channel, ccr);
    switch (Channel)
        {
            case TIM_CHANNEL_1:
                htim->Instance->CCR1 = ccr;
                break;
            case TIM_CHANNEL_2:
                htim->Instance->CCR2 = ccr;
                break;
            case TIM_CHANNEL_3:
                htim->Instance->CCR3 = ccr;
                break;
            case TIM_CHANNEL_4:
                htim->Instance->CCR4 = ccr;
                break;
            default:
                return;
        }

    // Generate an update event to reload the value immediately
    htim->Instance->EGR = TIM_EGR_UG;
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
