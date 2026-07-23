/*
  Project 10: Full IoT Home Automation Hub
  ESP32 + DHT11 + Gas Sensor + PIR + LDR + 2-Channel Relay + OLED + Buzzer + LEDs + Buttons

  Wiring (per diagram.json):
    DHT   data       -> GPIO15
    GAS   AOUT        -> GPIO35
    LDR   AO          -> GPIO34
    PIR   OUT         -> GPIO13
    RELAY1 IN (FAN)   -> GPIO26   (active LOW: LOW=ON, HIGH=OFF)
    RELAY2 IN (LIGHT) -> GPIO27   (active LOW)
    BUZZER            -> GPIO14
    LED_RED  (gas alert)   -> GPIO25
    LED_GREEN(system OK)   -> GPIO2
    BTN_FAN override   -> GPIO0   (INPUT_PULLUP)
    BTN_LIGHT override -> GPIO32  (INPUT_PULLUP)
    OLED SDA -> GPIO21, SCL -> GPIO22 (I2C addr 0x3C)

  NOTE: the Wokwi sim part "wokwi-dht22" is used for the DHT footprint (Wokwi has
  no DHT11 timing model), so DHTTYPE is set to DHT22 below for the simulator.
  On real hardware with an actual DHT11, just change DHTTYPE to DHT11.

  Required libraries (Arduino Library Manager names) — see libraries.txt:
    DHT sensor library, Adafruit Unified Sensor, Adafruit SSD1306, Adafruit GFX Library,
    PubSubClient
  (WebServer and Preferences ship with the ESP32 board core — no install needed.
   We use the built-in synchronous WebServer instead of ESPAsyncWebServer/AsyncTCP
   because those two conflict with newer ESP32 core mbedtls headers in Wokwi's
   build environment.)
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <WebServer.h>
#include <Preferences.h>

// ---------------- USER CONFIG ----------------
const char* WIFI_SSID      = "Wokwi-GUEST";      // Wokwi's built-in open sim network
const char* WIFI_PASSWORD  = "";
const char* MQTT_BROKER    = "broker.hivemq.com";
const int   MQTT_PORT      = 1883;
const char* MQTT_TOPIC     = "iitjammu/dinesh/home";   // <-- replace [yourname] with your name
const char* MQTT_CLIENT_ID = "esp32-home-hub-dinesh";

// ---------------- PIN MAP ----------------
#define PIN_DHT         15
#define PIN_GAS         35
#define PIN_LDR         34
#define PIN_PIR         13
#define PIN_RELAY_FAN   26
#define PIN_RELAY_LIGHT 27
#define PIN_BUZZER      14
#define PIN_LED_RED     25
#define PIN_LED_GREEN   2
#define PIN_BTN_FAN     0
#define PIN_BTN_LIGHT   32

#define DHTTYPE DHT22   // Wokwi sim uses DHT22 protocol; swap to DHT11 for real hardware
DHT dht(PIN_DHT, DHTTYPE);

#define OLED_W 128
#define OLED_H 64
Adafruit_SSD1306 display(OLED_W, OLED_H, &Wire, -1);

WiFiClient espClient;
PubSubClient mqttClient(espClient);
WebServer server(80);
Preferences prefs;

// ---------------- STATE ----------------
float g_temp = 0, g_hum = 0;
int   g_gasPct = 0, g_ldrPct = 0;
bool  g_pir = false;

bool fanOn = false;
bool lightOn = false;
bool gasAlert = false;

bool fanManual = false, lightManual = false;
unsigned long fanManualUntil = 0, lightManualUntil = 0;
const unsigned long MANUAL_OVERRIDE_MS = 10UL * 60UL * 1000UL; // 10 min

unsigned long lastMotionMs = 0;
const unsigned long NO_MOTION_TIMEOUT_MS = 3UL * 60UL * 1000UL; // 3 min

unsigned long lastSensorRead = 0;
const unsigned long SENSOR_INTERVAL_MS = 5000;

unsigned long lastMqttPublish = 0;
const unsigned long MQTT_INTERVAL_MS = 30000;

unsigned long lastOledSwitch = 0;
const unsigned long OLED_INTERVAL_MS = 5000;
int oledScreen = 0;

// factory reset combo (hold both buttons 5s)
unsigned long bothBtnPressStart = 0;
bool bothBtnHeld = false;
const unsigned long FACTORY_RESET_HOLD_MS = 5000;

// ---------------- HELPERS ----------------
String uptimeString() {
  unsigned long s = millis() / 1000;
  unsigned long days = s / 86400; s %= 86400;
  unsigned long hrs  = s / 3600;  s %= 3600;
  unsigned long mins = s / 60;
  char buf[32];
  snprintf(buf, sizeof(buf), "%lud:%02luh:%02lum", days, hrs, mins);
  return String(buf);
}

void setRelayFan(bool on) {
  fanOn = on;
  digitalWrite(PIN_RELAY_FAN, on ? LOW : HIGH); // active LOW
}

void setRelayLight(bool on) {
  lightOn = on;
  digitalWrite(PIN_RELAY_LIGHT, on ? LOW : HIGH); // active LOW
}

void allRelaysOff() {
  setRelayFan(false);
  setRelayLight(false);
}

// ---------------- SENSOR READ ----------------
void readSensors() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (!isnan(h)) g_hum = h;
  if (!isnan(t)) g_temp = t;

  int gasRaw = analogRead(PIN_GAS);          // 0-4095 (12-bit ADC)
  g_gasPct = map(gasRaw, 0, 4095, 0, 100);

  int ldrRaw = analogRead(PIN_LDR);          // 0-4095
  g_ldrPct = map(ldrRaw, 0, 4095, 0, 100);   // higher = brighter (check/invert on real HW)

  bool motion = digitalRead(PIN_PIR) == HIGH;
  g_pir = motion;
  if (motion) lastMotionMs = millis();
}

// ---------------- AUTOMATION ----------------
void applyAutomationRules() {
  unsigned long now = millis();

  if (fanManual && now > fanManualUntil) fanManual = false;
  if (lightManual && now > lightManualUntil) lightManual = false;

  // ---- Rule 3: Gas safety shutdown (highest priority, overrides manual too) ----
  if (g_gasPct > 60) {
    gasAlert = true;
    digitalWrite(PIN_LED_RED, HIGH);
    digitalWrite(PIN_BUZZER, HIGH);
    digitalWrite(PIN_LED_GREEN, LOW);
    allRelaysOff();
    fanManual = false;
    lightManual = false;
    publishAlertMQTT();
    return;
  } else {
    gasAlert = false;
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_BUZZER, LOW);
    digitalWrite(PIN_LED_GREEN, HIGH);
  }

  // ---- Rule 1: Fan hysteresis ----
  if (!fanManual) {
    if (!fanOn && g_temp > 32.0) setRelayFan(true);
    if (fanOn  && g_temp < 28.0) setRelayFan(false);
  }

  // ---- Rule 2: Light — LDR + PIR ----
  if (!lightManual) {
    bool motionRecent = (millis() - lastMotionMs) < NO_MOTION_TIMEOUT_MS;
    if (!lightOn && g_ldrPct < 20 && g_pir) setRelayLight(true);
    if (lightOn && (g_ldrPct > 60 || !motionRecent)) setRelayLight(false);
  }
}

// ---------------- BUTTONS ----------------
void handleButtons() {
  bool fanBtn   = digitalRead(PIN_BTN_FAN)   == LOW; // pressed
  bool lightBtn = digitalRead(PIN_BTN_LIGHT) == LOW;

  if (fanBtn && lightBtn) {
    if (!bothBtnHeld) { bothBtnHeld = true; bothBtnPressStart = millis(); }
    else if (millis() - bothBtnPressStart > FACTORY_RESET_HOLD_MS) {
      factoryReset();
      bothBtnHeld = false;
    }
    return;
  } else {
    bothBtnHeld = false;
  }

  static bool lastFanBtn = false, lastLightBtn = false;
  if (fanBtn && !lastFanBtn) {
    fanManual = true;
    fanManualUntil = millis() + MANUAL_OVERRIDE_MS;
    setRelayFan(!fanOn);
    prefs.putBool("fanOn", fanOn);
    prefs.putBool("fanManual", true);
  }
  if (lightBtn && !lastLightBtn) {
    lightManual = true;
    lightManualUntil = millis() + MANUAL_OVERRIDE_MS;
    setRelayLight(!lightOn);
    prefs.putBool("lightOn", lightOn);
    prefs.putBool("lightManual", true);
  }
  lastFanBtn = fanBtn;
  lastLightBtn = lightBtn;
}

void factoryReset() {
  prefs.clear();
  fanManual = false; lightManual = false;
  setRelayFan(false); setRelayLight(false);
  for (int i = 0; i < 6; i++) {
    digitalWrite(PIN_LED_RED, i % 2);
    delay(150);
  }
  digitalWrite(PIN_LED_RED, LOW);
}

// ---------------- OLED ----------------
void updateOled() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (oledScreen == 0) {
    display.println("SCREEN 1: CLIMATE");
    display.println();
    display.print("Temp : "); display.print(g_temp, 1); display.println(" C");
    display.print("Hum  : "); display.print(g_hum, 1); display.println(" %");
  } else if (oledScreen == 1) {
    display.println("SCREEN 2: SAFETY/LIGHT");
    display.println();
    display.print("Gas  : "); display.print(g_gasPct); display.println(" %");
    display.print("PIR  : "); display.println(g_pir ? "MOTION" : "clear");
    display.print("LDR  : "); display.print(g_ldrPct); display.println(" %");
  } else {
    display.println("SCREEN 3: SYSTEM");
    display.println();
    display.print("Fan  : "); display.println(fanOn ? "ON" : "OFF");
    display.print("Light: "); display.println(lightOn ? "ON" : "OFF");
    display.print("Up   : "); display.println(uptimeString());
  }
  display.display();
}

// ---------------- WIFI / MQTT ----------------
void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(300);
    Serial.print(".");
  }
  Serial.println();
  Serial.println(WiFi.status() == WL_CONNECTED
                    ? "WiFi connected: " + WiFi.localIP().toString()
                    : "WiFi FAILED - continuing offline");
}

void connectMQTT() {
  if (WiFi.status() != WL_CONNECTED) return;
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  if (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT...");
    if (mqttClient.connect(MQTT_CLIENT_ID)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc="); Serial.println(mqttClient.state());
    }
  }
}

String buildJsonPayload(bool alert) {
  String json = "{";
  json += "\"temp\":" + String(g_temp, 1) + ",";
  json += "\"humidity\":" + String(g_hum, 1) + ",";
  json += "\"gas\":" + String(g_gasPct) + ",";
  json += "\"pir\":" + String(g_pir ? "true" : "false") + ",";
  json += "\"light\":" + String(g_ldrPct) + ",";
  json += "\"fan\":" + String(fanOn ? "true" : "false") + ",";
  json += "\"light_relay\":" + String(lightOn ? "true" : "false") + ",";
  json += "\"alert\":" + String(alert ? "true" : "false");
  json += "}";
  return json;
}

void publishMQTT() {
  if (!mqttClient.connected()) connectMQTT();
  if (!mqttClient.connected()) return;
  mqttClient.loop();
  String payload = buildJsonPayload(gasAlert);
  mqttClient.publish(MQTT_TOPIC, payload.c_str());
}

void publishAlertMQTT() {
  if (!mqttClient.connected()) connectMQTT();
  if (!mqttClient.connected()) return;
  String payload = buildJsonPayload(true);
  mqttClient.publish(MQTT_TOPIC, payload.c_str());
}

// ---------------- WEB DASHBOARD ----------------
String htmlDashboard() {
  String html = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='5'>";
  html += "<title>Home Hub</title><style>body{font-family:sans-serif;background:#111;color:#eee;padding:20px}";
  html += "h1{color:#4fc3f7}.card{background:#222;padding:12px 18px;margin:10px 0;border-radius:8px}";
  html += ".on{color:#4caf50;font-weight:bold}.off{color:#888}.alert{color:#f44336;font-weight:bold}";
  html += "button{padding:8px 16px;margin-right:8px;border:none;border-radius:6px;cursor:pointer}</style></head><body>";
  html += "<h1>ESP32 Home Automation Hub</h1>";

  html += "<div class='card'>Temp: " + String(g_temp, 1) + " C &nbsp; Humidity: " + String(g_hum, 1) + " %</div>";
  html += "<div class='card'>Gas: " + String(g_gasPct) + "% " + (gasAlert ? "<span class='alert'>ALERT!</span>" : "") + "</div>";
  html += "<div class='card'>Light (LDR): " + String(g_ldrPct) + "% &nbsp; Motion: " + String(g_pir ? "YES" : "no") + "</div>";
  html += "<div class='card'>Fan: <span class='" + String(fanOn ? "on'>ON" : "off'>OFF") + "</span>" +
          (fanManual ? " (manual)" : " (auto)") +
          " <form action='/toggleFan' method='POST' style='display:inline'><button>Toggle</button></form></div>";
  html += "<div class='card'>Light Relay: <span class='" + String(lightOn ? "on'>ON" : "off'>OFF") + "</span>" +
          (lightManual ? " (manual)" : " (auto)") +
          " <form action='/toggleLight' method='POST' style='display:inline'><button>Toggle</button></form></div>";
  html += "<div class='card'>Uptime: " + uptimeString() + "</div>";
  html += "</body></html>";
  return html;
}

void setupWebServer() {
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", htmlDashboard());
  });

  server.on("/data", HTTP_GET, []() {
    server.send(200, "application/json", buildJsonPayload(gasAlert));
  });

  server.on("/toggleFan", HTTP_POST, []() {
    fanManual = true;
    fanManualUntil = millis() + MANUAL_OVERRIDE_MS;
    setRelayFan(!fanOn);
    prefs.putBool("fanOn", fanOn);
    prefs.putBool("fanManual", true);
    server.sendHeader("Location", "/");
    server.send(303);
  });

  server.on("/toggleLight", HTTP_POST, []() {
    lightManual = true;
    lightManualUntil = millis() + MANUAL_OVERRIDE_MS;
    setRelayLight(!lightOn);
    prefs.putBool("lightOn", lightOn);
    prefs.putBool("lightManual", true);
    server.sendHeader("Location", "/");
    server.send(303);
  });

  server.begin();
}

// ---------------- SETUP / LOOP ----------------
void setup() {
  Serial.begin(115200);

  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_RELAY_FAN, OUTPUT);
  pinMode(PIN_RELAY_LIGHT, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_BTN_FAN, INPUT_PULLUP);
  pinMode(PIN_BTN_LIGHT, INPUT_PULLUP);

  allRelaysOff();
  digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_LED_GREEN, HIGH);

  dht.begin();

  Wire.begin(21, 22);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 init failed");
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("Home Hub booting...");
  display.display();

  prefs.begin("homehub", false);
  fanManual   = prefs.getBool("fanManual", false);
  lightManual = prefs.getBool("lightManual", false);
  if (fanManual)   setRelayFan(prefs.getBool("fanOn", false));
  if (lightManual) setRelayLight(prefs.getBool("lightOn", false));
  if (fanManual)   fanManualUntil   = millis() + MANUAL_OVERRIDE_MS;
  if (lightManual) lightManualUntil = millis() + MANUAL_OVERRIDE_MS;

  connectWiFi();
  setupWebServer();
  connectMQTT();

  lastSensorRead = millis();
  lastMqttPublish = millis();
  lastOledSwitch = millis();
}

void loop() {
  unsigned long now = millis();

  server.handleClient();

  handleButtons();

  if (now - lastSensorRead >= SENSOR_INTERVAL_MS) {
    readSensors();
    applyAutomationRules();
    lastSensorRead = now;
  }

  if (now - lastMqttPublish >= MQTT_INTERVAL_MS) {
    publishMQTT();
    lastMqttPublish = now;
  } else if (mqttClient.connected()) {
    mqttClient.loop();
  }

  if (now - lastOledSwitch >= OLED_INTERVAL_MS) {
    oledScreen = (oledScreen + 1) % 3;
    updateOled();
    lastOledSwitch = now;
  }
}
