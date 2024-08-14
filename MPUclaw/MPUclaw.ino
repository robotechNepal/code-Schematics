#include <Wire.h>
#include <Servo.h>
#include <MPU6050.h>

// Define servo pin
#define SERVO_PIN 9

// Create a servo object
Servo servo;

// Define positions for open and close
#define OPEN_POS 0
#define CLOSE_POS 90

// MPU6050 object
MPU6050 mpu;

// Variables for angle calculation
const float accAngleWeight = 0.02; // Weight for accelerometer angle
float angle = 0; // Current angle

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize I2C communication
  Wire.begin();

  // Initialize MPU6050
  mpu.initialize();

  // Attach the servo to the pin
  servo.attach(SERVO_PIN);
}

void loop() {
  // Read accelerometer data
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Calculate accelerometer angle
  float accAngle = atan2(-ax, ay) * 180.0 / PI;
  angle = angle * (1.0 - accAngleWeight) + accAngle * accAngleWeight;

  // Read gyroscope data
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);

  // Calculate pitch, yaw, and roll from gyroscope data
  float pitch = (atan2(ax, sqrt(ay * ay + az * az)) * 180.0) / PI;
  float roll = (atan2(ay, sqrt(ax * ax + az * az)) * 180.0) / PI;
  float yaw = (atan2(sqrt(ax * ax + ay * ay), az) * 180.0) / PI;

  // Map angle to servo position
  int servoPosition = map(angle, -90, 90, OPEN_POS, CLOSE_POS);

  // Constrain servo position to avoid issues
  servoPosition = constrain(servoPosition, OPEN_POS, CLOSE_POS);

  // Control the servo
  servo.write(servoPosition);

  // Print values to serial monitor
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print(" | Roll: ");
  Serial.print(roll);
  Serial.print(" | Yaw: ");
  Serial.println(yaw);

  // Add a small delay to stabilize readings
  delay(10);
}
