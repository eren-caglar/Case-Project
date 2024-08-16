# Case Project
This project implements a secure lock system using an Arduino Nano microcontroller. The system includes a 4x4 keypad for password entry, a 16x2 LCD display for user interaction, and a servo motor for controlling the lock mechanism. The project is developed using Atmel Studio, highlighting its capabilities in AVR microcontroller programming.

## Project Overview
The security system is designed with the following components:
- Arduino Nano: Serves as the microcontroller, managing all inputs and outputs.
- 4x4 Keypad: Used for password entry, with debouncing handled by Timer0 to prevent erroneous inputs.
- 16x2 LCD Display: Provides user feedback, such as password entry and system status, using I2C communication.
- Servo Motor: Controls the lock, rotating 180 degrees to lock or unlock based on the correct password input.

## Key Features
- Password Entry: The system uses a 6-digit password entered via the keypad. The password is stored and compared in real-time.
- LCD Feedback: The LCD displays prompts and feedback, making the system user-friendly.
- Servo Motor Control: The servo motor is triggered by the correct password, moving to lock or unlock the system.

## Development Environment
This project is developed entirely in Atmel Studio, a powerful integrated development environment (IDE) for programming AVR microcontrollers. The use of Atmel Studio allows for precise control and customization of microcontroller features, such as Timer0 for keypad debouncing and Timer1 for servo motor control.

## Components and Configuration
1) Arduino Nano:
- Microcontroller: ATmega328/ATmega328P
- Operating Voltage: 5V
- Digital I/O Pins: 14 (6 PWM outputs)
- Analog Input Pins: 8
2) 4x4 Keypad:
- Configured with rows as input and columns as output.
- Debounced using Timer0 in Atmel Studio to ensure reliable keypress detection.
3) 16x2 LCD Display:
- Uses I2C communication.
- Custom libraries adapted for AVR microcontroller usage within Atmel Studio.
4) Servo Motor:
- Controlled using Timer1 in Fast PWM mode within Atmel Studio.
- Moves 180 degrees based on password validation.

## Circuit Design
The circuit design includes the following connections:
- Keypad: Connected to the digital input pins of the Arduino Nano.
- LCD Display: Connected via I2C for simple communication.
- Servo Motor: Connected to a PWM-capable pin for precise control.
Circuit diagrams and pin configurations are included in the project files for easy reference.

## Project Files
- Main Code: main.c - Contains the logic for password handling, LCD control, and servo motor operation. This code is specifically written and compiled using Atmel Studio.
- Libraries: Libraries: Includes LiquidCrystal_I2C.c and LiquidCrystal_I2C.h for LCD control. These libraries were originally designed for Arduino but have been modified and customized for AVR microcontrollers in Atmel Studio.
- Schematics: Circuit diagrams and pin connections provided for replicating the setup.

