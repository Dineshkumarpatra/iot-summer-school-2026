# Project 03 – Smart Parking System

## Overview

The Smart Parking System is an ESP32-based IoT project that detects whether a parking slot is occupied or available using an HC-SR04 ultrasonic sensor. The parking status is displayed on an OLED screen, while LEDs and a buzzer provide visual and audible indications.

---

## Objective

To develop a simple and efficient parking monitoring system that automatically detects vehicle presence and displays the parking status in real time.

---

## Components Required

- ESP32 DevKit V1
- HC-SR04 Ultrasonic Sensor
- SSD1306 OLED Display (128×64, I2C)
- Green LED
- Red LED
- Active Buzzer
- 220Ω Resistors (2)
- Breadboard
- Jumper Wires

---

## Pin Connections

| Component | ESP32 Pin |
|-----------|-----------|
| HC-SR04 Trigger | GPIO5 |
| HC-SR04 Echo | GPIO18 |
| OLED SDA | GPIO21 |
| OLED SCL | GPIO22 |
| Green LED | GPIO26 |
| Red LED | GPIO25 |
| Active Buzzer | GPIO27 |

---

## Software Requirements

- Arduino IDE
- ESP32 Board Package
- Adafruit SSD1306 Library
- Adafruit GFX Library

---

## Working Principle

The ESP32 continuously measures the distance using the HC-SR04 ultrasonic sensor.

- If the distance is **greater than or equal to 15 cm**, the parking slot is **Available**.
  - Green LED turns ON
  - Red LED turns OFF
  - Buzzer remains OFF
  - OLED displays **Parking Available**

- If the distance is **less than 15 cm**, the parking slot is **Occupied**.
  - Red LED turns ON
  - Green LED turns OFF
  - Buzzer turns ON
  - OLED displays **Parking Occupied**

The measured distance is also displayed on the OLED and printed to the Serial Monitor.

---

## Applications

- Smart Parking Systems
- Shopping Malls
- Residential Apartments
- Office Buildings
- College Campuses
- Smart Cities

---

## Future Enhancements

- Multiple Parking Slot Detection
- IoT Cloud Integration
- Mobile App Monitoring
- Automatic Gate Control
- Parking Slot Reservation

---

## Author

**Dinesh Patra**

IoT Summer School 2026 – Assignment 03