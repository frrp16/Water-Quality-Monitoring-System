int sensorPin = 36; // A0 for Arduino / 36 for ESP32

void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  int turbidity = map(sensorValue, 0, 750, 100, 0);
  delay(100);

  Serial.print("Turbidity: ");
  Serial.println(turbidity);

  if (turbidity < 20) {
    Serial.println("Status: Clear");
  } else if (turbidity >= 20 && turbidity < 50) {
    Serial.println("Status: Cloudy");
  } else {
    Serial.println("Status: Dirty");
  }
  
  delay(1000); // Adjust delay as needed
}
