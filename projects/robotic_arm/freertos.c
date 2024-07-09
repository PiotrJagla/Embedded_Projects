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
/* Definitions for GrapperTask */
osThreadId_t GrapperTaskHandle;
const osThreadAttr_t GrapperTask_attributes = {
  .name = "GrapperTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StepperMotorTask(void *argument);
void GrapperTaskFunc(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buf, adc_buf_size);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

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

  /* creation of GrapperTask */
  GrapperTaskHandle = osThreadNew(GrapperTaskFunc, NULL, &GrapperTask_attributes);

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
		uint8_t dutyCycle = 70;
		uint8_t maxDutyCycle = 100;
		uint8_t minDutyCycle = 70;
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
			if(dutyCycle >= maxDutyCycle) {
				dutyCycle = maxDutyCycle;
			}
		} else if(xVal > 240) {
			dutyCycle--;
			if(dutyCycle <= minDutyCycle) {
				dutyCycle = minDutyCycle;
			}
		}

		htim2.Instance->CCR1 = dutyCycle;
		osDelay(30);
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

/* USER CODE BEGIN Header_GrapperTaskFunc */
/**
* @brief Function implementing the GrapperTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_GrapperTaskFunc */
void GrapperTaskFunc(void *argument)
{
  /* USER CODE BEGIN GrapperTaskFunc */
	uint8_t grapperDutyCycle = 70;
	uint8_t maxGrapperDutyCycle = 100;
	uint8_t minGrapperDutyCycle = 70;

  /* Infinite loop */
	while(1)
  {
		GPIO_PinState button1State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
		GPIO_PinState button2State = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
		if(button1State == GPIO_PIN_RESET) {
			grapperDutyCycle += 2;
			if(grapperDutyCycle >= maxGrapperDutyCycle) {
				grapperDutyCycle = maxGrapperDutyCycle;
			}
		} else if(button2State == GPIO_PIN_RESET) {
			grapperDutyCycle -= 2;
			if(grapperDutyCycle <= minGrapperDutyCycle) {
				grapperDutyCycle = minGrapperDutyCycle;
			}
		}

		htim2.Instance->CCR2 = grapperDutyCycle;
    osDelay(100);
  }
  /* USER CODE END GrapperTaskFunc */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

