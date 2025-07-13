#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED screen size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// SPI Pins for SSD1306
#define OLED_MOSI   19
#define OLED_CLK    18
#define OLED_DC     17
#define OLED_CS     5
#define OLED_RESET  16

// Create display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         &SPI, OLED_DC, OLED_RESET, OLED_CS);

void setup() {
  Serial.begin(115200);

  // Start SPI bus (if not started automatically)
  SPI.begin(OLED_CLK, -1, OLED_MOSI, OLED_CS);

  // Initialize display
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true); // Stop
  } else {
    Serial.println(F("SSD1306 allocation successful"));
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Hello from ESP32!");
  display.println("SSD1306 SPI Demo");
  display.display();
  delay(2000);

  display.clearDisplay();
}

void loop() {
  // Scroll some text
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("Scrolling...");
  display.display();

  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(3000);
  display.stopscroll();
  delay(1000);
}
