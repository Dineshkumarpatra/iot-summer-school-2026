/*
 * Project     : Traffic Light Controller
 * Author      : Dinesh Patra
 * Date        : 04-07-2026
 * Description : Traffic light with pedestrian button and Serial Monitor output.
 */

const int redLED = 2;
const int yellowLED = 3;
const int greenLED = 4;
const int buttonPin = 7;

void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  pinMode(buttonPin, INPUT);

  Serial.begin(9600);
}

void showRed(unsigned long duration) {
  digitalWrite(redLED, HIGH);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);

  Serial.print("[");
  Serial.print(millis());
  Serial.println(" ms] RED");

  delay(duration);
}

void showYellow(unsigned long duration) {
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, HIGH);
  digitalWrite(greenLED, LOW);

  Serial.print("[");
  Serial.print(millis());
  Serial.println(" ms] YELLOW");

  delay(duration);
}

void showGreen(unsigned long duration) {
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, HIGH);

  Serial.print("[");
  Serial.print(millis());
  Serial.println(" ms] GREEN");

  delay(duration);
}

void loop() {

  if (digitalRead(buttonPin) == HIGH) {
    Serial.print("[");
    Serial.print(millis());
    Serial.println(" ms] Pedestrian Button Pressed");

    showRed(8000);
    return;
  }

  showRed(5000);

  if (digitalRead(buttonPin) == HIGH) {
    showRed(8000);
    return;
  }

  showYellow(2000);

  if (digitalRead(buttonPin) == HIGH) {
    showRed(8000);
    return;
  }

  showGreen(4000);
}