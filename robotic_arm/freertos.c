/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motors_driver.h"
#include "tim.h"
#include "adc.h"
#include "usart.h"
#include "string.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define adc_x 0
#define adc_y 1
#define adc_buf_size 2

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

enum StepperMotorState stepperMotorState;
uint8_t adc_buf[2];


/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for stepperMotor */
osThreadId_t stepperMotorHandle;
const osThreadAttr_t stepperMotor_attributes = {
  .name = "stepperMotor",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StepperMotorTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buf, adc_buf_size);

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of stepperMotor */
  stepperMotorHandle = osThreadNew(StepperMotorTask, NULL, &stepperMotor_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
		stepperMotorState = STOPPED;
    disableStepperMotor();
		char msg[30];
		uint8_t xVal;
		uint8_t yVal;
		uint8_t dutyCycle = 75;
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  /* Infinite loop */
	while(1)
  {
		xVal = adc_buf[adc_x];
		yVal = adc_buf[adc_y];
		if(yVal < 10) {
			stepperMotorState = TURNING_LEFT;
		} else if(yVal > 240) {
			stepperMotorState = TURNING_RIGHT;
		} else {
			stepperMotorState = STOPPED;
			disableStepperMotor();
		}

		if(xVal < 10) {
			dutyCycle++;
			if(dutyCycle >= 100) {
				dutyCycle = 100;
			}
		} else if(xVal > 240) {
			dutyCycle--;
			if(dutyCycle <= 60) {
				dutyCycle = 60;
			}
		}

		htim2.Instance->CCR1 = (uint32_t)dutyCycle;

		//sprintf(msg, "X: %d, Y: %d, D: %d\r\n", adc_buf[adc_x], adc_buf[adc_y], (uint8_t)dutyCycle);
		//HAL_UART_Transmit(&huart2, msg, strlen(msg), HAL_MAX_DELAY);

		osDelay(30);


		//htim2.Instance->CCR1 = 75;
		//osDelay(1000);

		//htim2.Instance->CCR1 = 25;
		//osDelay(3000);

  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StepperMotorTask */
/**
* @brief Function implementing the stepperMotor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StepperMotorTask */
void StepperMotorTask(void *argument)
{
  /* USER CODE BEGIN StepperMotorTask */
  /* Infinite loop */
	while(1)
  {
		switch(stepperMotorState){
		case STOPPED:
			osDelay(500);
			break;
		case TURNING_LEFT:
			startTurningLeft();
			break;
		case TURNING_RIGHT:
			startTurningRight();
			break;
		default:
			break;
		}
  }
  /* USER CODE END StepperMotorTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

