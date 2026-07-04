/*
 * Project : LED Blink
 * Author  : Dinesh Patra
 * Date    : 04-07-2026
 * Description:
 * Blink the built-in LED every 1 second.
 */

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);
  delay(1000);

  digitalWrite(13, LOW);
  delay(1000);
}