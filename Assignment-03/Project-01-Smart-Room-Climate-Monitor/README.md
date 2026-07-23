# 🌡️ Project 1: Smart Room Climate Monitor

**IIT Jammu Summer School 2026 – Assignment 03**

## 📖 Overview

The Smart Room Climate Monitor is an IoT project built using an ESP32 Dev Board. It continuously monitors room temperature and humidity using a DHT11 sensor and displays the readings on a 0.96-inch OLED display. When the temperature exceeds **38°C** or the humidity exceeds **80%**, the system activates a buzzer and a red LED to alert the user. Under normal conditions, a green LED remains ON.

---

## 🎯 Objectives

- Measure room temperature and humidity in real time.
- Display readings on an OLED display.
- Indicate safe and unsafe environmental conditions.
- Generate audible and visual alerts.
- Log sensor readings in CSV format to the Serial Monitor.

---

## 🛠 Components Used

| Component | Quantity |
|-----------|---------:|
| ESP32 Dev Board (38-pin) | 1 |
| DHT11 Temperature & Humidity Sensor | 1 |
| OLED Display 0.96" (SSD1306 I2C) | 1 |
| Active Buzzer | 1 |
| Red LED | 1 |
| Green LED | 1 |
| 220Ω Resistors | 2 |
| Breadboard | 1 |
| Jumper Wires | As required |

---

## 🔌 Wiring Connections

### DHT11

| Sensor Pin | ESP32 |
|------------|-------|
| VCC | 3.3V |
| GND | GND |
| DATA | GPIO4 |

### OLED Display

| OLED Pin | ESP32 |
|----------|-------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO21 |
| SCL | GPIO22 |

### LEDs

| LED | ESP32 GPIO |
|-----|------------|
| Green LED | GPIO26 |
| Red LED | GPIO25 |

### Buzzer

| Buzzer | ESP32 |
|--------|-------|
| + | GPIO27 |
| - | GND |

---

## 📚 Libraries Required

- DHTesp
- Adafruit GFX
- Adafruit SSD1306
- Wire

---

## ⚙️ Features

- Live temperature monitoring
- Live humidity monitoring
- OLED display output
- Green LED for normal conditions
- Red LED + buzzer for alert conditions
- Comfort status indication
- CSV data logging to Serial Monitor

---

## 📊 Comfort Status

| Condition | Status |
|----------|--------|
| Temp < 25°C and Humidity < 60% | COOL |
| Temp 25–32°C | COMFORT |
| Temp 32–38°C | HOT |
| Temp > 38°C or Humidity > 80% | DANGER |

---

## 📟 Sample Serial Output

```text
millis,temp,humidity,status
5000,28.4,65,COMFORT
10000,29.0,66,COMFORT
15000,39.1,82,DANGER
```

---

## 📂 Folder Contents

```text
Project-01-Smart-Room-Climate-Monitor/
├── climate_monitor.ino
├── README.md
├── wiring_diagram.png
├── hardware_setup.jpg
├── oled_output.jpg
├── serial_monitor.png
├── alarm.jpg
└── demo_video_link.txt
```

---

## ▶️ How to Run

1. Install the ESP32 board package in Arduino IDE.
2. Install the required libraries.
3. Connect the hardware as per the wiring table.
4. Upload `climate_monitor.ino` to the ESP32.
5. Open the Serial Monitor at **115200 baud**.

---

## 📸 Expected Output

- OLED displays temperature and humidity.
- Green LED glows during normal conditions.
- Red LED and buzzer activate when thresholds are exceeded.
- CSV data is printed to the Serial Monitor every 5 seconds.

---

## 👨‍💻 Author

**Dinesh Kumar Patra**

B.Tech Computer Science & Engineering

IIT Jammu Summer School 2026