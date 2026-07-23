# Project 06 – Wi-Fi Weather Dashboard

## Overview

The Wi-Fi Weather Dashboard is an IoT-based weather monitoring system built using the ESP32. It reads temperature, humidity, atmospheric pressure, altitude, and light intensity from connected sensors and hosts a live web dashboard over Wi-Fi. Any device connected to the same local network can access the dashboard using the ESP32 IP address through a web browser.

---

## Objective

To develop a local weather station that monitors environmental conditions and displays real-time sensor data on a web dashboard hosted directly by the ESP32.

---

## Components Required

- ESP32 DevKit
- DHT22 Sensor (Wokwi equivalent of DHT11)
- BMP280 Pressure & Altitude Sensor
- SSD1306 OLED Display (I2C)
- LDR Sensor
- Breadboard
- Jumper Wires

---

## Pin Connections

| Component | ESP32 Pin |
|-----------|-----------|
| DHT22 DATA | GPIO4 |
| BMP280 SDA | GPIO21 |
| BMP280 SCL | GPIO22 |
| OLED SDA | GPIO21 |
| OLED SCL | GPIO22 |
| LDR Analog Output | GPIO34 |
| 3.3V | DHT22, BMP280, OLED, LDR |
| GND | Common Ground |

---

## Software Requirements

- Arduino IDE
- ESP32 Board Package
- WiFi Library
- WebServer Library
- DHT Sensor Library
- Adafruit BMP280 Library
- Adafruit SSD1306 Library
- Adafruit GFX Library
- ArduinoJson Library

---

## Features

- Connects to local Wi-Fi
- Displays SSID and IP Address on OLED
- Reads temperature and humidity
- Measures atmospheric pressure
- Calculates altitude
- Reads light intensity
- Hosts a live HTML dashboard
- Automatic webpage refresh every 10 seconds
- JSON API available at `/data`
- Automatic Wi-Fi reconnection

---

## Working Principle

After power-on, the ESP32 connects to the configured Wi-Fi network. The OLED displays the connected SSID and IP address. Every five seconds the ESP32 reads values from the DHT22, BMP280, and LDR sensors. A built-in web server hosts a dashboard displaying all sensor values. Any device connected to the same Wi-Fi network can access the dashboard using the ESP32 IP address.

The dashboard background changes according to temperature:
- Blue for cooler temperatures
- Orange for warmer temperatures

If the Wi-Fi connection is lost, the ESP32 automatically attempts to reconnect every 30 seconds.

---

## JSON Endpoint

```
http://ESP32_IP/data
```

Example Response

```json
{
  "temp": 28.5,
  "humidity": 65.2,
  "pressure": 1012.8,
  "altitude": 112.4,
  "light": 2430
}
```

---

## HTML Dashboard Features

- Live Temperature
- Humidity
- Pressure
- Altitude
- Light Level
- Connected Wi-Fi SSID
- ESP32 IP Address
- Auto Refresh every 10 seconds

---

## Applications

- Smart Weather Station
- Home Automation
- Greenhouse Monitoring
- Agriculture
- Environmental Monitoring
- Educational IoT Projects

---

## Future Enhancements

- Historical Data Logging
- Cloud Storage
- Mobile Application
- Email Alerts
- Weather Prediction
- MQTT Integration

---

## Author

**Dinesh Patra**

IoT Summer School 2026

Assignment 03 – Project 06: Wi-Fi Weather Dashboard