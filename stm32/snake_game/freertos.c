/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "usart.h"
#include "adc.h"
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADC_BUF_SIZE 3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint8_t adc_buf[ADC_BUF_SIZE];
/* USER CODE END Variables */
/* Definitions for MatrixRenderTas */
osThreadId_t MatrixRenderTasHandle;
const osThreadAttr_t MatrixRenderTas_attributes = {
  .name = "MatrixRenderTas",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for GameLogicTask */
osThreadId_t GameLogicTaskHandle;
const osThreadAttr_t GameLogicTask_attributes = {
  .name = "GameLogicTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void MatrixRender(void *argument);
void GameLogic(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buf, ADC_BUF_SIZE);
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
  /* creation of MatrixRenderTas */
  MatrixRenderTasHandle = osThreadNew(MatrixRender, NULL, &MatrixRenderTas_attributes);

  /* creation of GameLogicTask */
  GameLogicTaskHandle = osThreadNew(GameLogic, NULL, &GameLogicTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_MatrixRender */
/**
  * @brief  Function implementing the MatrixRenderTas thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_MatrixRender */
void MatrixRender(void *argument)
{
  /* USER CODE BEGIN MatrixRender */
  /* Infinite loop */
	while(1)
  {

		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
    osDelay(1000);
  }
  /* USER CODE END MatrixRender */
}

/* USER CODE BEGIN Header_GameLogic */
/**
* @brief Function implementing the GameLogicTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_GameLogic */
void GameLogic(void *argument)
{
  /* USER CODE BEGIN GameLogic */
	char msg[30];
	uint8_t xVal=0;
	uint8_t yVal=0;
	uint8_t potVal = 0;
  /* Infinite loop */
	while(1)
  {

		potVal = adc_buf[2];
		xVal = adc_buf[1];
		yVal = adc_buf[0];

		sprintf(msg, "X: %hu, Y: %hu, pot: %hu\r\n", xVal, yVal, potVal);
		HAL_UART_Transmit(&huart2, msg, strlen(msg), HAL_MAX_DELAY);
		osDelay(200);
  }
  /* USER CODE END GameLogic */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

