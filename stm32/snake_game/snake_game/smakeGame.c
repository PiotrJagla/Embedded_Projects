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
		{2,0,0,0,0,0,0,0,0,0,0},
		{4,0,0,0,0,0,0,0,0,0,0},
		1,0};
uint8_t isInGame = 0;

void pushRandomDot() {
	uint8_t randVal = HAL_GetTick()%64;
	uint8_t randCol = randVal/MATRIX_SIZE;
	uint8_t randRow = randVal%MATRIX_SIZE;
	gameMatrix[randCol] |= (1<<randRow);

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

	isInGame = 1;
	pushRandomDot();
	gameMatrix[snake.yPositions[0]] |= (1<<snake.xPositions[0]);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); //R
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);   //G
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); //B
}

void updateSnakeGame(){
	if(isInGame == 1) {
		//Remove previous position
		gameMatrix[snake.yPositions[0]] &= ~(1<<snake.xPositions[0]);

		//Update pos
		snake.xPositions[0] += snake.xDir;
		snake.yPositions[0] += snake.yDir;

		//Is out of bounds
		if(snake.xPositions[0] >= MATRIX_SIZE) {
			isInGame = 0;
		} else if(snake.xPositions[0] < 0) {
			isInGame = 0;
		}
		if(snake.yPositions[0] >= MATRIX_SIZE) {
			isInGame = 0;
		} else if(snake.yPositions[0] < 0) {
			isInGame = 0;
		}
		if(isInGame == 0) {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); //R
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);   //G
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); //B
		}

		//dot collected
		if(gameMatrix[snake.yPositions[0]] & (1<<snake.xPositions[0])) {
			pushRandomDot();

			//blink when dot is collected
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); //R
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);   //G
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); //B
			osDelay(50);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); //R
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);   //G
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); //B
		}


		//Draw new position
		gameMatrix[snake.yPositions[0]] |= (1<<snake.xPositions[0]);
	}
}

void updateDirection(uint8_t yJoystickVal, uint8_t xJoystickVal) {
	if(xJoystickVal < 10) {
		snake.yDir = 1;
		snake.xDir = 0;
	}
	else if(xJoystickVal > 240) {
		snake.yDir = -1;
		snake.xDir = 0;
	}

	if(yJoystickVal < 10) {
		snake.xDir = -1;
		snake.yDir = 0;
	}
	else if(yJoystickVal > 240) {
		snake.xDir = 1;
		snake.yDir = 0;
	}

}






