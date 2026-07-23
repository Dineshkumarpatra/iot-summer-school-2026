# Project 02 – Gas & Fire Safety Alert System

## Assignment Information

- **Assignment:** Assignment 03
- **Project:** Project 02 – Gas & Fire Safety Alert System
- **Program:** IIT Jammu Summer School 2026
- **Developer:** Dinesh Kumar Patra
- **Platform:** ESP32
- **Programming Language:** Arduino (C++)

---

# Project Overview

The Gas & Fire Safety Alert System is an IoT-based safety application developed using the ESP32 microcontroller. It continuously monitors the environment for combustible gas leakage and fire hazards using an MQ-2 Gas Sensor and a Flame Sensor.

Whenever a dangerous condition is detected, the system immediately alerts the user using LEDs and an active buzzer while displaying the current status on the OLED display. The sensor readings are also transmitted through the Serial Monitor in CSV format for monitoring and logging.

This project demonstrates a simple and effective safety system suitable for homes, laboratories, kitchens, industries, and smart buildings.

---

# Objectives

- Detect combustible gas leakage.
- Detect the presence of fire.
- Display live sensor information.
- Provide instant visual alerts.
- Provide audible warning.
- Log sensor data through Serial Monitor.
- Demonstrate real-time IoT monitoring using ESP32.

---

# Features

- Real-time Gas Detection
- Real-time Fire Detection
- OLED Status Display
- Green LED Safe Indicator
- Red LED Danger Indicator
- Active Buzzer Alarm
- CSV Serial Output
- Non-blocking programming using millis()
- Low-cost IoT Safety Solution

---

# Hardware Components

| Component | Quantity |
|------------|----------|
| ESP32 Development Board | 1 |
| MQ-2 Gas Sensor | 1 |
| Flame Sensor | 1 |
| SSD1306 OLED Display (128×64) | 1 |
| Green LED | 1 |
| Red LED | 1 |
| Active Buzzer | 1 |
| 220Ω Resistors | 2 |
| Breadboard | 1 |
| Jumper Wires | As Required |

---

# Software Requirements

- Arduino IDE
- ESP32 Board Package
- Adafruit SSD1306 Library
- Adafruit GFX Library

---

# Pin Connections

| ESP32 Pin | Component |
|------------|-----------|
| GPIO34 | MQ-2 Analog Output |
| GPIO35 | Flame Sensor Digital Output |
| GPIO21 | OLED SDA |
| GPIO22 | OLED SCL |
| GPIO26 | Green LED |
| GPIO25 | Red LED |
| GPIO27 | Active Buzzer |
| 3.3V | Sensor & OLED VCC |
| GND | Common Ground |

---

# Working Principle

1. ESP32 continuously reads the MQ-2 Gas Sensor.
2. ESP32 continuously checks the Flame Sensor.
3. The OLED displays live sensor information.
4. If gas exceeds the threshold, the system detects a gas leak.
5. If the flame sensor detects fire, the system reports a fire alert.
6. During dangerous conditions:
   - Red LED turns ON.
   - Active buzzer sounds.
7. During safe conditions:
   - Green LED remains ON.
8. Sensor readings are printed to the Serial Monitor in CSV format every five seconds.

---

# Alert Conditions

| Condition | Green LED | Red LED | Buzzer |
|------------|-----------|----------|---------|
| Safe | ON | OFF | OFF |
| Gas Leak | OFF | ON | ON |
| Fire Detected | OFF | ON | ON |
| Gas + Fire | OFF | ON | ON |

---

# OLED Display

The OLED displays:

- Gas Sensor Value
- Fire Detection Status
- System Status

Example:

Gas: 420

Fire: NO

Status: SAFE

---

# Serial Monitor Output

CSV Format

```
millis,gas,fire,status
5000,420,0,SAFE
10000,1600,0,GAS LEAK
15000,1750,1,GAS+FIRE
```

---

# Folder Structure

```
Project-02-Gas-Fire-Safety-Alert/
│
├── README.md
├── gas_fire_alert.ino

```

---

# Installation

1. Install Arduino IDE.
2. Install ESP32 Board Package.
3. Install required libraries.
4. Connect all hardware components.
5. Upload the code.
6. Open Serial Monitor at 115200 baud.
7. Observe OLED display.
8. Test the system using gas and flame.

---

# Applications

- Home Safety
- Kitchen Safety
- LPG Leakage Detection
- Industrial Safety
- Laboratory Monitoring
- Smart Buildings
- Fire Detection Systems
- IoT Monitoring Projects

---

# Future Improvements

- GSM SMS Alert
- Wi-Fi Notifications
- Blynk Dashboard
- Firebase Integration
- Mobile Application
- Cloud Data Logging
- Temperature Monitoring
- Smoke Level Analytics

---

# Expected Output

✔ OLED displays gas level and fire status.

✔ Green LED indicates normal conditions.

✔ Red LED and buzzer activate when gas leakage or fire is detected.

✔ CSV data is continuously displayed on the Serial Monitor.

---

# Author

**Dinesh Kumar Patra**

IIT Jammu Summer School 2026

Assignment 03

Project 02 – Gas & Fire Safety Alert System

---

# License

This project is developed for educational purposes as part of the IIT Jammu Summer School 2026 Assignment.