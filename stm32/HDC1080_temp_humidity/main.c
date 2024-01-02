/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>

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
TIM_HandleTypeDef htim16;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
#define TEMP_WHOLE 0
#define TEMP_DECIMAL 1
#define HUMI_WHOLE 2
#define HUMI_DECIMAL 3
#define READ_TEMP 0
#define READ_HUMI 1

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM16_Init(void);
/* USER CODE BEGIN PFP */
HAL_StatusTypeDef ret;
uint8_t tempBuf[4];
void lightNumberOnDigit(uint8_t number, uint8_t digit);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
	if(huart->Instance == huart1.Instance) {
		HAL_UART_Receive_IT(&huart1, tempBuf, 4);
	}
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
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
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */

  uint16_t timer_val;


  uint8_t pcUartBuf[70];

  //HAL_TIM_Base_Start_IT(&htim16);


  int counter = 0;
  uint8_t digitOne;
  uint8_t digitTwo;
  uint8_t digitThree;
  uint8_t digitFour;
  GPIO_PinState buttonState;
  int buttonLock = 1;
  int readTempOrHumidity = 0;
  HAL_UART_Receive_IT(&huart1, tempBuf, 4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
	  //RECIEVING DATA FROM SEND FROM ATMEGA
	  //ret = HAL_UART_Receive(&huart1, tempBuf, 4, 100);
	  if(counter++ == 30) {
		  counter = 0;
		  if(ret != HAL_OK) {
			  //sprintf(pcUartBuf, "Error with uart: %d\r\n", ret);
			  sprintf(pcUartBuf, "Values after error: %d, %d, %d, %d, ERR: %d\r\n", tempBuf[0], tempBuf[1], tempBuf[2], tempBuf[3], ret);
		  }
		  else {
			  if(readTempOrHumidity == READ_TEMP) {
				  digitOne = tempBuf[TEMP_WHOLE]/10;
				  digitTwo = tempBuf[TEMP_WHOLE]%10;
				  digitThree = tempBuf[TEMP_DECIMAL]/10;
				  digitFour = tempBuf[TEMP_DECIMAL]%10;
				  lightTempIndicator();
			  } else if(readTempOrHumidity == READ_HUMI) {
				  digitOne = tempBuf[HUMI_WHOLE]/10;
				  digitTwo = tempBuf[HUMI_WHOLE]%10;
				  digitThree = tempBuf[HUMI_DECIMAL]/10;
				  digitFour = tempBuf[HUMI_DECIMAL]%10;
				  lightHumidityIndicator();
			  }

			  sprintf(pcUartBuf, "Data: %d, %d, %d, %d\r\n", tempBuf[0], tempBuf[1], tempBuf[2], tempBuf[3]);
		  }
		  HAL_UART_Transmit(&huart2, pcUartBuf, strlen(pcUartBuf), 100);
	  }

	  if(buttonLock == 1 && HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == GPIO_PIN_RESET) {
		  buttonLock = 0;
		  readTempOrHumidity = !readTempOrHumidity;
		  if(readTempOrHumidity == READ_TEMP) {
			  digitOne = tempBuf[TEMP_WHOLE]/10;
			  digitTwo = tempBuf[TEMP_WHOLE]%10;
			  digitThree = tempBuf[TEMP_DECIMAL]/10;
			  digitFour = tempBuf[TEMP_DECIMAL]%10;
			  lightTempIndicator();
		  } else if(readTempOrHumidity == READ_HUMI) {
			  digitOne = tempBuf[HUMI_WHOLE]/10;
			  digitTwo = tempBuf[HUMI_WHOLE]%10;
			  digitThree = tempBuf[HUMI_DECIMAL]/10;
			  digitFour = tempBuf[HUMI_DECIMAL]%10;
			  lightHumidityIndicator();
		  }
	  } else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == GPIO_PIN_SET) {
		  buttonLock = 1;
	  }

	  //LIGHTING HEX DISPLAYS

	  lightNumberOnDigit(digitOne, 1);
	  HAL_Delay(1);
	  lightNumberOnDigit(digitTwo, 2);
	  HAL_Delay(1);
	  lightNumberOnDigit(digitThree, 3);
	  HAL_Delay(1);
	  lightNumberOnDigit(digitFour, 4);
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
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_TIM16;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.Tim16ClockSelection = RCC_TIM16CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 8000-1;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 5000 -1;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2|GPIO_PIN_10|LD2_Pin|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 PA6 PA8 PA9
                           PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB2 PB10 LD2_Pin PB14
                           PB15 PB4 PB5 PB6
                           PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_10|LD2_Pin|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC7 PC10 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */

