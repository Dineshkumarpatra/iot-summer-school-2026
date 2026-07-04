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

  bool doKey = digitalRead(btnDo);
  bool reKey = digitalRead(btnRe);
  bool miKey = digitalRead(btnMi);
  bool faKey = digitalRead(btnFa);

  int pressed = doKey + reKey + miKey + faKey;

  if (pressed >= 2) {

    tone(buzzer,392);

    return;

  }

  if (doKey)
    tone(buzzer,262);

  else if (reKey)
    tone(buzzer,294);

  else if (miKey)
    tone(buzzer,330);

  else if (faKey)
    tone(buzzer,349);

  else
    noTone(buzzer);

}