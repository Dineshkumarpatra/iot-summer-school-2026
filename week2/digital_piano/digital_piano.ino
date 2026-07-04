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