
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <ArduinoJson.h>

#define DHTPIN 4
#define DHTTYPE DHT22

#define LDR_PIN 34

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

const char* ssid = ".";
const char* password = "12345678910";

WebServer server(80);

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float temperature = 0.0;
float humidity = 0.0;
float pressure = 0.0;
float altitude = 0.0;
int lightLevel = 0;

unsigned long sensorTimer = 0;
unsigned long wifiReconnectTimer = 0;

const unsigned long sensorInterval = 5000;
const unsigned long reconnectInterval = 30000;

String buildHTML();
String buildJSON();
void setup() {

  Serial.begin(115200);

  pinMode(LDR_PIN, INPUT);

  Wire.begin(21, 22);

  dht.begin();

  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 not found!");
    while (1);
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED initialization failed!");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Weather Station");
  display.println("Connecting WiFi...");
  display.display();

  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi Connected");
  display.print("SSID:");
  display.println(WiFi.SSID());
  display.print("IP:");
  display.println(WiFi.localIP());
  display.display();

  server.on("/", []() {
    server.send(200, "text/html", buildHTML());
  });

  server.on("/data", []() {
    server.send(200, "application/json", buildJSON());
  });

  server.begin();

  sensorTimer = millis();
  wifiReconnectTimer = millis();

  Serial.println("HTTP Server Started");
}
void loop() {

  server.handleClient();

  if (millis() - sensorTimer >= sensorInterval) {

    sensorTimer = millis();

    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    pressure = bmp.readPressure() / 100.0F;
    altitude = bmp.readAltitude(1013.25);

    lightLevel = analogRead(LDR_PIN);

    Serial.println("----------------------------");
    Serial.print("Temperature : ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity    : ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("Pressure    : ");
    Serial.print(pressure);
    Serial.println(" hPa");

    Serial.print("Altitude    : ");
    Serial.print(altitude);
    Serial.println(" m");

    Serial.print("Light Level : ");
    Serial.println(lightLevel);

    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);

    display.println("Weather Station");

    display.print("T: ");
    display.print(temperature, 1);
    display.println(" C");

    display.print("H: ");
    display.print(humidity, 1);
    display.println(" %");

    display.print("P: ");
    display.print(pressure, 1);
    display.println(" hPa");

    display.print("A: ");
    display.print(altitude, 1);
    display.println(" m");

    display.print("L: ");
    display.println(lightLevel);

    display.display();
  }

  if (WiFi.status() != WL_CONNECTED) {

    if (millis() - wifiReconnectTimer >= reconnectInterval) {

      wifiReconnectTimer = millis();

      Serial.println("Reconnecting WiFi...");

      WiFi.disconnect();
      WiFi.begin(ssid, password);
    }
  }
}
String buildHTML() {

  String color = "#2196F3";

  if (temperature >= 30) {
    color = "#FF9800";
  }

  String html = "";

  html += "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<meta http-equiv='refresh' content='10'>";
  html += "<title>ESP32 Weather Dashboard</title>";

  html += "<style>";
  html += "body{";
  html += "font-family:Arial;";
  html += "background:";
  html += color;
  html += ";";
  html += "margin:0;";
  html += "padding:20px;";
  html += "text-align:center;";
  html += "color:white;";
  html += "}";

  html += ".card{";
  html += "background:white;";
  html += "color:black;";
  html += "width:320px;";
  html += "margin:15px auto;";
  html += "padding:20px;";
  html += "border-radius:15px;";
  html += "box-shadow:0 4px 10px rgba(0,0,0,0.3);";
  html += "}";

  html += "h1{margin-bottom:25px;}";

  html += "</style>";
  html += "</head>";

  html += "<body>";

  html += "<h1>ESP32 Wi-Fi Weather Dashboard</h1>";

  html += "<div class='card'>";
  html += "<h2>Temperature</h2>";
  html += "<h3>";
  html += String(temperature,1);
  html += " &deg;C</h3>";
  html += "</div>";

  html += "<div class='card'>";
  html += "<h2>Humidity</h2>";
  html += "<h3>";
  html += String(humidity,1);
  html += " %</h3>";
  html += "</div>";

  html += "<div class='card'>";
  html += "<h2>Pressure</h2>";
  html += "<h3>";
  html += String(pressure,1);
  html += " hPa</h3>";
  html += "</div>";

  html += "<div class='card'>";
  html += "<h2>Altitude</h2>";
  html += "<h3>";
  html += String(altitude,1);
  html += " m</h3>";
  html += "</div>";

  html += "<div class='card'>";
  html += "<h2>Light Level</h2>";
  html += "<h3>";
  html += String(lightLevel);
  html += "</h3>";
  html += "</div>";

  html += "<div class='card'>";
  html += "<b>Connected Wi-Fi</b><br>";
  html += WiFi.SSID();
  html += "<br><br>";

  html += "<b>ESP32 IP Address</b><br>";
  html += WiFi.localIP().toString();
  html += "</div>";

  html += "<p>Auto Refresh Every 10 Seconds</p>";

  html += "</body>";
  html += "</html>";

  return html;
}
String buildJSON() {

  JsonDocument doc;

  doc["temp"] = temperature;
  doc["humidity"] = humidity;
  doc["pressure"] = pressure;
  doc["altitude"] = altitude;
  doc["light"] = lightLevel;

  String json;

  serializeJson(doc, json);

  return json;
}