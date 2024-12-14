
class Point: 
    def __init__(self, x: float, y: float):
        self.x = x
        self.y = y

class Size:
    def __init__(self, width: float, height: float):
        self.width = width
        self.height = height

class Rect:
    def __init__(self, pos, size):
        self.pos = pos
        self.size = size

def rectangle(display, point, size):
    display.rect(int(point.x), int(point.y), int(size.width), int(size.height), 1)

