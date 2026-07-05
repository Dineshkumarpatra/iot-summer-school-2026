/*
 * Project : Smart Street Light
 * Author  : Dinesh Patra
 * Description:
 * LDR + PIR based automatic street light.
 */

const int ldrPin = A0;
const int pirPin = 2;
const int ledPin = 9;

const int ldrThreshold = 500;

unsigned long motionTime = 0;
bool motionDetected = false;

void setup() {

  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  int light = analogRead(ldrPin);

  if (light > ldrThreshold) {

    analogWrite(ledPin,0);

    Serial.println("[DAY] EVENT: Street Light OFF");

    delay(1000);

    return;
  }

  bool motion = digitalRead(pirPin);

  if (motion) {

    analogWrite(ledPin,255);

    motionDetected = true;

    motionTime = millis();

    Serial.println("[NIGHT] EVENT: Motion Detected");

  }

  if (motionDetected) {

    if (millis() - motionTime >= 30000) {

      analogWrite(ledPin,51);

      motionDetected = false;

      Serial.println("[NIGHT] EVENT: No Motion - Dim Mode");

    }

  }

  delay(200);

}