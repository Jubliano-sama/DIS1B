#include <SoftwareSerial.h>
#include <Arduino.h>

// Define the type for the read data function pointer
typedef String (*ReadDataFunc)();
typedef void (*TaskFunc)();
typedef void (*TaskResetFunc)(void);

// Global variables
SoftwareSerial BluetoothSerial(10, 11); // RX | TX
unsigned long midnightMilliseconds; // Milliseconds at last midnight

// Task array to store task functions and their execution status
struct Task {
  unsigned long millisAfterMidnight;
  TaskFunc function;
  bool executed;
};

// Array of tasks to be executed
const int amountOfTasks = 10;
Task tasks[amountOfTasks];

// Function to reset all tasks
void resetAllTasks() {
  for (int i = 0; i < amountOfTasks; i++) {
    Task& task = tasks[i];
    task.executed = false;
  }
}

// Function to convert HH:MM to milliseconds after midnight
unsigned long timeToMilliseconds(int hour, int minute) {
  return (hour * 3600000L) + (minute * 60000L);
}

// Function to update the current time
void setCurrentTime(int hour, int minute) {
  midnightMilliseconds = millis() - timeToMilliseconds(hour,minute);
}

// Function to get the current time in milliseconds since midnight
unsigned long getCurrentTimeOfDayMillis() {
  return millis() - midnightMilliseconds;
}

// Function to check and reset time at midnight
void checkAndResetAtMidnight(TaskResetFunc taskResetFunc) {
  unsigned long elapsed = millis() - midnightMilliseconds;
  if (elapsed >= 86400000L) { // 24 hours in milliseconds
    midnightMilliseconds += 86400000L;

    // Reset task execution status for the new day
    taskResetFunc();
  }
}

void executeTasks(Task* tasks, int amountOfTasks) {
  for (int i = 0; i < amountOfTasks; i++) {
    Task& task = tasks[i];
    unsigned long currentTime = getCurrentTimeOfDayMillis();
    if (currentTime >= task.millisAfterMidnight && !task.executed) {
      task.function();
      task.executed = true;
    }
  }
}

// Function to read data from Bluetooth
String readBluetoothData() {
  if (BluetoothSerial.available()) {
    return BluetoothSerial.readStringUntil('\n');
  }
  return "";
}

// Function to handle Bluetooth data
void handleBluetoothData(ReadDataFunc readData) {
  String data = readData();
  
  if (data.length() > 0) {
    if (data.startsWith("current:")) {
      int hour = data.substring(data.indexOf(':') + 2, data.indexOf(':') + 4).toInt();
      int minute = data.substring(data.lastIndexOf(':') + 1).toInt();
      setCurrentTime(hour, minute);
      Serial.print("Current time set to: ");
      Serial.print(hour);
      Serial.print(":");
      Serial.println(minute);
    } else if (data == "up" || data == "1") {
      Serial.println("Received 'up' command");
    } else if (data == "down" || data == "0") {
      Serial.println("Received 'down' command");
    } else {
      // Assuming the rest of the data is time in HH:MM format
      int hour = data.substring(0, data.indexOf(':')).toInt();
      int minute = data.substring(data.indexOf(':') + 1).toInt();
      Serial.print("Received Time: ");
      Serial.print(hour);
      Serial.print(":");
      Serial.println(minute);
    }
  }
}

void setup() {
  Serial.begin(9600);
  BluetoothSerial.begin(9600);
  Serial.println("Bluetooth device is ready to pair");
  midnightMilliseconds = millis(); // Assuming the Arduino starts at midnight
}

void loop() {
  handleBluetoothData(readBluetoothData);
  // Additional code for loop can be added here
}