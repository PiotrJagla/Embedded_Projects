# EmbeddedDemoProjects
Here i have some fun Embedded projects:

# Snake
I used dot-matrix and joystick to create a snake game with stm32-F302R8 <br>
I can control the snake speed with potentiometer <br>
When game starts the LED to the right lights green, but when i lose LED turns red. There is also short blink of blue light when i collect a dot <br>
Scored points are counted with 2 7-seg displays <br>
I used freeRTOS is this project to properly multiplex 7-seg displays and dot-matrix <br>
<img src="https://github.com/PiotrJagla/EmbeddedDemoProjects/assets/76881722/3f638a0d-2471-46c0-952e-a13e278808d8" width="500" height="600" />



# Robotic arm with gripper
Robotic arm made witm stm32-F302R8. <br>
I can control the arm with joystick and the gripper with two button in the background.<br>
This arm consists of one stepper motor to turn around, one servo motor to turn up and down and another servo for gripper.<br>
<img src="https://github.com/PiotrJagla/EmbeddedDemoProjects/assets/76881722/7306012e-2b5f-4fa7-ba2d-20f658d4e757" width="500" height="600" />

# Gyroscope measurement virualization
Gyroscope measurement visualization with MPU6050 inertial measurement unit and atmega328p <br>
Cube on the screen moves as i move the sensor <br>
<img src="https://github.com/PiotrJagla/EmbeddedDemoProjects/assets/76881722/d194f7e9-7e98-423c-b847-813f9e2887f7" width="500" height="600" />
<img src="https://github.com/PiotrJagla/EmbeddedDemoProjects/assets/76881722/11a14a5f-d969-4506-8920-4983f89d0e7e" width="500" height="600" />

# Temperature and humidity
I used a HDC1080 high pricision humidity and temperature sensor to get data to ATmega328p. <br>
Then Atmega328p sens data via UART to STM32 nucleo-F302R8. <br>
This data is displayed in 4 7seg displays. <br>
I can switch from temperature to humidity with button click. <br>
<img src="https://github.com/PiotrJagla/EmbeddedDemoProjects/assets/76881722/db82a2d0-b618-4fd6-8fe6-a171b3f60295" width="500" height="600" />
<img src="https://github.com/PiotrJagla/EmbeddedDemoProjects/assets/76881722/a2e06ba5-4eb9-401e-926c-5f8130da90a0" width="500" height="600" />



