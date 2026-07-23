/*
=========================================================
Project 01 : Smart Room Climate Monitor
Assignment 03 - IIT Jammu Summer School 2026

Hardware:
ESP32 Dev Module
DHT22
SSD1306 OLED (128x64 I2C)
Green LED
Red LED
Active Buzzer

Author: Dinesh Kumar Patra
=========================================================
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------------- Pins ----------------
#define DHTPIN      4
#define DHTTYPE     DHT22

#define GREEN_LED   26
#define RED_LED     25
#define BUZZER      27

DHT dht(DHTPIN, DHTTYPE);

// ---------------- Variables ----------------
float temperature = 0;
float humidity = 0;
String climateStatus = "UNKNOWN";

unsigned long sensorTimer = 0;
unsigned long serialTimer = 0;
unsigned long buzzerTimer = 0;

bool buzzerState = false;

const unsigned long SENSOR_INTERVAL = 2000;
const unsigned long SERIAL_INTERVAL = 5000;
const unsigned long BUZZER_INTERVAL = 250;

void setup()
{
  Serial.begin(115200);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);

  dht.begin();

  Wire.begin(21,22);

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C))
  {
    Serial.println("OLED initialization failed!");
    while(true);
  }

  display.clearDisplay();
  display.display();

  Serial.println();
  Serial.println("===========================================");
  Serial.println(" Smart Room Climate Monitor Started");
  Serial.println("===========================================");
  Serial.println("millis,temp,humidity,status");
}

void loop()
{
  unsigned long currentMillis = millis();

  // ==========================
  // Read Sensor Every 2 Seconds
  // ==========================
  if(currentMillis - sensorTimer >= SENSOR_INTERVAL)
  {
    sensorTimer = currentMillis;

    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    if(isnan(temperature) || isnan(humidity))
    {
      climateStatus = "ERROR";

      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(10,22);
      display.println("ERROR");
      display.display();

      Serial.println("Failed to read DHT22!");

      return;
    }

    // ==========================
    // Climate Status
    // ==========================
    if(temperature > 38 || humidity > 80)
      climateStatus = "DANGER";
    else if(temperature >= 32)
      climateStatus = "HOT";
    else if(temperature >= 25)
      climateStatus = "COMFORT";
    else
      climateStatus = "COOL";

    // ==========================
    // LEDs
    // ==========================
    if(climateStatus == "DANGER")
    {
      digitalWrite(RED_LED,HIGH);
      digitalWrite(GREEN_LED,LOW);
    }
    else
    {
      digitalWrite(RED_LED,LOW);
      digitalWrite(GREEN_LED,HIGH);
      digitalWrite(BUZZER,LOW);
      buzzerState = false;
    }

    // ==========================
    // OLED Display
    // ==========================
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(18,0);
    display.println("Climate Monitor");

    display.drawLine(0,10,128,10,SSD1306_WHITE);

    display.setCursor(0,18);
    display.print("Temp : ");
    display.print(temperature,1);
    display.print((char)247);
    display.println("C");

    display.setCursor(0,34);
    display.print("Hum  : ");
    display.print(humidity,1);
    display.println("%");

    display.setCursor(0,50);
    display.print("Status: ");
    display.print(climateStatus);

    display.display();
  }

  // ==========================
  // Buzzer
  // ==========================
  if(climateStatus == "DANGER")
  {
    if(currentMillis - buzzerTimer >= BUZZER_INTERVAL)
    {
      buzzerTimer = currentMillis;

      buzzerState = !buzzerState;
      digitalWrite(BUZZER,buzzerState);
    }
  }

  // ==========================
  // Serial CSV
  // ==========================
  if(currentMillis - serialTimer >= SERIAL_INTERVAL)
  {
    serialTimer = currentMillis;

    Serial.print(currentMillis);
    Serial.print(",");

    Serial.print(temperature,1);
    Serial.print(",");

    Serial.print(humidity,1);
    Serial.print(",");

    Serial.println(climateStatus);
  }
}