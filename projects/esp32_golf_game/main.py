from machine import SPI, SoftSPI, Pin, Timer
import time
import ssd1306
import random

from usr.util import *

SCREEN_THRESHOLD = 16
SCREEN_HEIGHT = 64
SCREEN_WIDTH = 128

#Controls
left_button = Pin(25, Pin.IN, Pin.PULL_DOWN)
right_button = Pin(26, Pin.IN, Pin.PULL_DOWN)
led_pin = Pin(2, Pin.OUT)

#Init display
spi = SPI(1, baudrate=500000, polarity=1, phase=0, sck=Pin(18), mosi=Pin(23), miso=Pin(19))
cs = Pin(5, Pin.OUT)
dc = Pin(17, Pin.OUT)
rst = Pin(16, Pin.OUT)
display = ssd1306.SSD1306_SPI(128, 64, spi, dc, rst, cs)

#Game
did_lost = False
points = 0
player_size = Size(width=10, height=3)
player_pos = Point(40, SCREEN_HEIGHT-player_size.height)

enemies_pos = []
enemy_size = Size(width=3, height=3)


tim = Timer(1)
tim.init(mode=Timer.PERIODIC, period=3000, 
           callback=lambda t : enemies_pos.append(Point(x=random.randint(0, 100), y=SCREEN_THRESHOLD)))

def check_collision(rect1, rect2):
    x1, y1, w1, h1 = rect1.pos.x, rect1.pos.y, rect1.size.width, rect1.size.height
    x2, y2, w2, h2 = rect2.pos.x, rect2.pos.y, rect2.size.width, rect2.size.height

    if x1 + w1 <= x2 or x1 >= x2 + w2:
        return False
    if y1 + h1 <= y2 or y1 >= y2 + h2:
        return False

    return True

def update():
    global did_lost

    if did_lost:
        return

    #Update position
    if left_button.value():
        player_pos.x -= 0.5
    elif right_button.value():
        player_pos.x += 0.5

    #Update comets
    for enemy in enemies_pos:
        enemy.y += 0.1

    #Update collision
    for enemy in enemies_pos.copy():
        if check_collision(Rect(enemy, enemy_size), Rect(player_pos, player_size)):
            global points
            points += 1
            enemies_pos.remove(enemy)

        if enemy.y + enemy_size.height >= SCREEN_HEIGHT:
            did_lost = True






def render():
    global did_lost

    display.fill(0)
    rectangle(display, player_pos, player_size)

    for enemy in enemies_pos:
        rectangle(display, enemy, enemy_size)

    if did_lost:
        display.text("YOU LOST, HAHAHA", 0, 30, 1)
        tim.deinit()

    display.text(f"Points: {points}", 0,0,1)
    display.show()

while True:
    update()
    render()

