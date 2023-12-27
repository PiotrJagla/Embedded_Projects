/*
 * smakeGame.c
 *
 *  Created on: Dec 27, 2023
 *      Author: root
 */
#include "snakeGame.h"
#include "gpio.h"
#include "FreeRTOS.h"

uint8_t gameMatrix[8] = {0,32,0,0,0,0,0,0};
struct player snake = {2,4,1,0};
uint8_t isInGame = 0;

void pushRandomDot() {
	uint8_t randVal = HAL_GetTick()%64;
	uint8_t randCol = randVal/MATRIX_SIZE;
	uint8_t randRow = randVal%MATRIX_SIZE;
	gameMatrix[randCol] |= (1<<randRow);

}


void initSnakeGame(){
	for(uint8_t i = 0 ; i < MATRIX_SIZE ; ++i) {
		gameMatrix[i] = 0;
	}
	pushRandomDot();
	snake.xPos = 2;
	snake.yPos = 4;
	snake.xDir = 1;
	snake.yDir = 0;
	gameMatrix[snake.yPos] |= (1<<snake.xPos);
}

void updateSnakeGame(){
	if(isInGame == 1) {
		//Remove previous position
		gameMatrix[snake.yPos] &= ~(1<<snake.xPos);

		//Update pos
		snake.xPos += snake.xDir;
		snake.yPos += snake.yDir;

		//Is out of bounds
		if(snake.xPos >= MATRIX_SIZE) {
			isInGame = 0;
		} else if(snake.xPos < 0) {
			isInGame = 0;
		}
		if(snake.yPos >= MATRIX_SIZE) {
			isInGame = 0;
		} else if(snake.yPos < 0) {
			isInGame = 0;
		}
		if(isInGame == 0) {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); //R
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);   //G
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); //B
		}

		//dot collected
		if(gameMatrix[snake.yPos] & (1<<snake.xPos)) {
			pushRandomDot();
		}

		//Draw new position
		gameMatrix[snake.yPos] |= (1<<snake.xPos);
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






