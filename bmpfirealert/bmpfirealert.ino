#include <Wire.h>
#include <Adafruit_BMP085.h>

#define BUZZER_PIN 3
#define LED_PIN_1 11
#define LED_PIN_2 9
#define LED_PIN_3 7

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);

  if (!bmp.begin()) {
    Serial.println("Could not find BMP180 or BMP085 sensor!");
    while (1) {}
  }
}

void loop() {
  // Read temperature from BMP180
  float temperature = bmp.readTemperature();

  // Check if temperature crosses threshold
  if (temperature > 30.0) {
    // If temperature is higher than 30°C, trigger alert
    triggerAlert();
  } else {
    // If temperature is normal, turn off alert
    turnOffAlert();
  }

  delay(1000); // Delay between temperature readings
}

void triggerAlert() {
  digitalWrite(BUZZER_PIN, HIGH); // Turn on buzzer
  digitalWrite(LED_PIN_1, HIGH);  // Turn on LEDs
  digitalWrite(LED_PIN_2, HIGH);
  digitalWrite(LED_PIN_3, HIGH);
}

void turnOffAlert() {
  digitalWrite(BUZZER_PIN, LOW); // Turn off buzzer
  digitalWrite(LED_PIN_1, LOW);  // Turn off LEDs
  digitalWrite(LED_PIN_2, LOW);
  digitalWrite(LED_PIN_3, LOW);
}
