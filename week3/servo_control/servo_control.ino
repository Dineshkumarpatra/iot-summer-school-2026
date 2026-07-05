#include <Servo.h>

Servo servoMotor;

const int servoPin = 9;
const int potPin = A0;
const int buttonPin = 2;

void setup() {

  servoMotor.attach(servoPin);

  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);

}

void loop() {

  int value = analogRead(potPin);

  int angle = map(value,0,1023,0,180);

  servoMotor.write(angle);

  Serial.print("Servo Angle : ");
  Serial.println(angle);

  if(digitalRead(buttonPin)==LOW){

    for(int i=0;i<=180;i++){

      servoMotor.write(i);
      delay(10);

    }

    for(int i=180;i>=0;i--){

      servoMotor.write(i);
      delay(10);

    }

  }

  delay(100);

}