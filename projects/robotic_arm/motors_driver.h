/*
 * motors_driver.h
 *
 *  Created on: Jan 29, 2024
 *      Author: root
 */

#ifndef INC_MOTORS_DRIVER_H_
#define INC_MOTORS_DRIVER_H_

#include "FreeRTOS.h"
#include "gpio.h"

#define STEP_MOTOR1_BASE ((GPIO_TypeDef *) GPIOC)
#define STEP_MOTOR1_PIN  ((uint16_t) GPIO_PIN_4)

#define STEP_MOTOR2_BASE ((GPIO_TypeDef *) GPIOA)
#define STEP_MOTOR2_PIN  ((uint16_t) GPIO_PIN_5)

#define STEP_MOTOR3_BASE ((GPIO_TypeDef *) GPIOA)
#define STEP_MOTOR3_PIN  ((uint16_t) GPIO_PIN_6)

#define STEP_MOTOR4_BASE ((GPIO_TypeDef *) GPIOA)
#define STEP_MOTOR4_PIN  ((uint16_t) GPIO_PIN_7)

#define SERVO_MOTOR_PWM_BASE ((GPIO_TypeDef *) GPIOB)
#define SERVO_MOTOR_PWN_PIN  ((uint16_t) GPIO_PIN_1)



enum StepperMotorState {
	TURNING_RIGHT,
	TURNING_LEFT,
	STOPPED
};

void startTurningRight();
void startTurningLeft();
void disableStepperMotor();

#endif /* INC_MOTORS_DRIVER_H_ */
