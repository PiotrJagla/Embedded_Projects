#include <Wire.h>
#include "ClosedCube_HDC1080.h"

ClosedCube_HDC1080 hdc1080;


void setup()
{
	Serial.begin(115200);
	hdc1080.begin(0x40);
}

void loop()
{
  float temp = hdc1080.readTemperature();
  uint8_t temp_before_decimal_dot = (uint8_t)temp;
  uint8_t temp_after_decimal_dot = (uint8_t)((temp - temp_before_decimal_dot)*100);
  float humidity = hdc1080.readHumidity();
  uint8_t humidity_before_decimal_dot = (uint8_t)humidity;
  uint8_t humidity_after_decimal_dot = (uint8_t)((humidity - humidity_before_decimal_dot)*100);
  uint32_t whole_data = (temp_before_decimal_dot << 24) + (temp_after_decimal_dot << 16) + (humidity_before_decimal_dot << 8) + humidity_after_decimal_dot;

	//Serial.write('T');
	Serial.write(temp_before_decimal_dot);
  Serial.write(temp_after_decimal_dot);
	//Serial.write('H');
	Serial.write(humidity_before_decimal_dot);
	Serial.write(humidity_after_decimal_dot);

  delay(10);
  
	
}

