import math
import random
import platform

import pygame
import serial.tools.list_ports

ports = serial.tools.list_ports.comports()
serialMonitor = serial.Serial()
portList = []

for onePort in ports:
    portList.append(str(onePort))
    print(str(onePort))


os_name = platform.system()
if os_name == "Windows":
    selectedPortNum = input("select Port: COM")
    
    for i in range(0,len(portList)):
        if portList[i].startswith("COM" + str(selectedPortNum)):
            selectedPort = "COM" + str(selectedPortNum) 
            print(selectedPort)
elif os_name == "Linux":
    selectedPortNum = input("select Port: /dev/ttyACM")
    
    for i in range(0,len(portList)):
        if portList[i].startswith("/dev/ttyACM" + str(selectedPortNum)):
            selectedPort = "/dev/ttyACM" + str(selectedPortNum) 
            print(selectedPort)






serialMonitor.baudrate = 115200
serialMonitor.port = selectedPort
serialMonitor.open()


WIDTH, HEIGHT = 900, 500
WINDOW = pygame.display.set_mode((WIDTH, HEIGHT))
SPEED = 1
pygame.display.set_caption("joystick game")

WHITE = (255,255,255)
BLACK = (0,0,0)
FPS = 60
rect = pygame.Rect(50,50,200,100)

def draw_window(rectColor):
    WINDOW.fill(BLACK)
    pygame.draw.rect(WINDOW, rectColor, rect)
    pygame.display.update()



def main():

    run = True
    clock = pygame.time.Clock()
    xComponent = 0.0
    yComponent = 0.0
    xNormalized = 0.0
    yNormalized = 0.0
    switchValue = False
    lock = True
    rectColor = (255,255,255)
    while run:
        if switchValue and lock:
            print("switch clicked")
            rectColor = (random.randint(0,255), random.randint(0,255), random.randint(0,255))
            lock = False
        if not switchValue:
            lock = True


        clock.tick(FPS)

        #check events
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False

        #read from serial monitor
        if serialMonitor.in_waiting:
            readData = serialMonitor.readline()
<<<<<<< HEAD
            try:
                movementData = readData.decode("UTF-8").rstrip('\n').split(":")
            except Exception as e:
                print(e)
            component = movementData[0]
            value = movementData[1]
=======
            movementData = readData.decode("UTF-8").rstrip('\n').split(":")

            try:
                component = movementData[0]
                value = movementData[1]
            except Exception as e:
                print(movementData)

>>>>>>> 0fe9dc8c60b26b0a1b538edef99537fbf6647bff
            if component.startswith("switch"):
                if int(value) == 0:
                    switchValue = True
                elif int(value) == 1:
                    switchValue = False
                
            elif component.startswith("X-axis"):
                xComponent = float(value)
            elif component.startswith("Y-axis"):
                yComponent = float(value)



            xNormalized = xComponent - 500.0
            yNormalized = yComponent - 500.0
            if xNormalized >= -50 and xNormalized <= 50:
                xNormalized = 0
            if yNormalized >= -50 and yNormalized <= 50:
                yNormalized = 0

            if xNormalized != 0 or yNormalized != 0:
                vectorLength = math.sqrt(xNormalized*xNormalized + yNormalized*yNormalized)
                xNormalized /= vectorLength
                yNormalized /= vectorLength


        rect.x += xNormalized*SPEED
        rect.y += yNormalized*SPEED

        #detect collision with window
        if rect.x <= 0:
            rect.x = 0
        elif rect.x + rect.width >= WIDTH:
            rect.x = WIDTH - rect.width

        if rect.y <= 0:
            rect.y = 0
        elif rect.y + rect.height >= HEIGHT:
            rect.y = HEIGHT - rect.height





        draw_window(rectColor)

    pygame.quit()


if __name__ == "__main__":
    main()
