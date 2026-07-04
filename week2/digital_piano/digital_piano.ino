/*
 * Project : Digital Piano
 * Author  : Dinesh Patra
 * Date    : 04-07-2026
 * Description:
 * 4-key digital piano with Major/Minor mode.
 * If two keys are pressed together, Sol (392 Hz) is played.
 */

const int buzzer = 8;

const int btnDo = A0;
const int btnRe = A1;
const int btnMi = A2;
const int btnFa = A3;
const int modeBtn = A4;

bool majorMode = true;
bool lastModeState = LOW;

void setup() {

  pinMode(btnDo, INPUT);
  pinMode(btnRe, INPUT);
  pinMode(btnMi, INPUT);
  pinMode(btnFa, INPUT);
  pinMode(modeBtn, INPUT);

  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);
  Serial.println("Digital Piano Started");
}

void loop() {

  // ---------- Mode Toggle ----------
  bool modeState = digitalRead(modeBtn);

  if (modeState == HIGH && lastModeState == LOW) {

    majorMode = !majorMode;

    if (majorMode)
      Serial.println("Major Mode");
    else
      Serial.println("Minor Mode");

    delay(250);
  }

  lastModeState = modeState;

  // ---------- Read Keys ----------
  bool doKey = digitalRead(btnDo);
  bool reKey = digitalRead(btnRe);
  bool miKey = digitalRead(btnMi);
  bool faKey = digitalRead(btnFa);

  int pressed = doKey + reKey + miKey + faKey;

  // ---------- Chord Detection ----------
  if (pressed >= 2) {
    tone(buzzer, 392);   // Sol
    return;
  }

  // ---------- Major / Minor Notes ----------
  if (doKey) {

    if (majorMode)
      tone(buzzer, 262);      // Do
    else
      tone(buzzer, 247);

  }

  else if (reKey) {

    if (majorMode)
      tone(buzzer, 294);      // Re
    else
      tone(buzzer, 277);

  }

  else if (miKey) {

    if (majorMode)
      tone(buzzer, 330);      // Mi
    else
      tone(buzzer, 311);

  }

  else if (faKey) {

    if (majorMode)
      tone(buzzer, 349);      // Fa
    else
      tone(buzzer, 330);

  }

  else {

    noTone(buzzer);

  }

  delay(10);
}