/*
 * snakeGame.h
 *
 *  Created on: Dec 27, 2023
 *      Author: root
 */

#ifndef INC_SNAKEGAME_H_
#define INC_SNAKEGAME_H_

#include <stdint.h>

#define MATRIX_SIZE 8

struct player{
	uint8_t xPos;
	uint8_t yPos;
	uint8_t xDir;
	uint8_t yDir;
};

extern struct player snake;
extern uint8_t gameMatrix[];
extern uint8_t isInGame;

void updateSnakeGame();
void initSnakeGame();
void updateDirection(uint8_t yJoystickVal, uint8_t xJoystickVal);



#endif /* INC_SNAKEGAME_H_ */
