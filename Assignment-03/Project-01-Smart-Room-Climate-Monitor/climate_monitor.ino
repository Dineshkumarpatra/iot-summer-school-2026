/*
==========================================================
 Project 01 : Smart Room Climate Monitor
 Assignment 03 - IIT Jammu Summer School 2026

 Board  : ESP32 Dev Module
 Sensor : DHT11
 Display: SSD1306 OLED (128x64)
==========================================================
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------------- Pin Definitions ----------------
#define DHT_PIN     4
#define GREEN_LED   26
#define RED_LED     25
#define BUZZER      27

// ---------------- Objects ----------------
DHTesp dht;

// ---------------- Variables ----------------
unsigned long previousSensorMillis = 0;
unsigned long previousSerialMillis = 0;

const unsigned long sensorInterval = 2000;   // Read every 2 sec
const unsigned long serialInterval = 5000;   // Print every 5 sec

float temperature = 0;
float humidity = 0;
String status = "";

void setup()
{
  Serial.begin(115200);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);

  dht.setup(DHT_PIN, DHTesp::DHT11);

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED initialization failed!");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  Serial.println("millis,temp,humidity,status");
}

void loop()
{
  unsigned long currentMillis = millis();

  // Read Sensor Every 2 Seconds
  if (currentMillis - previousSensorMillis >= sensorInterval)
  {
    previousSensorMillis = currentMillis;

    TempAndHumidity data = dht.getTempAndHumidity();

    temperature = data.temperature;
    humidity = data.humidity;

    // Determine Status
    if (temperature > 38 || humidity > 80)
      status = "DANGER";
    else if (temperature >= 32)
      status = "HOT";
    else if (temperature >= 25)
      status = "COMFORT";
    else
      status = "COOL";

    // Alarm Logic
    if (status == "DANGER")
    {
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);

      digitalWrite(BUZZER, HIGH);
      delay(200);
      digitalWrite(BUZZER, LOW);
    }
    else
    {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      digitalWrite(BUZZER, LOW);
    }

    // OLED Display
    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Smart Climate");

    display.setCursor(0, 18);
    display.print("Temp : ");
    display.print(temperature);
    display.println(" C");

    display.setCursor(0, 34);
    display.print("Hum  : ");
    display.print(humidity);
    display.println(" %");

    display.setCursor(0, 50);
    display.print("Status: ");
    display.print(status);

    display.display();
  }

  // Print CSV Every 5 Seconds
  if (currentMillis - previousSerialMillis >= serialInterval)
  {
    previousSerialMillis = currentMillis;

    Serial.print(currentMillis);
    Serial.print(",");

    Serial.print(temperature);
    Serial.print(",");

    Serial.print(humidity);
    Serial.print(",");

    Serial.println(status);
  }
}