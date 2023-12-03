import pygame
import serial.tools.list_ports

ports = serial.tools.list_ports.comports()
serialMonitor = serial.Serial()
portList = []

for onePort in ports:
    portList.append(str(onePort))
    print(str(onePort))

selectedPortNum = input("select Port: COM")

for i in range(0,len(portList)):
    if portList[i].startswith("COM" + str(selectedPortNum)):
        selectedPort = "COM" + str(selectedPortNum) 
        print(selectedPort)

serialMonitor.baudrate = 115200
serialMonitor.port = selectedPort
serialMonitor.open()


WIDTH, HEIGHT = 900, 500
WINDOW = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("joystick game")

WHITE = (255,255,255)
BLACK = (0,0,0)
FPS = 60
rect = pygame.Rect(50,50,200,100)

def draw_window():
    WINDOW.fill(BLACK)
    pygame.draw.rect(WINDOW, WHITE, rect)
    pygame.display.update()



def main():

    run = True
    clock = pygame.time.Clock()
    while run:
        clock.tick(FPS)
        #check events
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_RIGHT:
                    rect.x += 1

        #read from serial monitor
        if serialMonitor.in_waiting:
            readData = serialMonitor.readline()
            print(readData.decode("UTF-8"))

        draw_window()
        

    pygame.quit()


if __name__ == "__main__":
    main()
