/*
 * Project : Digital Piano
 * Author  : Dinesh Patra
 * Date    : 04-07-2026
 */

const int buzzer = 8;

const int btnDo = A0;
const int btnRe = A1;
const int btnMi = A2;
const int btnFa = A3;

void setup() {

  pinMode(btnDo, INPUT);
  pinMode(btnRe, INPUT);
  pinMode(btnMi, INPUT);
  pinMode(btnFa, INPUT);

  pinMode(buzzer, OUTPUT);
}

void loop() {

  if (digitalRead(btnDo))
    tone(buzzer,262);

  else if (digitalRead(btnRe))
    tone(buzzer,294);

  else if (digitalRead(btnMi))
    tone(buzzer,330);

  else if (digitalRead(btnFa))
    tone(buzzer,349);

  else
    noTone(buzzer);

}