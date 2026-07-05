# Environmental Monitoring Station

## Project Description

This project monitors environmental conditions using a DHT22 sensor (used in Wokwi as a replacement for DHT11). The Arduino reads temperature and humidity values every 2 seconds and displays them in CSV format on the Serial Monitor.

## Hardware Used

- Arduino UNO
- DHT22 Sensor (Wokwi replacement for DHT11)
- Red LED
- Green LED
- 2 × 220 Ω Resistors
- Jumper Wires

## Connections

| Component | Arduino Pin |
|-----------|-------------|
| DHT22 DATA | D2 |
| DHT22 VCC | 5V |
| DHT22 GND | GND |
| Red LED | D8 |
| Green LED | D9 |

## Features

- Reads Temperature (°C)
- Reads Temperature (°F)
- Reads Humidity (%)
- Displays data in CSV format every 2 seconds
- Red LED turns ON if Temperature > 35°C or Humidity > 80%
- Green LED indicates normal conditions

## Library Used

- DHT Sensor Library by Adafruit

## Note

Wokwi does not currently provide a DHT11 component. Therefore, DHT22 was used for simulation. The program logic is identical to the DHT11 implementation.