# Project 7 – Motion-Activated Security Camera

## Overview

This project implements a smart motion-activated security camera using the ESP32-CAM module. The system remains in **STANDBY** mode until motion is detected by a PIR sensor. Once motion is detected, the camera starts streaming live video, activates a buzzer and LED alert, and records the event with a timestamp. After 60 seconds without motion, the system automatically returns to standby mode.

---

## Objectives

- Detect motion using a PIR sensor.
- Start live camera streaming only when motion is detected.
- Activate buzzer and LED during an alert.
- Display motion status and event log on a web page.
- Return to standby after 60 seconds of inactivity.

---

# Hardware Requirements

| Component | Quantity |
|-----------|---------:|
| ESP32-CAM (AI Thinker) | 1 |
| PIR Motion Sensor (HC-SR501) | 1 |
| Active Buzzer | 1 |
| Red LED | 1 |
| 220Ω Resistor | 1 |
| FTDI USB-to-TTL Programmer | 1 |
| Breadboard | 1 |
| Jumper Wires | As Required |

---

# Software Requirements

- Arduino IDE
- ESP32 Board Package
- WiFi Library
- esp_camera Library
- CameraWebServer Example
- Google Chrome / Edge Browser

---

# Pin Connections

| ESP32-CAM | Component |
|------------|-----------|
| 5V | PIR VCC |
| GND | PIR GND |
| GPIO13 | PIR OUT |
| GPIO14 | Buzzer (+) |
| GPIO2 | Red LED |
| GND | LED Cathode |
| GND | Buzzer (-) |

---

# FTDI Programming Connections

| FTDI | ESP32-CAM |
|------|-----------|
| TX | U0R |
| RX | U0T |
| 5V | 5V |
| GND | GND |
| GPIO0 | GND (Upload Mode Only) |

---

# Upload Procedure

1. Connect the FTDI programmer to the ESP32-CAM.
2. Connect GPIO0 to GND.
3. Select **AI Thinker ESP32-CAM** in Arduino IDE.
4. Select the correct COM Port.
5. Upload the program.
6. Disconnect GPIO0 from GND.
7. Press the RESET button.
8. Open the Serial Monitor at **115200 baud**.
9. Note the IP address displayed.
10. Open the IP address in a web browser.

---

# System Operation

## STANDBY Mode

- Camera streaming is inactive.
- PIR sensor continuously monitors motion.
- LED OFF.
- Buzzer OFF.

---

## ALERT Mode

When motion is detected:

- Red LED turns ON.
- Buzzer beeps for 200 ms.
- Camera live stream starts.
- Motion status changes to **Detected**.
- Event timestamp is stored.
- Browser displays the live stream.

---

## Return to Standby

If no motion is detected for 60 seconds:

- Camera stream stops.
- LED turns OFF.
- System returns to standby.
- State transition is printed on the Serial Monitor.

---

# Web Dashboard Features

The browser page displays:

- Live MJPEG Camera Stream
- Motion Detection Status
- Camera Status
- Last Five Motion Events
- Event Timestamps
- ESP32 IP Address

---

# Serial Monitor Output

Example:

```
WiFi Connected
IP Address: 192.168.1.105

System State : STANDBY

Motion Detected!

Alert Mode Started

Event #1
Timestamp : 52341 ms

Returning to Standby...
```

---

# Project Features

- Motion Detection using PIR Sensor
- ESP32-CAM Live Video Streaming
- Motion Activated Camera
- Buzzer Alert
- LED Alert Indicator
- Event Logging
- Automatic Standby Mode
- Browser-Based Monitoring
- WiFi Enabled Surveillance

---

# Working Principle

1. ESP32-CAM connects to WiFi.
2. PIR sensor continuously monitors movement.
3. Motion detection triggers an interrupt.
4. Camera streaming begins.
5. LED and buzzer provide local alert.
6. Motion event is logged with timestamp.
7. Live video is displayed in a browser.
8. After 60 seconds without motion, the system returns to standby mode.

---

# Folder Structure

```
Project-07-Motion-Activated-Security-Camera/
│
├── Project-07-Motion-Activated-Security-Camera.ino
├── README.md
├── images/
│   ├── circuit.png
│   ├── dashboard.png
│   └── stream.png
└── demo.mp4
```

---

# Expected Output

- ESP32-CAM connects to WiFi.
- Browser displays the live camera stream.
- PIR detects movement.
- LED turns ON.
- Buzzer sounds.
- Motion event appears in the log.
- Camera remains active while motion continues.
- Returns to standby after 60 seconds of inactivity.

---

# Bonus Challenge

Save a JPEG image to the microSD card whenever motion is detected.

Example filenames:

```
motion_001.jpg
motion_002.jpg
motion_003.jpg
```

---

# Applications

- Home Security
- Hostel Room Monitoring
- Office Surveillance
- Smart Door Monitoring
- Warehouse Security
- Intrusion Detection
- IoT Surveillance System

---

# Future Improvements

- Email notifications with captured images.
- Telegram alert integration.
- Cloud image storage.
- Face detection.
- Person recognition.
- Mobile application support.
- Night vision enhancement.
- Motion sensitivity adjustment.

---

# Conclusion

This project demonstrates a complete IoT-based motion-activated security camera using the ESP32-CAM. It combines motion sensing, live video streaming, event logging, and real-time alerts to create a simple and effective surveillance system suitable for smart homes and security applications.