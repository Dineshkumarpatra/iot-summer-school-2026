# 🌱 Project 4 – Smart Plant Watering Monitor

## Overview

The Smart Plant Watering Monitor is an ESP32-based IoT system that continuously monitors soil moisture and environmental conditions. It automatically activates a water pump (simulated using a relay and LED) whenever the soil becomes too dry. The system also displays real-time information on an OLED screen and provides manual watering using a push button.

This project demonstrates smart agriculture concepts using embedded systems and automation.

---

# Objectives

- Monitor soil moisture in real time.
- Measure ambient temperature and humidity.
- Automatically water the plant when soil becomes dry.
- Prevent relay chattering using hysteresis.
- Display sensor readings on an OLED display.
- Provide manual watering mode using a push button.

---

# Components Used

| Component | Quantity |
|-----------|---------:|
| ESP32 Dev Board | 1 |
| Capacitive Soil Moisture Sensor | 1 |
| DHT11 Sensor | 1 |
| 2-Channel Relay Module | 1 |
| OLED Display (SSD1306 128×64 I2C) | 1 |
| Push Button | 1 |
| Active Buzzer | 1 |
| Red LED | 1 |
| Green LED | 1 |
| 220Ω Resistors | 2 |
| Breadboard | 1 |
| Jumper Wires | As Required |

---

# Pin Configuration

| Component | ESP32 GPIO |
|-----------|------------|
| Soil Moisture Sensor | GPIO34 |
| DHT11 Data | GPIO4 |
| Relay Channel 1 | GPIO26 |
| Relay Channel 2 | GPIO27 |
| OLED SDA | GPIO21 |
| OLED SCL | GPIO22 |
| Push Button | GPIO0 |
| Buzzer | GPIO14 |
| Red LED | GPIO25 |
| Green LED | GPIO2 |

---

# Wiring Connections

## Soil Moisture Sensor

- VCC → 3.3V
- GND → GND
- AOUT → GPIO34

---

## DHT11

- VCC → 3.3V
- GND → GND
- DATA → GPIO4

---

## OLED Display

- SDA → GPIO21
- SCL → GPIO22
- VCC → 3.3V
- GND → GND

---

## Relay Module

- VCC → 5V
- GND → GND
- IN1 → GPIO26
- IN2 → GPIO27

---

## Push Button

- One Side → GPIO0
- Other Side → GND

---

## LEDs

### Red LED

- GPIO25 → 220Ω → LED → GND

### Green LED

- GPIO2 → 220Ω → LED → GND

---

## Buzzer

- Positive → GPIO14
- Negative → GND

---

# Sensor Calibration

Measured values:

| Condition | ADC Value |
|-----------|----------:|
| Dry Air | 3200 |
| Fully Wet | 1200 |

Formula:

```cpp
int moisturePercent = map(rawValue, 3200, 1200, 0, 100);
moisturePercent = constrain(moisturePercent, 0, 100);
```

---

# Soil Moisture States

| Moisture | State |
|----------:|------|
| Below 30% | DRY |
| 30–70% | OPTIMAL |
| Above 70% | WET |

---

# Pump Automation

### Pump ON

- Soil Moisture < 30%

### Pump OFF

- Soil Moisture > 40%

This hysteresis prevents rapid ON/OFF switching.

---

# Manual Override

Press the push button to:

- Turn pump ON immediately.
- Keep pump ON for 5 seconds.
- Return to automatic mode.

---

# OLED Display

The OLED displays:

```
Soil : 45%

Temp : 27°C

Humidity : 68%

Status : OPTIMAL

██████████░░░░░░░░░
```

The bottom bar graph represents soil moisture.

---

# Serial Monitor Output

Example:

```
Soil Moisture : 45%

Temperature : 27°C

Humidity : 68%

Pump : OFF

Status : OPTIMAL

-----------------------------
```

---

# Working Principle

1. ESP32 reads soil moisture.
2. Reads temperature and humidity.
3. Converts ADC value into moisture percentage.
4. Determines soil condition.
5. Displays values on OLED.
6. Automatically controls relay.
7. Allows manual watering.
8. Repeats continuously.

---

# Features

- Automatic Water Pump Control
- Soil Moisture Monitoring
- Temperature Monitoring
- Humidity Monitoring
- OLED Display
- Moisture Percentage
- Bar Graph Display
- Manual Override Button
- Relay Hysteresis
- LED Status Indicators
- Buzzer Alert

---

# Folder Structure

```
Project-04-Smart-Plant-Watering-Monitor/
│
├── Project-04-Smart-Plant-Watering-Monitor.ino
├── README.md
├── images/
│   ├── circuit.png
│   ├── oled-display.png
│   ├── serial-monitor.png
│   └── working.jpg
└── demo.mp4
```

---

# Expected Output

- Soil moisture percentage updates continuously.
- OLED shows live readings.
- Pump turns ON when soil is dry.
- Pump turns OFF when moisture exceeds 40%.
- Manual override works for 5 seconds.
- LEDs indicate system status.
- Buzzer alerts during watering.

---

# Applications

- Smart Agriculture
- Home Gardening
- Indoor Plant Monitoring
- Greenhouse Automation
- Water Conservation
- Precision Farming

---

# Future Improvements

- Wi-Fi Monitoring
- ThingSpeak Integration
- Mobile App
- SMS Notifications
- Automatic Water Tank Monitoring
- Cloud Data Logging

---

# Bonus Challenge

Publish the following data to ThingSpeak every 5 minutes:

- Soil Moisture
- Temperature
- Humidity

Include the ThingSpeak channel link in the README.

---

# Conclusion

The Smart Plant Watering Monitor is an ESP32-based IoT project that automates plant irrigation using soil moisture sensing. It improves water efficiency, reduces manual effort, and demonstrates practical applications of embedded systems and smart agriculture technologies.