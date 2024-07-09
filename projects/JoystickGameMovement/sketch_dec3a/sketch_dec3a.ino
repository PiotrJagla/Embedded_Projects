const int SW_pin = 2;
const int X_pin = 0;
const int Y_pin = 1;

void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(115200);  
}

void loop() {
  Serial.print("switch: ");
  Serial.println(digitalRead(SW_pin));

  Serial.print("X-axis: ");
  Serial.println(analogRead(X_pin));

  Serial.print("Y-axis: ");
  Serial.println(analogRead(Y_pin));

  delay(100);
}
