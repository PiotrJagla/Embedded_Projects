/*
 * smakeGame.c
 *
 *  Created on: Dec 27, 2023
 *      Author: root
 */
#include "snakeGame.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "usart.h"

uint8_t gameMatrix[8] = {0,32,0,0,0,0,0,0};
struct player snake = {
		{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		1,0,1};
uint8_t isInGame = 0;
uint8_t dotCollected = 0;

void pushRandomDot() {
	uint8_t randVal = HAL_GetTick()%64;
	uint8_t randCol = randVal/MATRIX_SIZE;
	uint8_t randRow = randVal%MATRIX_SIZE;
	gameMatrix[randCol] |= (1<<randRow);

}

void printSnakeState() {
	char str[80];
	sprintf(str, "\n\rp: %hu, x1: %hu, y1: %hu, x2: %hu, y2: %hu, x3: %hu, y3: %hu\r\n",
			snake.points, snake.xPositions[0], snake.yPositions[0],
			snake.xPositions[1], snake.yPositions[1], snake.xPositions[2], snake.yPositions[2]);
	HAL_UART_Transmit(&huart2,str, strlen(str), HAL_MAX_DELAY);
}


void initSnakeGame(){
	//reset game matrix
	for(uint8_t i = 0 ; i < MATRIX_SIZE ; ++i) {
		gameMatrix[i] = 0;
	}

	//reset snake positions
	for(uint8_t i = 1; i < 10; ++i) {
		snake.xPositions[i] = 0;
		snake.yPositions[i] = 0;
	}
	snake.xPositions[0] = 2;
	snake.yPositions[0] = 4;
	snake.xDir = 1;
	snake.yDir = 0;
	snake.points = 1;


	isInGame = 1;
	pushRandomDot();
	gameMatrix[snake.yPositions[0]] |= (1<<snake.xPositions[0]);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); //R
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);   //G
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); //B
}

int8_t isOutOfBounds() {
		if(snake.xPositions[0] >= MATRIX_SIZE) {
			return 1;
		} else if(snake.xPositions[0] < 0) {
			return 1;
		}
		if(snake.yPositions[0] >= MATRIX_SIZE) {
			return 1;
		} else if(snake.yPositions[0] < 0) {
			return 1;
		}
		return 0;
}

void updateSnakeGame(){
	if(isInGame == 1) {
		if(dotCollected == 0) {
			gameMatrix[snake.yPositions[snake.points - 1]] &= ~(1<<snake.xPositions[snake.points-1]);
		} else {
			snake.points++;
		}
		for(int8_t i = snake.points - 2 ; i >= 0; --i) {
			snake.xPositions[i+1] = snake.xPositions[i];
			snake.yPositions[i+1] = snake.yPositions[i];
		}
		snake.xPositions[0] += snake.xDir;
		snake.yPositions[0] += snake.yDir;


		//Is out of bounds
		if(isOutOfBounds() == 1) {
			isInGame = 0;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); //R
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);   //G
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); //B
		}


		//dot collected
		if(gameMatrix[snake.yPositions[0]] & (1<<snake.xPositions[0])) {
			pushRandomDot();

			dotCollected = 1;

			//blink when dot is collected
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6); //B
			osDelay(50);
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6); //B
		} else {
			dotCollected = 0;
		}


		//Draw new position
		gameMatrix[snake.yPositions[0]] |= (1<<snake.xPositions[0]);
	}
}

void updateDirection(uint8_t yJoystickVal, uint8_t xJoystickVal) {
	if(snake.yDir == 0) {
		if(xJoystickVal < 10) {
				snake.yDir = 1;
				snake.xDir = 0;
		}
		else if(xJoystickVal > 240) {
			snake.yDir = -1;
			snake.xDir = 0;
		}
	}

	if(snake.xDir == 0) {
		if(yJoystickVal < 10) {
				snake.xDir = -1;
				snake.yDir = 0;
			}
			else if(yJoystickVal > 240) {
				snake.xDir = 1;
				snake.yDir = 0;
			}
	}
}






