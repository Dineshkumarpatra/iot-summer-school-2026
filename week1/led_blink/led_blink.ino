/*
 * Project     : LED Blink
 * Author      : Dinesh Patra
 * Date        : 04-07-2026
 * Description : LED blink with serial counter and potentiometer speed control.
 */

int blinkCount = 0;
const int potPin = A0;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {

  int value = analogRead(potPin);
  int blinkDelay = map(value, 0, 1023, 100, 1000);

  digitalWrite(13, HIGH);
  delay(blinkDelay);

  digitalWrite(13, LOW);
  delay(blinkDelay);

  blinkCount++;

  Serial.print("Blink count: ");
  Serial.print(blinkCount);
  Serial.print(" | Delay: ");
  Serial.println(blinkDelay);
}