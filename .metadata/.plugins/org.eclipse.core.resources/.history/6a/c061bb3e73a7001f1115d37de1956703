/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdint.h>
#include "encoder.h"
#include "mpu6050.h"
#include "kalman_filter.h"
#include "nidec_h24.h"
#include "pid.h"
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
//encoder
static encoder_t enc;
static float rpm;

//IMU
static float distance, distance_error, distance_setpoint, new_angle, pid_out;
static mpu_data data;
static HAL_StatusTypeDef status;

//controllers
static PID_t pid;
static float Kp = 9;
static float Ki = 0.05;
static float Kd = 0.4;

//Control variables
static float rpm_limit = 0.03;
static float weight_balance = 5;
static float set_point = 0;
static float max_pid = 450;

//kalman
static float in_angle;
static Kalman filter;
static float angle_offset = -1;
static float pitch_angle;


//sampling time timer2
static float dt;

/* USER CODE BEGIN PV */

//prototype to get the dt
float getDt(TIM_HandleTypeDef *htim);

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_TIM5_Init();
	MX_I2C1_Init();
	MX_TIM1_Init();
	MX_TIM2_Init();
	/* USER CODE BEGIN 2 */

	//compute dt of timer2
	dt = getDt(&htim2);

	//pid config
	pid_init(&pid, Kp, Ki, Kd, -max_pid, max_pid, 150, -150);
	pid_set_setpoint(&pid, set_point);

	//imu init
	status = mpu6050_init();

	data = mpu6050_data(); //achieve the initial angle for the kalman filter

	in_angle = -atan(data.ax / sqrt(data.ay * data.ay + data.az * data.az))* 180 / M_PI;

	// Kalman filter init
	kalman_init(&filter, in_angle ); //set the initial angle

	//motor init
	nidec_h24_init();

	//encoder init
	encoder_init(&enc, AB, &htim1, 100);

	//the timer2 starts
	HAL_TIM_Base_Start_IT(&htim2);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {

		HAL_Delay(1);
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 84;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if (htim->Instance == TIM2) {

		data = mpu6050_data();

		new_angle = -atan(data.ax / sqrt(data.ay * data.ay + data.az * data.az))
				* 180 / M_PI;

		kalmanUpdate(&filter, data.gy, new_angle);


		pitch_angle = filter.angle - angle_offset;

		distance = -10 * pitch_angle;
		distance_error = distance_setpoint - distance;

		if (distance_error < distance_setpoint) {
			distance_setpoint -= weight_balance * dt;
		} else {
			distance_setpoint += weight_balance * dt;
		}

		rpm = fabs(encoder_get_pps(&enc));

		if (pid_out < 0 && rpm > 700)
			distance_setpoint -= rpm_limit;

		if (pid_out > 0 && rpm > 700)
			distance_setpoint += rpm_limit;

		pid_set_setpoint(&pid, distance_setpoint);

		if (pitch_angle < -20 || pitch_angle > 20) {
			distance_setpoint = 0;
			nidec_h24_Move(0, 0, 0);
			pid_reset(&pid);
		} else {
			pid_out = pid_compute_control_action(&pid, distance);
			nidec_h24_Move(pid_out, 450, 1);
		}
	}

}

float getDt(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2 || htim->Instance == TIM3
			|| htim->Instance == TIM4 || htim->Instance == TIM5
			|| htim->Instance == TIM6 || htim->Instance == TIM7)
		return (float) (((uint32_t) htim->Instance->ARR) + 1)
				* (((uint32_t) htim->Instance->PSC) + 1)
				/ ((uint32_t) HAL_RCC_GetPCLK1Freq() * 2);
	else
		return (float) (((uint32_t) htim->Instance->ARR) + 1)
				* (((uint32_t) htim->Instance->PSC) + 1)
				/ ((uint32_t) HAL_RCC_GetPCLK2Freq() * 2);
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
