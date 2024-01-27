#include "timeFunctions.h"
#include <Arduino.h>

unsigned long midnightMilliseconds; // Milliseconds at last midnight

// Function to convert HH:MM to milliseconds after midnight
unsigned long timeToMilliseconds(int hour, int minute)
{
    return (hour * 3600000UL) + (minute * 60000UL);
}

// Function to update the current time
void setCurrentTime(int hour, int minute)
{
    midnightMilliseconds = millis() - timeToMilliseconds(hour, minute);
}

// Function to get the current time in milliseconds since midnight
unsigned long getCurrentTimeOfDayMillis()
{
    return millis() - midnightMilliseconds;
}

// Function to check and reset time at midnight
void checkAndResetAtMidnight(TaskResetFunc taskResetFunc)
{
    unsigned long elapsed = millis() - midnightMilliseconds;
    if (elapsed >= 86400000L)
    { // 24 hours in milliseconds
        midnightMilliseconds += 86400000UL;

        // Reset task execution status for the new day
        taskResetFunc();
    }
}

String millisecondsToHHMMSS(unsigned long milliseconds)
{
    int hours = milliseconds / 3600000;
    int minutes = (milliseconds / 60000) % 60;
    int seconds = (milliseconds / 1000) % 60;

    return String(hours) + ":" + String(minutes) + ":" + String(seconds);
}

void initTimeFunctions()
{
    // Assume time starts at midnight
    midnightMilliseconds = millis();
}