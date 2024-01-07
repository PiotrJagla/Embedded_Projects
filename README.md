# EmbeddedDemoProjects
Here i have some fun Embedded projects:

# Temperature and humidity
I used a HDC1080 high pricision humidity and temperature sensor to get data to ATmega328p.
Then Atmega328p sens data via UART to STM32 nucleo-F302R8.
This data is displayed in 4 7seg displays. 
I can switch from temperature to humidity with button click.
![411592469_1053614085886085_698731388967239220_n](https://github.com/PiotrJagla/EmbeddedDemoProjects/assets/76881722/db82a2d0-b618-4fd6-8fe6-a171b3f60295)
![411778409_877724747387040_620683428153191571_n](https://github.com/PiotrJagla/EmbeddedDemoProjects/assets/76881722/a2e06ba5-4eb9-401e-926c-5f8130da90a0)

# Snake
I used dot-matrix and joystick to create a snake game with stm32-F302R8 <br>
I can also control the snake speed with potentiometer <br>
When game is started the LED to the right lights green, but when i lose LED turns red. There is also short blink of blue light when i collect a dot <br>
Scored points are counted with 2 7-seg displays <br>
I used freeRTOS is this project to properly multiplex displayed and dot-matrix <br>
![414647398_643927464406667_7967834541922400208_n](https://github.com/PiotrJagla/EmbeddedDemoProjects/assets/76881722/3f638a0d-2471-46c0-952e-a13e278808d8)
