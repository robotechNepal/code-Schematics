// Define pins for ultrasonic sensor
const int trigPin = 6; // Trig pin of the sensor
const int echoPin = 7; // Echo pin of the sensor
const int buzzerPin = 5; // Buzzer pin

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600); // Initialize serial communication for debugging
}

void loop() {
  long duration;
  int distance;
  
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Send a pulse to trigger the sensor
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the echo duration
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance in cm
  distance = duration * 0.034 / 2; // Speed of sound is 34,000 cm/s
  
  // Print distance for debugging
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Check if an object is within 10cm range
  if (distance <= 10 && distance > 0) {
    // Object detected within 10cm, turn on the buzzer
    digitalWrite(buzzerPin, HIGH);
  } else {
    // No object detected within 10cm, turn off the buzzer
    digitalWrite(buzzerPin, LOW);
  }
  
  delay(100); // Wait for a short duration before next reading
}
