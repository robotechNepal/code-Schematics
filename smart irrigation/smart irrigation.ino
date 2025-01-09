// Pin definitions
const int soilMoisturePin = A0;  // Soil moisture sensor analog pin
const int relayPin = 8;          // Relay pin

// Threshold for soil moisture
const int threshold = 700;

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);
  
  // Set relay pin as OUTPUT
  pinMode(relayPin, OUTPUT);
  
  // Turn relay off initially
  digitalWrite(relayPin, LOW);
}

void loop() {
  // Read the soil moisture sensor value
  int sensorValue = analogRead(soilMoisturePin);
  
  // Print the value to the Serial Monitor
  Serial.print("Soil moisture: ");
  Serial.println(sensorValue);

  // Check if the sensor value is below the threshold
  if (sensorValue < threshold) {
    // Turn relay on (activate the pump)
    digitalWrite(relayPin, HIGH);
  } else {
    // Turn relay off
    digitalWrite(relayPin, LOW);
  }
  
  // Wait for a short time before reading again
  delay(1000);
}
