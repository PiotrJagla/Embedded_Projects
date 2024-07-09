#include <Wire.h>

#define TM1650_ADDRESS 0x24 // Base address for the TM1650
#define TM1650_CTRL_CMD 0x48 // Control register command
#define TM1650_DISP_CMD 0x68 // Display register command (assumed for the example)

void setup() {
  Wire.begin(); // Initialize I2C as master
  Serial.begin(9600); // For debugging
  initializeDisplay();
}

void loop() {
  // Write digits to the four segments
  writeDigit(0, 0x3F); // Display 0 on the first segment
  writeDigit(1, 0x06); // Display 1 on the second segment
  writeDigit(2, 0x5B); // Display 2 on the third segment
  writeDigit(3, 0x4F); // Display 3 on the fourth segment

  delay(1000); // Wait for a second
}

void initializeDisplay() {
  Wire.beginTransmission(TM1650_ADDRESS);
  Wire.write(TM1650_CTRL_CMD); // Control command to configure the display
  Wire.write(0x01); // Example: Turn on display, 1/16 duty, no blinking
  Wire.endTransmission();
}

void writeDigit(uint8_t segment, uint8_t digit) {
  Wire.beginTransmission(TM1650_ADDRESS + segment); // Address is base address + segment number
  Wire.write(digit); // Digit to display
  Wire.endTransmission();
}