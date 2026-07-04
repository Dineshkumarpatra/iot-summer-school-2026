# LED Blink Project

## Project Description

This project demonstrates the basic operation of an LED using an Arduino UNO. The LED blinks continuously, supports serial monitoring of blink count, and uses a potentiometer to control the blink speed.

---

## Hardware Required

- Arduino UNO
- LED
- 220 Ω Resistor
- Potentiometer (10 kΩ)
- Breadboard
- Jumper Wires
- USB Cable

---

## Circuit Diagram Description

- Connect the LED anode (+) to Digital Pin 13 through a 220 Ω resistor.
- Connect the LED cathode (-) to GND.
- Connect the potentiometer:
  - One terminal to 5V
  - One terminal to GND
  - Middle terminal (wiper) to Analog Pin A0.

---

## How to Upload the Code

1. Connect the Arduino UNO to the computer using a USB cable.
2. Open the Arduino IDE.
3. Open `led_blink.ino`.
4. Select the correct **Board** and **Port**.
5. Click **Verify** to compile the program.
6. Click **Upload** to transfer the code to the Arduino.

---

## Expected Output

- The LED blinks continuously.
- Blink speed changes according to the potentiometer position.
- The Serial Monitor displays the blink count and current delay value.

---

## Troubleshooting Tips

1. Verify that the correct board and COM port are selected in the Arduino IDE.
2. Ensure all wiring connections are secure and the LED polarity is correct.
3. If the potentiometer does not affect blink speed, check that its middle pin is connected to A0.
