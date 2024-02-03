#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <Arduino.h>

// Function declarations
void motorForward();
void motorReverse();
void motorStop();
void setupMotorPins();

const int motorPin1 = A4; // Input 1
const int motorPin2 = A3; // Input 2
const int enablePin = A2; // Enable Pin (PWM Control)

extern volatile bool motorIsStopped;
#endif // MOTORCONTROL_H
