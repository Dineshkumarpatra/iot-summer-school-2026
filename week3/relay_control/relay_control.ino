#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const int relayPin = 8;
const int buttonPin = 3;

bool relayState = false;
bool manualOverride = false;
bool lastButton = HIGH;

void setup() {

  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);

  dht.begin();
}

void loop() {

  float temp = dht.readTemperature();

  bool button = digitalRead(buttonPin);

  if(button == LOW && lastButton == HIGH){

    manualOverride = !manualOverride;

    relayState = !relayState;

    digitalWrite(relayPin, relayState);

    Serial.print("Manual Override | Temp: ");
    Serial.print(temp);
    Serial.print(" C | Relay: ");
    Serial.println(relayState ? "ON" : "OFF");

    delay(200);
  }

  lastButton = button;

  if(!manualOverride){

    if(temp > 32){

      relayState = true;

    }

    else if(temp < 28){

      relayState = false;

    }

    digitalWrite(relayPin, relayState);

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print(" C | Relay: ");
    Serial.println(relayState ? "ON" : "OFF");

  }

  delay(1000);

}