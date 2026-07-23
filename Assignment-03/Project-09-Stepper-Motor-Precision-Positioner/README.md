# Project 09: Servo Precision Position Controller (Wokwi Version)

> **IoT Summer School 2026 – Assignment 03**

---

## 📌 Project Overview

This project demonstrates a **precision position controller** using an **Arduino Uno** and an **SG90 Servo Motor**. The servo represents the motion of a precision stepper motor because **Wokwi does not currently support the 28BYJ-48 Stepper Motor with the ULN2003 driver**.

The potentiometer is used to set the desired angle, while push buttons allow manual clockwise and counter-clockwise movement in fixed increments. An OLED display continuously shows the current position, target position, home position, and movement direction.

This project simulates the working principle behind CNC machines, robotic arms, and automated positioning systems.

---

# 🎯 Objectives

- Control a servo motor accurately.
- Set a target angle using a potentiometer.
- Move the servo manually using push buttons.
- Store a logical Home position.
- Display real-time information on an OLED screen.
- Learn precision positioning techniques used in industrial automation.

---

# 🛠 Components Required

| Component | Quantity |
|------------|----------|
| Arduino Uno R3 | 1 |
| SG90 Servo Motor | 1 |
| 10k Potentiometer | 1 |
| OLED Display 0.96" (SSD1306 I2C) | 1 |
| Push Button | 3 |
| LED | 1 |
| 220Ω Resistor | 1 |
| Breadboard | 1 |
| Jumper Wires | As Required |

---

# 🔌 Circuit Connections

## Servo Motor

| Servo | Arduino |
|---------|----------|
| Signal | D9 |
| VCC | 5V |
| GND | GND |

---

## OLED Display

| OLED | Arduino |
|--------|---------|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

---

## Potentiometer

| Potentiometer | Arduino |
|---------------|----------|
| Left | GND |
| Middle | A0 |
| Right | 5V |

---

## Push Buttons

| Button | Arduino |
|----------|----------|
| Clockwise | D2 |
| Counter Clockwise | D3 |
| Home | D4 |

Buttons use **INPUT_PULLUP**, therefore no external resistors are required.

---

## LED

| LED | Arduino |
|------|----------|
| Anode | D13 (220Ω resistor) |
| Cathode | GND |

---

# 📌 Pin Mapping

| Arduino Pin | Connected Device |
|--------------|------------------|
| D2 | CW Button |
| D3 | CCW Button |
| D4 | Home Button |
| D9 | Servo Signal |
| D13 | LED |
| A0 | Potentiometer |
| A4 | OLED SDA |
| A5 | OLED SCL |

---

# ⚙ Working Principle

### 1. Potentiometer

The potentiometer generates an analog voltage between **0V and 5V**.

Arduino reads this voltage using

```cpp
analogRead(A0)
```

The value (0–1023) is mapped to

```
0° → 180°
```

which becomes the target angle.

---

### 2. Clockwise Button

Each press rotates the servo

```
+45°
```

---

### 3. Counter Clockwise Button

Each press rotates the servo

```
-45°
```

---

### 4. Home Button

Stores the current servo position as

```
Home Position
```

without moving the servo.

---

### 5. OLED Display

The OLED continuously displays

- Current Angle
- Target Angle
- Home Position
- Direction

Example

```
Current : 90°
Target  : 135°
Home    : 45°
Dir     : CW
```

---

# 💻 Software Used

- Arduino IDE 2.x
- Wokwi Simulator
- C++
- SSD1306 OLED Library

---

# 📚 Libraries Required

Install these libraries using Library Manager.

- Adafruit GFX
- Adafruit SSD1306
- Servo
- Wire

---

# 🚀 Features

✔ Potentiometer angle control

✔ Manual CW rotation

✔ Manual CCW rotation

✔ Home position memory

✔ OLED live status

✔ Smooth servo movement

✔ Serial Monitor logging

✔ LED indication while moving

---

# 📟 Serial Monitor Output

Example

```
Current: 45
Target: 120
Home: 45
Direction: CW
```

---

# 📊 Program Flow

```
Start
      │
      ▼
Initialize Servo
      │
      ▼
Initialize OLED
      │
      ▼
Read Potentiometer
      │
      ▼
Read Buttons
      │
      ▼
Move Servo
      │
      ▼
Update OLED
      │
      ▼
Print Serial Data
      │
      ▼
Repeat
```

---

# 🎯 Learning Outcomes

After completing this project you will understand

- Servo motor control
- PWM generation
- Position control
- Analog input processing
- Push button interfacing
- OLED display programming
- Human Machine Interface (HMI)
- Embedded systems programming

---

# ⚠ Why Servo Instead of Stepper?

The original assignment specifies

- 28BYJ-48 Stepper Motor
- ULN2003 Driver

However, Wokwi currently **does not support simulation of the 28BYJ-48 + ULN2003 combination**.

To demonstrate the same positioning concepts inside Wokwi, an **SG90 Servo Motor** was used instead.

On actual hardware, the servo can be replaced with

- 28BYJ-48 Stepper Motor
- ULN2003 Driver

without changing the overall project concept.

---

# 📂 Project Structure

```
Project-09-Stepper-Motor-Precision-Positioner
│
├── Project-09-Stepper-Motor-Precision-Positioner.ino
├── diagram.json
├── README.md
└── images
```

---

# 📷 Expected Output

OLED

```
Current : 135°
Target  : 180°
Home    : 90°
Dir     : CW
```

Servo rotates smoothly to the selected angle.

The LED turns ON while moving.

The Serial Monitor continuously prints position updates.

---

# 📜 Conclusion

This project demonstrates a precision position control system using an Arduino Uno and an SG90 Servo Motor. It provides practical experience with analog input, digital buttons, servo control, OLED display interfacing, and embedded programming concepts. Although a servo is used in simulation, the same control logic forms the foundation for CNC machines, robotic arms, automated manufacturing systems, and industrial motion control applications.

---

## 👨‍💻 Author

**Dinesh Patra**

**IoT Summer School 2026**

Assignment 03 – Project 09

Arduino + Wokwi Simulation