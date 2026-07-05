/*
 * Project : Ultrasonic Parking Sensor
 * Author  : Dinesh Patra
 * Description:
 * Parking distance alert using HC-SR04.
 */

const int trigPin = 9;
const int echoPin = 10;

const int redLED = 7;
const int yellowLED = 6;
const int buzzer = 8;

long duration;
float distance;

void setup() {

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;

  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 50) {

    Serial.println("SAFE");

    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    noTone(buzzer);

  }

  else if (distance > 20) {

    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, HIGH);

    tone(buzzer,1000);

    delay(500);

    noTone(buzzer);

    delay(500);

  }

  else if (distance > 10) {

    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);

    tone(buzzer,1000);

    delay(200);

    noTone(buzzer);

    delay(200);

  }

  else {

    digitalWrite(redLED,!digitalRead(redLED));
    digitalWrite(yellowLED,!digitalRead(yellowLED));

    tone(buzzer,1000);

    delay(100);

  }

}