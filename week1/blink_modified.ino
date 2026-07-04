/*
 * Project : Modified LED Blink
 * Author  : Dinesh Patra
 * Description:
 * Blink LED three times rapidly, then pause for 2 seconds.
 */

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }

  delay(2000);
}
