#ifndef TIMEFUNCTIONS_H
#define TIMEFUNCTIONS_H

#include <Arduino.h>

// Function to convert HH:MM to milliseconds after midnight
unsigned long timeToMilliseconds(int hour, int minute);

// Function to set the current time
void setCurrentTime(int hour, int minute);

typedef void (*TaskResetFunc)(void);

// Function to get the current time in milliseconds since midnight
unsigned long getCurrentTimeOfDayMillis();

// Function to check and reset time at midnight
void checkAndResetAtMidnight(void (*taskResetFunc)());

// Function to convert milliseconds to HH:MM:SS format
String millisecondsToHHMMSS(unsigned long milliseconds);

void initTimeFunctions();
#endif // TIMEFUNCTIONS_H
