#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

#define HDC1080_ADDR 0x40
#define TEMP_REG     0x00
#define HUMID_REG    0x01

const char* ssid = "ssid";
const char* password = "password";

const char* mqtt_server = "192.168.1.42";
const char* mqtt_user = "mqtt_user";
const char* mqtt_pass = "mqtt_password";

const char* temp_topic = "home/esp32/hdc1080/temperature";
const char* hum_topic  = "home/esp32/hdc1080/humidity";
const char* temp_config_topic = "homeassistant/sensor/esp32_temp/config";
const char* hum_config_topic  = "homeassistant/sensor/esp32_hum/config";

WiFiClient espClient;
PubSubClient client(espClient);

void publishDiscoveryConfig() {
  String tempConfig = 
    "{\"name\": \"ESP32 Temperature\","
    "\"state_topic\": \"" + String(temp_topic) + "\","
    "\"unit_of_measurement\": \"°C\","
    "\"device_class\": \"temperature\","
    "\"value_template\": \"{{ value }}\","
    "\"unique_id\": \"esp32_temp\"}";

  String humConfig = 
    "{\"name\": \"ESP32 Humidity\","
    "\"state_topic\": \"" + String(hum_topic) + "\","
    "\"unit_of_measurement\": \"%\","
    "\"device_class\": \"humidity\","
    "\"value_template\": \"{{ value }}\","
    "\"unique_id\": \"esp32_hum\"}";

  client.publish(temp_config_topic, tempConfig.c_str(), true);
  client.publish(hum_config_topic, humConfig.c_str(), true);
}

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
      // publishDiscoveryConfig();
    } else {
      delay(5000);
    }
  }
}

float readTemperature() {
  Wire.beginTransmission(HDC1080_ADDR);
  Wire.write(TEMP_REG);
  Wire.endTransmission();
  delay(20);  // Wait for measurement

  Wire.requestFrom(HDC1080_ADDR, 2);
  uint16_t raw = (Wire.read() << 8) | Wire.read();
  return (raw / 65536.0) * 165.0 - 40.0;
}

float readHumidity() {
  Wire.beginTransmission(HDC1080_ADDR);
  Wire.write(HUMID_REG);
  Wire.endTransmission();
  delay(20);

  Wire.requestFrom(HDC1080_ADDR, 2);
  uint16_t raw = (Wire.read() << 8) | Wire.read();
  return (raw / 65536.0) * 100.0;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); 
  setup_wifi();

  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  float temperature = readTemperature();
  float humidity = readHumidity();

  char tempStr[10], humStr[10];
  dtostrf(temperature, 4, 2, tempStr);
  dtostrf(humidity, 4, 2, humStr);

  client.publish(temp_topic, tempStr, true);
  client.publish(hum_topic, humStr, true);

  delay(30000);
}
