/*
====================================================
Project 09 : Servo Precision Position Controller
(Wokwi Version using SG90 Servo)
Author : Dinesh Patra
====================================================
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Servo myServo;

// ---------------- Pins ----------------
const int servoPin = 9;
const int potPin   = A0;
const int cwBtn    = 2;
const int ccwBtn   = 3;
const int homeBtn  = 4;
const int ledPin   = 13;

// ---------------- Variables ----------------
int currentAngle = 90;
int targetAngle  = 90;
int homeAngle    = 90;
String direction = "STOP";

bool lastCW = HIGH;
bool lastCCW = HIGH;
bool lastHOME = HIGH;

// ----------------------------------------------------

void smoothMove(int fromAngle, int toAngle)
{
  if (fromAngle < toAngle)
  {
    direction = "CW";

    for (int i = fromAngle; i <= toAngle; i++)
    {
      myServo.write(i);
      digitalWrite(ledPin, HIGH);
      delay(10);
    }
  }
  else
  {
    direction = "CCW";

    for (int i = fromAngle; i >= toAngle; i--)
    {
      myServo.write(i);
      digitalWrite(ledPin, HIGH);
      delay(10);
    }
  }

  digitalWrite(ledPin, LOW);
  currentAngle = toAngle;
}

// ----------------------------------------------------

void drawOLED()
{
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0,0);
  display.print("Current:");
  display.print(currentAngle);
  display.print((char)247);

  display.setCursor(0,16);
  display.print("Target :");
  display.print(targetAngle);
  display.print((char)247);

  display.setCursor(0,32);
  display.print("Home   :");
  display.print(homeAngle);
  display.print((char)247);

  display.setCursor(0,48);
  display.print("Dir:");
  display.print(direction);

  display.display();
}

// ----------------------------------------------------

void setup()
{
  Serial.begin(9600);

  pinMode(cwBtn, INPUT_PULLUP);
  pinMode(ccwBtn, INPUT_PULLUP);
  pinMode(homeBtn, INPUT_PULLUP);

  pinMode(ledPin, OUTPUT);

  myServo.attach(servoPin);
  myServo.write(currentAngle);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    while (1);
  }

  display.clearDisplay();
  display.display();
}

// ----------------------------------------------------

void loop()
{
  // Read Potentiometer
  targetAngle = map(analogRead(potPin), 0, 1023, 0, 180);

  // -------- Clockwise Button --------
  bool cwState = digitalRead(cwBtn);

  if (lastCW == HIGH && cwState == LOW)
  {
    int newAngle = currentAngle + 45;

    if (newAngle > 180)
      newAngle = 180;

    smoothMove(currentAngle, newAngle);
  }

  lastCW = cwState;

  // -------- Counter Clockwise Button --------
  bool ccwState = digitalRead(ccwBtn);

  if (lastCCW == HIGH && ccwState == LOW)
  {
    int newAngle = currentAngle - 45;

    if (newAngle < 0)
      newAngle = 0;

    smoothMove(currentAngle, newAngle);
  }

  lastCCW = ccwState;

  // -------- Home Button --------
  bool homeState = digitalRead(homeBtn);

  if (lastHOME == HIGH && homeState == LOW)
  {
    homeAngle = currentAngle;
    direction = "HOME";
  }

  lastHOME = homeState;

  // -------- Auto Move to Pot Target --------
  if (abs(targetAngle - currentAngle) > 2)
  {
    smoothMove(currentAngle, targetAngle);
  }
  else
  {
    direction = "STOP";
  }

  drawOLED();

  Serial.print("Current: ");
  Serial.print(currentAngle);

  Serial.print("   Target: ");
  Serial.print(targetAngle);

  Serial.print("   Home: ");
  Serial.print(homeAngle);

  Serial.print("   Direction: ");
  Serial.println(direction);

  delay(30);
}