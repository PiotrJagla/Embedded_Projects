#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "ssid";
const char* password = "password";

const char* mqtt_server = "192.168.1.42";  // Replace with your Home Assistant IP
const char* mqtt_user = "mqtt_user";        // Optional
const char* mqtt_pass = "mqtt_password";    // Optional

WiFiClient espClient;
PubSubClient client(espClient);

const int ledPin = 2;

const char* state_topic = "home/esp32/led";
const char* command_topic = "home/esp32/led/set";
const char* discovery_topic = "homeassistant/light/esp32_led/config";

void publishDiscoveryConfig() {
  String payload = 
    "{\"name\": \"ESP32 LED\","
    "\"command_topic\": \"home/esp32/led/set\","
    "\"state_topic\": \"home/esp32/led\","
    "\"payload_on\": \"ON\","
    "\"payload_off\": \"OFF\","
    "\"unique_id\": \"esp32_led_01\","
    "\"device\": {"
      "\"identifiers\": [\"esp32_led_board\"],"
      "\"name\": \"ESP32 Board\","
      "\"model\": \"ESP32-DevKit\","
      "\"manufacturer\": \"DIY\""
    "}"
    "}";
    
  client.publish(discovery_topic, payload.c_str(), true); // retain = true
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.printf("Message arrived [%s]: %s\n", topic, message.c_str());

  if (String(topic) == command_topic) {
    if (message == "ON") {
      digitalWrite(ledPin, HIGH);
      client.publish(state_topic, "ON", true);
    } else {
      digitalWrite(ledPin, LOW);
      client.publish(state_topic, "OFF", true);
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
      Serial.println("connected");

      client.subscribe(command_topic);
      publishDiscoveryConfig();
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
