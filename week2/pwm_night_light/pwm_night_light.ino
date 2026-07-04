/*
 * Project : PWM Fading Night Light
 * Author  : Dinesh Patra
 * Date    : 04-07-2026
 * Description:
 * Three lighting modes:
 * 1. Slow Breathing
 * 2. Fast Pulse
 * 3. SOS Pattern
 */

const int ledPin = 9;
const int buttonPin = 2;

int mode = 0;
bool lastButtonState = HIGH;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Mode 1 : Slow Breathing");
}

void breathing(int stepDelay) {
  for (int i = 0; i <= 255; i++) {
    analogWrite(ledPin, i);
    delay(stepDelay);
  }

  for (int i = 255; i >= 0; i--) {
    analogWrite(ledPin, i);
    delay(stepDelay);
  }
}

void dot() {
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(200);
}

void dash() {
  digitalWrite(ledPin, HIGH);
  delay(600);
  digitalWrite(ledPin, LOW);
  delay(200);
}

void sos() {

  dot();
  dot();
  dot();

  dash();
  dash();
  dash();

  dot();
  dot();
  dot();

  delay(1000);
}

void loop() {

  bool buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH) {

    mode++;

    if (mode > 2)
      mode = 0;

    if (mode == 0)
      Serial.println("Mode 1 : Slow Breathing");

    if (mode == 1)
      Serial.println("Mode 2 : Fast Pulse");

    if (mode == 2)
      Serial.println("Mode 3 : SOS");

    delay(250);
  }

  lastButtonState = buttonState;

  switch (mode) {

    case 0:
      breathing(6);
      break;

    case 1:
      breathing(1);
      break;

    case 2:
      sos();
      break;
  }
}