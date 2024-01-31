# EmbeddedDemoProjects
Here i have some fun Embedded projects:

# Snake
I used dot-matrix and joystick to create a snake game with stm32-F302R8 <br>
I can also control the snake speed with potentiometer <br>
When game is started the LED to the right lights green, but when i lose LED turns red. There is also short blink of blue light when i collect a dot <br>
Scored points are counted with 2 7-seg displays <br>
I used freeRTOS is this project to properly multiplex 7-seg displays and dot-matrix <br>
<img src="https://github.com/PiotrJagla/EmbeddedDemoProjects/assets/76881722/3f638a0d-2471-46c0-952e-a13e278808d8" width="400" height="400" />
![414647398_643927464406667_7967834541922400208_n](https://github.com/PiotrJagla/EmbeddedDemoProjects/assets/76881722/3f638a0d-2471-46c0-952e-a13e278808d8)


# Robotic arm with gripper
Robotic arm made witm stm32-F302R8. <br>
I can control the arm with joystick and the gripper with two button in the background.<br>
This arm consists of one stepper motor to turn around, one servo motor to turn up and down and another servo for gripper.<br>
![421289240_1775808462916178_1839222255133160598_n](https://github.com/PiotrJagla/EmbeddedDemoProjects/assets/76881722/7306012e-2b5f-4fa7-ba2d-20f658d4e757)

# Gyroscope measurement virualization
Gyroscope measurement visualization with MPU6050 inertial measurement unit and atmega328p
Cube on the screen moves as i move the sensor
![419163211_1114252446422169_2898686861297516872_n](https://github.com/PiotrJagla/EmbeddedDemoProjects/assets/76881722/d194f7e9-7e98-423c-b847-813f9e2887f7)
![415319191_256152287361427_4004618289510443761_n](https://github.com/PiotrJagla/EmbeddedDemoProjects/assets/76881722/11a14a5f-d969-4506-8920-4983f89d0e7e)

# Temperature and humidity
I used a HDC1080 high pricision humidity and temperature sensor to get data to ATmega328p.
Then Atmega328p sens data via UART to STM32 nucleo-F302R8.
This data is displayed in 4 7seg displays. 
I can switch from temperature to humidity with button click.
![411592469_1053614085886085_698731388967239220_n](https://github.com/PiotrJagla/EmbeddedDemoProjects/assets/76881722/db82a2d0-b618-4fd6-8fe6-a171b3f60295)
![411778409_877724747387040_620683428153191571_n](https://github.com/PiotrJagla/EmbeddedDemoProjects/assets/76881722/a2e06ba5-4eb9-401e-926c-5f8130da90a0)


