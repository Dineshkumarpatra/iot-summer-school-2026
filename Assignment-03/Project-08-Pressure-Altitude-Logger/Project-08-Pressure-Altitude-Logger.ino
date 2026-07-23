#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP085.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_BMP085 bmp;

#define POT_PIN     34
#define GREEN_LED   25
#define RED_LED     26

float pressureLog[24];
int logIndex = 0;
bool bufferFilled = false;

float pressure;
float temperature;
float altitude;
float seaLevelPressure;

String trend = "STABLE";

unsigned long lastSensorRead = 0;
unsigned long lastPageSwitch = 0;

const unsigned long SENSOR_INTERVAL = 30000; // 30 sec
const unsigned long PAGE_INTERVAL = 5000;    // 5 sec

bool page = false;

void setup() {

  Serial.begin(115200);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Failed");
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  if (!bmp.begin(0x76)) {

    if (!bmp.begin(0x77)) {

      display.clearDisplay();
      display.setCursor(0, 20);
      display.println("BMP280 NOT FOUND");
      display.display();

      Serial.println("BMP280 not found!");

      while (1);
    }
  }

  for (int i = 0; i < 24; i++)
    pressureLog[i] = 0;

  Serial.println();
  Serial.println("==============================================");
  Serial.println(" IoT Pressure & Altitude Logger ");
  Serial.println("==============================================");
}

void loop() {

  if (millis() - lastSensorRead >= SENSOR_INTERVAL) {

    lastSensorRead = millis();

    int pot = analogRead(POT_PIN);

    float localAltitude = map(pot, 0, 4095, 0, 2000);

    pressure = bmp.readPressure();
    temperature = bmp.readTemperature();

    seaLevelPressure =
      pressure /
      pow(1.0 - (localAltitude / 44330.0), 5.255);

    altitude = bmp.readAltitude(seaLevelPressure / 100.0);

    pressureLog[logIndex] = pressure;

    logIndex++;

    if (logIndex >= 24) {
      logIndex = 0;
      bufferFilled = true;
    }

    calculateTrend();

    Serial.println("------------------------------------------");
    Serial.print("Temperature : ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("Pressure    : ");
    Serial.print(pressure / 100.0);
    Serial.println(" hPa");

    Serial.print("Altitude    : ");
    Serial.print(altitude);
    Serial.println(" m");

    Serial.print("Sea Level P : ");
    Serial.print(seaLevelPressure / 100.0);
    Serial.println(" hPa");

    Serial.print("Trend       : ");
    Serial.println(trend);

    Serial.println("------------------------------------------");
  }

  if (millis() - lastPageSwitch >= PAGE_INTERVAL) {

    lastPageSwitch = millis();

    page = !page;
  }

  updateOLED();
}

void calculateTrend() {

  if (!bufferFilled && logIndex < 2) {

    trend = "STABLE";
    return;
  }

  int newest =
      (logIndex - 1 + 24) % 24;

  int oldest =
      bufferFilled ? logIndex : 0;

  float diff =
      pressureLog[newest] - pressureLog[oldest];

  if (diff > 50) {

    trend = "RISING";

    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }

  else if (diff < -50) {

    trend = "FALLING";

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }

  else {

    trend = "STABLE";

    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }
}

void updateOLED() {

  display.clearDisplay();

  if (!page) {

    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("Pressure Logger");

    display.setCursor(0, 18);
    display.print("Temp : ");
    display.print(temperature, 1);
    display.print(" C");

    display.setCursor(0, 34);
    display.print("Press: ");
    display.print(pressure / 100.0, 1);
    display.print(" hPa");

    display.setCursor(0, 50);
    display.print("Alt  : ");
    display.print(altitude, 1);
    display.print(" m");
  }

  else {

    display.setTextSize(2);

    display.setCursor(25, 0);

    if (trend == "RISING")
      display.print("^");

    else if (trend == "FALLING")
      display.print("v");

    else
      display.print("-");

    display.setTextSize(1);

    display.setCursor(0, 30);
    display.print("Trend:");

    display.setCursor(50, 30);
    display.print(trend);

    display.setCursor(0, 48);
    display.print("SLP:");

    display.print(seaLevelPressure / 100.0);

    display.print(" hPa");
  }

  display.display();
}