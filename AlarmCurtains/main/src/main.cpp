#include <SoftwareSerial.h>
#include <Arduino.h>
#include "timeFunctions.h"
#include "motorControl.h"

// Define the type for the read data function pointer
typedef String (*ReadDataFunc)();
typedef void (*TaskFunc)();

SoftwareSerial BluetoothSerial(10, 11); // RX | TX

// Task array to store task functions and their execution status
struct Task
{
	unsigned long millisAfterMidnight;
	TaskFunc function;
	bool executed;
};

// Function declarations
void resetAllTasks();
void executeTasks(Task *tasks, int amountOfTasks);
String readBluetoothData();
void handleBluetoothData(ReadDataFunc readData, Task *tasks);
void moveCurtainsUp();
void moveCurtainsDown();
bool readCeilingSensor();
bool readFloorSensor();
void initTimeFunctions();
void setupMotorPins();

// Array of tasks to be executed
const int amountOfTasks = 2;
Task tasks[amountOfTasks];

void setup()
{
	Serial.begin(9600);
	initTimeFunctions();
	setupMotorPins();
	BluetoothSerial.begin(9600);

	// Initialize all tasks as empty
	for (int i = 0; i < amountOfTasks; i++)
	{
		tasks[i].function = nullptr;
		tasks[i].millisAfterMidnight = 0;
		tasks[i].executed = false;
	}
	Serial.println("Bluetooth device is ready to pair");
}

void loop()
{
	handleBluetoothData(readBluetoothData, tasks);
	executeTasks(tasks, amountOfTasks);
}

// Function to reset all tasks
void resetAllTasks()
{
	for (int i = 0; i < amountOfTasks; i++)
	{
		Task &task = tasks[i];
		task.executed = false;
	}
}

void executeTasks(Task *tasks, int amountOfTasks)
{
	for (int i = 0; i < amountOfTasks; i++)
	{
		Task &task = tasks[i];
		unsigned long currentTime = getCurrentTimeOfDayMillis();
		if (currentTime >= task.millisAfterMidnight && !task.executed)
		{
			if (task.function != nullptr)
				task.function();
			task.executed = true;
		}
	}
}

// Function to read data from Bluetooth
String readBluetoothData()
{
	if (BluetoothSerial.available())
	{
		return BluetoothSerial.readStringUntil('\n');
	}
	return "";
}

// Function to handle Bluetooth data
void handleBluetoothData(ReadDataFunc readData, Task *tasks)
{
	String data = readData();
	if (data.length() > 0)
	{
		if (data.startsWith("current:"))
		{
			int hour = data.substring(data.indexOf(':') + 2, data.indexOf(':') + 4).toInt();
			int minute = data.substring(data.lastIndexOf(':') + 1).toInt();
			setCurrentTime(hour, minute);
			Serial.print("Current time set to: ");
			Serial.print(hour);
			Serial.print(":");
			Serial.println(minute);
			Serial.println(millisecondsToHHMMSS(getCurrentTimeOfDayMillis()));
		}
		else if (data.startsWith("uptime:"))
		{
			Serial.println("setting up task");
			unsigned long currentTime = getCurrentTimeOfDayMillis();
			int hour = data.substring(data.indexOf(':') + 2, data.indexOf(':') + 4).toInt();
			int minute = data.substring(data.lastIndexOf(':') + 1).toInt();
			tasks[0].function = moveCurtainsUp;
			tasks[0].millisAfterMidnight = timeToMilliseconds(hour, minute);
			tasks[0].executed = getCurrentTimeOfDayMillis() > tasks[0].millisAfterMidnight;
		}
		else if (data.startsWith("downtime:"))
		{
			Serial.println("setting down task");
			int hour = data.substring(data.indexOf(':') + 2, data.indexOf(':') + 4).toInt();
			int minute = data.substring(data.lastIndexOf(':') + 1).toInt();
			tasks[1].function = moveCurtainsDown;
			tasks[1].millisAfterMidnight = timeToMilliseconds(hour, minute);
			tasks[1].executed = getCurrentTimeOfDayMillis() > tasks[1].millisAfterMidnight;
		}
		else if (data == "up" || data == "1")
		{
			Serial.println("Received 'up' command");
		}
		else if (data == "down" || data == "0")
		{
			Serial.println("Received 'down' command");
		}
		else
		{
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

void moveCurtainsUp()
{
	Serial.println("Time to go up!");
	motorForward();
	unsigned long beginTime = millis();
	for (;;)
	{
		if (millis() - beginTime > 60000UL){
			Serial.println("something probably broke, aborting");
			motorStop();
			break;
		}
		if (readCeilingSensor())
		{
			motorStop();
			break;
		}
		else if (motorIsStopped)
		{
			Serial.println("Motor stopped unexpectedly");
			break;
		}
	}
}

void moveCurtainsDown()
{
	Serial.println("Time to go down!");
	motorReverse();
	unsigned long beginTime = millis();
	for (;;)
	{
		if (millis() - beginTime > 60000UL){
			Serial.println("something probably broke, aborting");
			motorStop();
			break;
		}
		if (readFloorSensor())
		{
			motorStop();
			break;
		}
		else if (motorIsStopped)
		{
			Serial.println("Motor stopped unexpectedly");
			break;
		}
	}
}

bool readCeilingSensor()
{
	return false;
}

bool readFloorSensor()
{
	return false;
}