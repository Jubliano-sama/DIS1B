#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <Arduino.h>

// Function declarations
void motorForward();
void motorReverse();
void motorStop();
void setupMotorPins();

// Define the L298N motor control pins
const int motorPin1 = 7; // Input 1
const int motorPin2 = 8; // Input 2
const int enablePin = 9; // Enable Pin (PWM Control)

extern bool motorIsStopped;
#endif // MOTORCONTROL_H
