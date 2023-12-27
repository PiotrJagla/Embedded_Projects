/*
 * dot_matrix_driver.c
 *
 *  Created on: Dec 27, 2023
 *      Author: root
 */
#include "dot_matrix_driver.h"
#include "main.h";



void writeC8(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, pinState);
}
void writeC7(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, pinState);
}
void writeR2(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, pinState);
}
void writeC1(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, pinState);
}
void writeR4(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, pinState);
}
void writeC6(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, pinState);
}
void writeC4(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, pinState);
}
void writeR1(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, pinState);
}
void writeR5(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, pinState);
}
void writeR7(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, pinState);
}
void writeC2(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, pinState);
}
void writeC3(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, pinState);
}
void writeR8(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, pinState);
}
void writeC5(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, pinState);
}
void writeR6(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, pinState);
}
void writeR3(GPIO_PinState pinState) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, pinState);
}


void writeCol(uint8_t col){
	writeC1(GPIO_PIN_RESET);
	writeC2(GPIO_PIN_RESET);
	writeC3(GPIO_PIN_RESET);
	writeC4(GPIO_PIN_RESET);
	writeC5(GPIO_PIN_RESET);
	writeC6(GPIO_PIN_RESET);
	writeC7(GPIO_PIN_RESET);
	writeC8(GPIO_PIN_RESET);
	switch(col) {
	case 0:
		writeC1(GPIO_PIN_SET);
		break;
	case 1:
		writeC2(GPIO_PIN_SET);
		break;
	case 2:
		writeC3(GPIO_PIN_SET);
		break;
	case 3:
		writeC4(GPIO_PIN_SET);
		break;
	case 4:
		writeC5(GPIO_PIN_SET);
		break;
	case 5:
		writeC6(GPIO_PIN_SET);
		break;
	case 6:
		writeC7(GPIO_PIN_SET);
		break;
	case 7:
		writeC8(GPIO_PIN_SET);
		break;
	}

}

void writeColDiodes(uint8_t col, uint8_t diodes) {
	writeCol(col);
	writeR1(GPIO_PIN_SET);
	writeR2(GPIO_PIN_SET);
	writeR3(GPIO_PIN_SET);
	writeR4(GPIO_PIN_SET);
	writeR5(GPIO_PIN_SET);
	writeR6(GPIO_PIN_SET);
	writeR7(GPIO_PIN_SET);
	writeR8(GPIO_PIN_SET);
	for(uint8_t i = 0 ; i < 8 ; ++i) {
		if(diodes & (1<<i)) {
			switch(i) {
			case 0:
				writeR1(GPIO_PIN_RESET);
				break;
			case 1:
				writeR2(GPIO_PIN_RESET);
				break;
			case 2:
				writeR3(GPIO_PIN_RESET);
				break;
			case 3:
				writeR4(GPIO_PIN_RESET);
				break;
			case 4:
				writeR5(GPIO_PIN_RESET);
				break;
			case 5:
				writeR6(GPIO_PIN_RESET);
				break;
			case 6:
				writeR7(GPIO_PIN_RESET);
				break;
			case 7:
				writeR8(GPIO_PIN_RESET);
				break;
			}

		}
	}

}






