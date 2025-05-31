#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_SEPARATOR_POS 16

// SPI pins (hardware SPI used internally)
#define OLED_MOSI   11
#define OLED_CLK    13
#define OLED_DC     9
#define OLED_CS     10
#define OLED_RESET  8

// Create display object for SPI
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         &SPI, OLED_DC, OLED_RESET, OLED_CS);

void setup() {
  // Initialize the display
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, SCREEN_SEPARATOR_POS);           // Start at top-left
  display.println(F("Hello world!"));
  display.display();                  // Show it on the screen
}

void loop() {
  // Nothing here
}
