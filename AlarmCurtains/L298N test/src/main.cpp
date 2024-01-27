#include <Arduino.h>

// Define the L298N motor control pins
const int motorPin1 = 7; // Input 1
const int motorPin2 = 8; // Input 2
const int enablePin = 9; // Enable Pin (PWM Control)


void motorForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, 255); // Full speed
}

void motorReverse() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(enablePin, 255); // Full speed
}

void motorStop() {
  digitalWrite(enablePin, LOW); // Disable motor
}

void setup() {
  // Initialize the motor control pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
}

void loop() {
  // Example usage
  motorForward();
  delay(2000);
  motorStop();
  delay(1000);
  motorReverse();
  delay(2000);
  motorStop();
  delay(1000);
}
