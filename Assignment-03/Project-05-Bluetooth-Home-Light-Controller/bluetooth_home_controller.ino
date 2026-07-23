// Project 05 - Bluetooth Home Light Controller (Wokwi Version)
// Commands are entered through Serial Monitor.

#define LIGHT1 26
#define LIGHT2 27
#define BUTTON1 0
#define BUTTON2 35
#define BUZZER 14
#define STATUS_LED 2

bool light1 = false;
bool light2 = false;

bool lastBtn1 = HIGH;
bool lastBtn2 = LOW;

unsigned long lastCommand = 0;
const unsigned long timeout = 1800000UL; // 30 minutes

void beep() {
  digitalWrite(BUZZER, HIGH);
  delay(50);
  digitalWrite(BUZZER, LOW);
}

void updateLights() {

  digitalWrite(LIGHT1, light1);
  digitalWrite(LIGHT2, light2);

}

void sendStatus() {

  Serial.print("L1:");
  Serial.print(light1 ? "ON" : "OFF");
  Serial.print("  L2:");
  Serial.println(light2 ? "ON" : "OFF");

}

void processCommand(char cmd) {

  switch (cmd) {

    case '1':
      light1 = true;
      break;

    case '2':
      light1 = false;
      break;

    case '3':
      light2 = true;
      break;

    case '4':
      light2 = false;
      break;

    case '5':
      light1 = true;
      light2 = true;
      break;

    case '6':
      light1 = false;
      light2 = false;
      break;

    case '?':
      sendStatus();
      return;

    default:
      return;
  }

  updateLights();
  beep();
  sendStatus();

  lastCommand = millis();
}

void setup() {

  Serial.begin(115200);

  pinMode(LIGHT1, OUTPUT);
  pinMode(LIGHT2, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT);

  pinMode(STATUS_LED, OUTPUT);

  digitalWrite(STATUS_LED, HIGH);

  updateLights();

  Serial.println();
  Serial.println("====================================");
  Serial.println("Bluetooth Home Light Controller");
  Serial.println("(Serial Monitor Simulation)");
  Serial.println("====================================");
  Serial.println("Commands:");
  Serial.println("1 -> Light 1 ON");
  Serial.println("2 -> Light 1 OFF");
  Serial.println("3 -> Light 2 ON");
  Serial.println("4 -> Light 2 OFF");
  Serial.println("5 -> ALL ON");
  Serial.println("6 -> ALL OFF");
  Serial.println("? -> Status");
  Serial.println();

  lastCommand = millis();
}

void loop() {

  if (Serial.available()) {

    char cmd = Serial.read();

    if (cmd != '\n' && cmd != '\r') {
      processCommand(cmd);
    }
  }

  bool b1 = digitalRead(BUTTON1);

  if (lastBtn1 == HIGH && b1 == LOW) {

    light1 = !light1;

    updateLights();

    beep();

    sendStatus();

    delay(200);
  }

  lastBtn1 = b1;

  bool b2 = digitalRead(BUTTON2);

  if (lastBtn2 == LOW && b2 == HIGH) {

    light2 = !light2;

    updateLights();

    beep();

    sendStatus();

    delay(200);
  }

  lastBtn2 = b2;

  if (millis() - lastCommand >= timeout) {

    if (light1 || light2) {

      light1 = false;
      light2 = false;

      updateLights();

      sendStatus();

      Serial.println("Auto Timeout - All Lights OFF");

      lastCommand = millis();
    }
  }
}