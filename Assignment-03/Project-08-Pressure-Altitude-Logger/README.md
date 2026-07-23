# Project 08 – IoT Pressure & Altitude Logger

## Overview

This project is an IoT-based Pressure & Altitude Logger built using the ESP32, BMP280 sensor, and SSD1306 OLED Display. The system continuously monitors atmospheric pressure, temperature, and altitude while predicting weather trends based on pressure changes.

Pressure readings are stored in a circular buffer, allowing the system to compare current and previous values to determine whether the pressure is rising, falling, or stable.

---

## Objective

- Read atmospheric pressure, temperature, and altitude.
- Store the last 24 pressure readings.
- Calculate pressure trends.
- Predict possible weather conditions.
- Display sensor values and trend on an OLED display.
- Use a potentiometer to simulate the local altitude for sea-level pressure compensation.
- Print all readings to the Serial Monitor.

---

## Components Used

| Component | Quantity |
|----------|----------|
| ESP32 Dev Board | 1 |
| BMP280 Pressure Sensor | 1 |
| SSD1306 OLED Display (I2C) | 1 |
| 10kΩ Potentiometer | 1 |
| Red LED | 1 |
| Green LED | 1 |
| 220Ω Resistors | 2 |
| Breadboard | 1 |
| Jumper Wires | As required |

---

## Circuit Connections

### BMP280

| BMP280 | ESP32 |
|--------|-------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO21 |
| SCL | GPIO22 |

### OLED Display

| OLED | ESP32 |
|------|-------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO21 |
| SCL | GPIO22 |

### Potentiometer

| Pin | ESP32 |
|-----|-------|
| Left | 3.3V |
| Right | GND |
| Middle (Wiper) | GPIO34 |

### LEDs

| LED | ESP32 |
|-----|-------|
| Green LED (+) | GPIO25 |
| Red LED (+) | GPIO26 |
| Negative | GND through 220Ω resistor |

---

## Working Principle

1. The BMP280 measures atmospheric pressure and temperature.
2. The potentiometer sets the local altitude (0–2000 m).
3. Sea-level pressure is calculated using the altitude compensation formula.
4. Every reading is stored in a circular buffer of 24 values.
5. The newest reading is compared with the oldest reading.
6. The weather trend is determined:
   - Rising Pressure → Good Weather
   - Falling Pressure → Rain Likely
   - Stable Pressure → No Significant Change
7. The OLED alternates between:
   - Sensor Data Page
   - Weather Trend Page
8. All sensor values are printed to the Serial Monitor every 30 seconds.

---

## Pressure Trend Logic

```
Difference = Newest Pressure − Oldest Pressure
```

| Difference | Trend |
|------------|-------|
| > +50 Pa | Rising ↑ |
| < -50 Pa | Falling ↓ |
| Between -50 Pa and +50 Pa | Stable → |

---

## Sea-Level Pressure Formula

The BMP280 measures atmospheric pressure at the current altitude. To compare readings accurately with weather forecasts, pressure is converted to sea-level pressure.

```
P₀ = P / (1 - altitude / 44330.0)^5.255
```

Where:

- **P₀** = Sea-Level Pressure
- **P** = Measured Pressure
- **Altitude** = Local altitude (meters)

Higher altitude naturally results in lower atmospheric pressure. This formula compensates for altitude differences so pressure readings can be compared consistently.

---

## OLED Display

### Page 1

- Pressure (hPa)
- Temperature (°C)
- Altitude (m)

### Page 2

- Pressure Trend
- Rising ↑
- Falling ↓
- Stable →

Pages automatically change every 5 seconds.

---

## Serial Monitor Output

Displays:

- Pressure
- Temperature
- Altitude
- Sea-Level Pressure
- Trend
- Reading Number

Updated every 30 seconds.

---

## Folder Structure

```
Project-08-Pressure-Altitude-Logger/
│
├── Project-08-Pressure-Altitude-Logger.ino
├── README.md
└── images/
    ├── oled-page1.jpg
    ├── oled-page2.jpg
    └── serial-monitor.png
```

---

## Features

- Real-time pressure monitoring
- Temperature measurement
- Altitude calculation
- Sea-level pressure compensation
- Circular buffer for data logging
- Pressure trend analysis
- OLED visualization
- LED weather indication
- Serial Monitor logging

---

## Future Improvements

- Store readings on an SD card.
- Upload data to Firebase or ThingSpeak.
- Display graphs on a web dashboard.
- Add Wi-Fi-based weather synchronization.
- Send notifications when pressure changes rapidly.

---

## Author

**Name:** Dinesh Patra

**Course:** IoT Summer School 2026

**Assignment:** Project 08 – Pressure & Altitude Logger