# Project 05 – Bluetooth Home Light Controller

## Overview

The Bluetooth Home Light Controller is an ESP32-based home automation project that allows users to control two lights wirelessly using Bluetooth commands. The project also supports manual control using push buttons, provides audible confirmation through a buzzer, and automatically turns off the lights after a period of inactivity.

## Objective

To develop a Bluetooth-controlled home lighting system using ESP32 with manual override and automatic energy-saving functionality.

## Components Required

- ESP32 DevKit
- 2 LEDs
- 2 Push Buttons
- Active Buzzer
- Breadboard
- Jumper Wires
- 220Ω Resistors

## Pin Connections

| Component | ESP32 Pin |
|-----------|-----------|
| Light 1 LED | GPIO26 |
| Light 2 LED | GPIO27 |
| Push Button 1 | GPIO0 |
| Push Button 2 | GPIO35 |
| Active Buzzer | GPIO14 |
| Built-in LED | GPIO2 |

## Software Requirements

- Arduino IDE
- ESP32 Board Package
- BluetoothSerial Library

## Bluetooth Commands

| Command | Function |
|---------|----------|
| 1 | Light 1 ON |
| 2 | Light 1 OFF |
| 3 | Light 2 ON |
| 4 | Light 2 OFF |
| 5 | All Lights ON |
| 6 | All Lights OFF |
| ? | Display Current Status |

## Working Principle

The ESP32 receives Bluetooth commands from a mobile phone and controls two lights accordingly. Every valid command activates a short buzzer beep and sends the current light status back to the user. Two push buttons provide manual control of each light. If no command is received for 30 minutes, the controller automatically switches off all lights to save energy.

## Applications

- Smart Homes
- Home Automation
- Office Lighting
- Classroom Automation
- Hostel Room Automation

## Future Enhancements

- Mobile Application
- Voice Control
- Wi-Fi Integration
- Google Assistant Support
- Cloud Monitoring

## Author

**Dinesh Patra**

IoT Summer School 2026 – Assignment 03