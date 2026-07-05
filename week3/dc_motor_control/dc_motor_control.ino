/*
 * Project : DC Motor Speed Control with L298N
 * Author  : Dinesh Patra
 */

const int ENA = 9;
const int IN1 = 8;
const int IN2 = 7;

const int potPin = A0;
const int dirButton = 2;
const int stopButton = 3;

bool direction = true;
bool motorRunning = true;

bool lastDirButton = HIGH;
bool lastStopButton = HIGH;

void setup() {

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(dirButton, INPUT_PULLUP);
  pinMode(stopButton, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {

  int pot = analogRead(potPin);

  int pwm = map(pot, 0, 1023, 0, 255);
  int percent = map(pot, 0, 1023, 0, 100);

  bool dirState = digitalRead(dirButton);
  bool stopState = digitalRead(stopButton);

  if (dirState == LOW && lastDirButton == HIGH) {
    direction = !direction;
    delay(200);
  }

  if (stopState == LOW && lastStopButton == HIGH) {
    motorRunning = !motorRunning;
    delay(200);
  }

  lastDirButton = dirState;
  lastStopButton = stopState;

  if (motorRunning) {

    analogWrite(ENA, pwm);

    if (direction) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    } else {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    }

  } else {

    analogWrite(ENA, 0);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  Serial.print("Direction : ");

  if (direction)
    Serial.print("Forward");
  else
    Serial.print("Reverse");

  Serial.print(" | Speed : ");
  Serial.print(percent);
  Serial.print("%");

  Serial.print(" | State : ");

  if (motorRunning)
    Serial.println("RUNNING");
  else
    Serial.println("STOPPED");

  delay(200);
}