/*
 * Project     : Serial Command Interface
 * Author      : Dinesh Patra
 * Date        : 04-07-2026
 * Description : Control the built-in LED using Serial Monitor commands.
 */

const int ledPin = 13;

String command = "";
int blinkCounter = 0;
bool ledState = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  Serial.println("=== Serial Command Interface ===");
  Serial.println("Commands:");
  Serial.println("LED_ON");
  Serial.println("LED_OFF");
  Serial.println("BLINK_X");
  Serial.println("STATUS");
  Serial.println("RESET");
}

void loop() {

  if (Serial.available()) {

    command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "LED_ON") {

      digitalWrite(ledPin, HIGH);
      ledState = true;
      Serial.println("LED ON");

    }

    else if (command == "LED_OFF") {

      digitalWrite(ledPin, LOW);
      ledState = false;
      Serial.println("LED OFF");

    }

    else if (command.startsWith("BLINK_")) {

      int count = command.substring(6).toInt();

      if (count >= 1 && count <= 9) {

        for (int i = 0; i < count; i++) {

          digitalWrite(ledPin, HIGH);
          delay(500);

          digitalWrite(ledPin, LOW);
          delay(500);

        }

        blinkCounter += count;

        Serial.print("Blinked ");
        Serial.print(count);
        Serial.println(" times");

      }

      else {

        Serial.println("ERROR: Blink count must be between 1 and 9");

      }

    }

    else if (command == "STATUS") {

      Serial.print("LED State : ");

      if (ledState)
        Serial.println("ON");
      else
        Serial.println("OFF");

      Serial.print("Blink Counter : ");
      Serial.println(blinkCounter);

    }

    else if (command == "RESET") {

      blinkCounter = 0;

      Serial.println("Blink counter reset");

    }

    else {

      Serial.println("ERROR: Unknown command");

    }

  }

}