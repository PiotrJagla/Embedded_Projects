#include <Arduino.h>

const int ledPin = 32;  // Built-in LED pin for many ESP32 boards

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Starting LED blink");
}

void loop() {
  digitalWrite(ledPin, HIGH);   // Turn the LED on
  delay(500);                   // Wait 500ms
  digitalWrite(ledPin, LOW);    // Turn the LED off
  delay(500);                   // Wait 500ms
}