/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
	if(htim == &htim16) {
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
	}
}
void lightTempIndicator() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,RESET); //A
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,RESET); //B
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,RESET); //C
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,SET); //D
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,SET); //E
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,SET); //F
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,SET); //G
}
void lightHumidityIndicator() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,RESET); //A
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,RESET); //B
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,SET); //C
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,RESET); //D
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,SET); //E
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,SET); //F
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,SET); //G
}
void writeD4(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, pinState);
}
void writeD3(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, pinState);
}
void writeD2(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, pinState);
}
void writeD1(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, pinState);
}
void writeDP(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, pinState);
}
void writeG(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, pinState);
}
void writeF(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, pinState);
}
void writeE(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, pinState);
}
void writeD(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, pinState);
}
void writeC(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, pinState);
}
void writeB(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, pinState);
}
void writeA(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, pinState);
}
void lightZero() {
	writeA(SET);
	writeB(SET);
	writeC(SET);
	writeD(SET);
	writeE(SET);
	writeF(SET);
	writeG(RESET);
}
void lightOne() {
	writeA(RESET);
	writeB(SET);
	writeC(SET);
	writeD(RESET);
	writeE(RESET);
	writeF(RESET);
	writeG(RESET);
}
void lightTwo() {
	writeA(SET);
	writeB(SET);
	writeC(RESET);
	writeD(SET);
	writeE(SET);
	writeF(RESET);
	writeG(SET);
}
void lightThree() {
	writeA(SET);
	writeB(SET);
	writeC(SET);
	writeD(SET);
	writeE(RESET);
	writeF(RESET);
	writeG(SET);
}
void lightFour() {
	writeA(RESET);
	writeB(SET);
	writeC(SET);
	writeD(RESET);
	writeE(RESET);
	writeF(SET);
	writeG(SET);
}
void lightFive() {
	writeA(SET);
	writeB(RESET);
	writeC(SET);
	writeD(SET);
	writeE(RESET);
	writeF(SET);
	writeG(SET);
}
void lightSix() {
	writeA(SET);
	writeB(RESET);
	writeC(SET);
	writeD(SET);
	writeE(SET);
	writeF(SET);
	writeG(SET);
}
void lightSeven() {
	writeA(SET);
	writeB(SET);
	writeC(SET);
	writeD(RESET);
	writeE(RESET);
	writeF(RESET);
	writeG(RESET);
}
void lightEight() {
	writeA(SET);
	writeB(SET);
	writeC(SET);
	writeD(SET);
	writeE(SET);
	writeF(SET);
	writeG(SET);
}
void lightNine() {
	writeA(SET);
	writeB(SET);
	writeC(SET);
	writeD(SET);
	writeE(RESET);
	writeF(SET);
	writeG(SET);
}
void lightNumberOnDigit(uint8_t number, uint8_t digit) {
	switch(digit) {
	case 1:
		writeD1(RESET);
		writeD2(SET);
		writeD3(SET);
		writeD4(SET);
		writeDP(RESET);
		break;
	case 2:
		writeD1(SET);
		writeD2(RESET);
		writeD3(SET);
		writeD4(SET);
		writeDP(SET);
		break;
	case 3:
		writeD1(SET);
		writeD2(SET);
		writeD3(RESET);
		writeD4(SET);
		writeDP(RESET);
		break;
	case 4:
		writeD1(SET);
		writeD2(SET);
		writeD3(SET);
		writeD4(RESET);
		writeDP(RESET);
		break;
	}

	switch(number) {
	case 0:
		lightZero();
		break;
	case 1:
		lightOne();
		break;
	case 2:
		lightTwo();
		break;
	case 3:
		lightThree();
		break;
	case 4:
		lightFour();
		break;
	case 5:
		lightFive();
		break;
	case 6:
		lightSix();
		break;
	case 7:
		lightSeven();
		break;
	case 8:
		lightEight();
		break;
	case 9:
		lightNine();
		break;
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
