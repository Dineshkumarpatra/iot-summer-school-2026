/*
 * Project : Vending Machine State Machine
 * Author  : Dinesh Patra
 * Date    : 04-07-2026
 *
 * State Diagram
 *
 * IDLE
 *   |
 * Coin Button
 *   ↓
 * COIN_INSERTED
 *   |
 * Select Button
 *   ↓
 * ITEM_SELECTED
 *   |
 * Automatic
 *   ↓
 * DISPENSING
 *   |
 * Finished
 *   ↓
 * IDLE
 *
 * Cancel Button returns to IDLE.
 */

enum State {
  IDLE,
  COIN_INSERTED,
  ITEM_SELECTED,
  DISPENSING
};

State currentState = IDLE;

const int redLED = 8;
const int yellowLED = 9;
const int greenLED = 10;

const int coinBtn = 2;
const int selectBtn = 3;
const int cancelBtn = 4;

void setup() {

  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  pinMode(coinBtn, INPUT_PULLUP);
  pinMode(selectBtn, INPUT_PULLUP);
  pinMode(cancelBtn, INPUT_PULLUP);

  Serial.begin(9600);

  Serial.println("State : IDLE");
}

void showState() {

  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);

  switch(currentState) {

    case IDLE:
      digitalWrite(redLED, HIGH);
      break;

    case COIN_INSERTED:
      digitalWrite(yellowLED, HIGH);
      break;

    case ITEM_SELECTED:
      digitalWrite(greenLED, HIGH);
      break;

    case DISPENSING:
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, HIGH);
      break;
  }
}

void loop() {

  showState();

  if (digitalRead(cancelBtn) == LOW) {

    currentState = IDLE;
    Serial.println("State : IDLE");

    delay(300);

  }

  switch(currentState) {

    case IDLE:

      if (digitalRead(coinBtn) == LOW) {

        currentState = COIN_INSERTED;
        Serial.println("State : COIN_INSERTED");

        delay(300);
      }

      break;

    case COIN_INSERTED:

      if (digitalRead(selectBtn) == LOW) {

        currentState = ITEM_SELECTED;
        Serial.println("State : ITEM_SELECTED");

        delay(300);
      }

      break;

    case ITEM_SELECTED:

      currentState = DISPENSING;

      Serial.println("State : DISPENSING");

      delay(2000);

      currentState = IDLE;

      Serial.println("State : IDLE");

      break;

    case DISPENSING:

      break;

  }

}