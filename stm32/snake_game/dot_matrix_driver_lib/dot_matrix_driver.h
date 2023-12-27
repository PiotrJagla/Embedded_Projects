/*
 * dot_matrix_driver.h
 *
 *  Created on: Dec 27, 2023
 *      Author: root
 */

#ifndef INC_DOT_MATRIX_DRIVER_H_
#define INC_DOT_MATRIX_DRIVER_H_
#include <stdint.h>
#include "gpio.h"

void writeCol(uint8_t col);
void writeColDiodes(uint8_t col, uint8_t diodes);

#endif /* INC_DOT_MATRIX_DRIVER_H_ */
