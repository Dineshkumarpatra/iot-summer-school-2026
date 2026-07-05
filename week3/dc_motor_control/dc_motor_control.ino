

const int EN1 = 9;      // PWM Enable
const int IN1 = 8;
const int IN2 = 7;

const int potPin = A0;
const int dirBtn = 2;
const int runBtn = 3;

bool direction = true;      // true = Forward
bool motorState = true;     // true = Running

bool lastDir = HIGH;
bool lastRun = HIGH;

void setup() {

  pinMode(EN1, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(dirBtn, INPUT_PULLUP);
  pinMode(runBtn, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {

  int pot = analogRead(potPin);

  int pwm = map(pot, 0, 1023, 0, 255);
  int speedPercent = map(pot, 0, 1023, 0, 100);

  bool dir = digitalRead(dirBtn);
  bool run = digitalRead(runBtn);

  // Toggle Direction
  if (dir == LOW && lastDir == HIGH) {
    direction = !direction;
    delay(200);
  }

  // Toggle Start / Stop
  if (run == LOW && lastRun == HIGH) {
    motorState = !motorState;
    delay(200);
  }

  lastDir = dir;
  lastRun = run;

  if (motorState) {

    analogWrite(EN1, pwm);

    if (direction) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    }
    else {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    }

  }
  else {

    analogWrite(EN1, 0);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  Serial.print("Direction : ");

  if (direction)
    Serial.print("Forward");
  else
    Serial.print("Reverse");

  Serial.print(" | Speed : ");
  Serial.print(speedPercent);
  Serial.print("%");

  Serial.print(" | State : ");

  if (motorState)
    Serial.println("RUNNING");
  else
    Serial.println("STOPPED");

  delay(100);
}