/*
=========================================================
Project 02 : Gas & Fire Safety Alert System
Assignment 03 - IIT Jammu Summer School 2026

Hardware:
ESP32 Dev Module
MQ-2 Gas Sensor
Flame Sensor
SSD1306 OLED (128x64)
Green LED
Red LED
Active Buzzer
=========================================================
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------------- Pin Definitions ----------------
#define MQ2_PIN      34      // Analog pin
#define FLAME_PIN    35      // Digital pin

#define GREEN_LED    26
#define RED_LED      25
#define BUZZER       27

// ---------------- Variables ----------------
int gasValue = 0;
bool fireDetected = false;
String statusText = "SAFE";

unsigned long sensorTimer = 0;
unsigned long serialTimer = 0;
unsigned long buzzerTimer = 0;

const unsigned long SENSOR_INTERVAL = 1000;
const unsigned long SERIAL_INTERVAL = 5000;
const unsigned long BUZZER_INTERVAL = 250;

bool buzzerState = false;

// Adjust this according to your MQ-2 calibration
const int GAS_THRESHOLD = 1500;

void setup()
{
    Serial.begin(115200);

    pinMode(FLAME_PIN, INPUT);

    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);

    Wire.begin(21,22);

    if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C))
    {
        Serial.println("OLED initialization failed!");
        while(true);
    }

    display.clearDisplay();
    display.display();

    Serial.println("millis,gas,fire,status");
}

void loop()
{
    unsigned long currentMillis = millis();

    if(currentMillis - sensorTimer >= SENSOR_INTERVAL)
    {
        sensorTimer = currentMillis;

        gasValue = analogRead(MQ2_PIN);
        fireDetected = digitalRead(FLAME_PIN) == LOW;

        if(gasValue > GAS_THRESHOLD && fireDetected)
            statusText = "GAS+FIRE";
        else if(gasValue > GAS_THRESHOLD)
            statusText = "GAS LEAK";
        else if(fireDetected)
            statusText = "FIRE";
        else
            statusText = "SAFE";

        if(statusText == "SAFE")
        {
            digitalWrite(GREEN_LED,HIGH);
            digitalWrite(RED_LED,LOW);
            digitalWrite(BUZZER,LOW);
            buzzerState = false;
        }
        else
        {
            digitalWrite(GREEN_LED,LOW);
            digitalWrite(RED_LED,HIGH);
        }

        // OLED Display
        display.clearDisplay();

        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);

        display.setCursor(12,0);
        display.println("Gas & Fire Alert");

        display.drawLine(0,10,128,10,SSD1306_WHITE);

        display.setCursor(0,18);
        display.print("Gas: ");
        display.println(gasValue);

        display.setCursor(0,34);
        display.print("Fire: ");
        display.println(fireDetected ? "YES" : "NO");

        display.setCursor(0,50);
        display.print("Status:");
        display.print(statusText);

        display.display();
    }

    if(statusText != "SAFE")
    {
        if(currentMillis - buzzerTimer >= BUZZER_INTERVAL)
        {
            buzzerTimer = currentMillis;

            buzzerState = !buzzerState;
            digitalWrite(BUZZER,buzzerState);
        }
    }

    if(currentMillis - serialTimer >= SERIAL_INTERVAL)
    {
        serialTimer = currentMillis;

        Serial.print(currentMillis);
        Serial.print(",");

        Serial.print(gasValue);
        Serial.print(",");

        Serial.print(fireDetected);
        Serial.print(",");

        Serial.println(statusText);
    }
}