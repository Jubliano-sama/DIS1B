#include "MotorControl.h"

volatile bool motorIsStopped = true;

void motorForward()
{
    motorIsStopped = false;
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    analogWrite(enablePin, 255); // Full speed
}

void motorReverse()
{
    motorIsStopped = false;
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    analogWrite(enablePin, 255); // Full speed
}

void motorStop()
{
    digitalWrite(enablePin, LOW); // Disable motor
    motorIsStopped = true;
}

void setupMotorPins()
{
    // Initialize the motor control pins as outputs
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(enablePin, OUTPUT);
}
