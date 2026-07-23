#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define TRIG_PIN 5
#define ECHO_PIN 18

#define GREEN_LED 26
#define RED_LED 25
#define BUZZER 27

void setup() {

  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Wire.begin(21,22);

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    while(true);
  }

  display.clearDisplay();
}

void loop() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  float distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0,0);
  display.print("Distance:");

  display.setCursor(70,0);
  display.print(distance);
  display.print(" cm");

  if(distance < 15){

    digitalWrite(RED_LED,HIGH);
    digitalWrite(GREEN_LED,LOW);
    digitalWrite(BUZZER,HIGH);

    display.setCursor(0,30);
    display.println("PARKING:");
    display.setCursor(0,45);
    display.println("OCCUPIED");

    Serial.println("Parking Occupied");

  }

  else{

    digitalWrite(RED_LED,LOW);
    digitalWrite(GREEN_LED,HIGH);
    digitalWrite(BUZZER,LOW);

    display.setCursor(0,30);
    display.println("PARKING:");
    display.setCursor(0,45);
    display.println("AVAILABLE");

    Serial.println("Parking Available");

  }

  display.display();

  delay(500);

}