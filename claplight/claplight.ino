const int microphonePin = A0; /
// microphone chai analog pin A0 ma halna paryo ani D0 chai unused hunxa 
const int ledPin = 13; // led ko lagi pin 13 use garum hai       
void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600); 
}
void loop() { 
  int sensorValue = analogRead(microphonePin);
  Serial.println(sensorValue);
  float voltage = sensorValue * (5.0 / 1023.0);
// yaa maathi chai sensor ko analog value lai voltage ma convert haneko 
  Serial.println(voltage); 

  if (voltage > 0.45) { //yeta 0.45 value lai milauna parxa if needed.
    digitalWrite(ledPin, HIGH); 
// Aba yaa condition haleko xa ki yedi A0 pin le 0.45 vanda 
//maathi ko voltage read garyo vane batti balxa. 
    Serial.println("Sound detected!");
      delay(1000); 

  } else {
    digitalWrite(ledPin, LOW); 

  delay(100); 
}
}

