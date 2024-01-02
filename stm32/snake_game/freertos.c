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
#include "dot_matrix_driver.h"
#include "tim.h"
#include "snakeGame.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADC_BUF_SIZE 3
#define ADC_X 1
#define ADC_Y 0
#define ADC_POT 2

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
	HAL_TIM_Base_Start(&htim16);

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
		for(uint8_t i = 0 ; i < MATRIX_SIZE ; ++i) {
			if(gameMatrix[i] != 0) {
				writeColDiodes(i, gameMatrix[i]);
				osDelay(2);
			}
		}
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

	//Make first dot
	char msg[80];

	uint8_t switchLock= 1;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); //R
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);   //G
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); //B

	initSnakeGame();

  /* Infinite loop */
	while(1)
  {
		if(switchLock == 1 && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET) {
			switchLock = 0;
			initSnakeGame();
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); //R
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);   //G
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); //B
		}
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_SET) {
			switchLock = 1;
		}

		updateDirection(adc_buf[ADC_Y], adc_buf[ADC_X]);
		updateSnakeGame();

		osDelay(300);

//		timerVal = __HAL_TIM_GET_COUNTER(&htim16);
  }
  /* USER CODE END GameLogic */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

