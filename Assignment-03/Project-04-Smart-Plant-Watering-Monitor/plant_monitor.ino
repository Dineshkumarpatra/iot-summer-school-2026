/*
  Project 4: Smart Plant Watering Monitor
  ESP32 + Capacitive Soil Moisture Sensor + DHT11 + Relay (pump) + OLED +
  Red/Green LEDs + Buzzer + manual override button

  Wiring:
    Soil sensor AOUT -> GPIO34 (ADC, 3.3V only — NEVER 5V on capacitive sensors)
    DHT11 DATA       -> GPIO4  (10kΩ pull-up to 3.3V)
    Relay IN1 (pump) -> GPIO26 (active LOW: LOW=ON, HIGH=OFF)
    Relay IN2        -> GPIO27 (unused in this project, held OFF)
    OLED SDA/SCL     -> GPIO21 / GPIO22 (I2C addr 0x3C)
    Push button       -> GPIO0, other leg to GND (INPUT_PULLUP)
    Red LED  (dry/pump active) -> GPIO25 (through 220Ω resistor)
    Green LED (optimal/ok)     -> GPIO2  (through 220Ω resistor)
    Buzzer            -> GPIO14 (short beep on entering DRY state)

  NOTE on the assignment's reference code: the snippet in the brief has
  `if(pumpOn && moisturePercent > 40) { pumpOn=false; digitalWrite(RELAY2, HIGH); }`
  — that's a typo in the brief (should be RELAY1, the same relay that was
  turned on). This sketch always drives the single pump relay correctly on
  GPIO26 for both the ON and OFF transitions.

  NOTE on GPIO0: this is the ESP32's boot-strapping pin. It works fine as a
  button in normal operation, but avoid holding it LOW at power-on/reset —
  that forces the board into flashing mode instead of booting your sketch.
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ---------------- PIN MAP ----------------
#define PIN_SOIL       34
#define PIN_DHT         4
#define PIN_RELAY1     26   // pump
#define PIN_RELAY2     27   // unused, held OFF
#define PIN_BUTTON      0
#define PIN_LED_RED    25
#define PIN_LED_GREEN   2
#define PIN_BUZZER     14

#define DHTTYPE DHT11
DHT dht(PIN_DHT, DHTTYPE);

#define OLED_W 128
#define OLED_H 64
Adafruit_SSD1306 display(OLED_W, OLED_H, &Wire, -1);

// ---------------- CALIBRATION ----------------
// Measure these for YOUR specific sensor (dry air vs fully submerged in water)
// and record the actual values in your README.
int CAL_DRY = 3200;  // raw ADC reading in dry air
int CAL_WET = 1200;  // raw ADC reading fully submerged in water

// ---------------- TIMING ----------------
const unsigned long SENSOR_INTERVAL_MS = 5UL * 60UL * 1000UL; // 5 minutes (spec)
// For bench-testing/demo, temporarily lower this, e.g. 5000UL (5s), then
// change it back to the 5-minute value before recording your final demo.
unsigned long lastSensorRead = 0;

const unsigned long OVERRIDE_WATER_MS = 5000; // manual override: force-water 5s
unsigned long overrideStartMs = 0;
bool overrideActive = false;

// ---------------- STATE ----------------
float g_temp = 0;
int   g_soilPct = 0;
bool  pumpOn = false;

enum SoilState { DRY, OPTIMAL, WET };
SoilState soilState = OPTIMAL;

// ---------------- HELPERS ----------------
const char* soilStateName(SoilState s) {
  switch (s) {
    case DRY: return "DRY";
    case WET: return "WET";
    default:  return "OPTIMAL";
  }
}

void setPump(bool on) {
  pumpOn = on;
  digitalWrite(PIN_RELAY1, on ? LOW : HIGH); // active LOW
  digitalWrite(PIN_LED_RED, on ? HIGH : LOW);
}

void beepBuzzer() {
  digitalWrite(PIN_BUZZER, HIGH);
  delay(150);
  digitalWrite(PIN_BUZZER, LOW);
}

// ---------------- SENSOR READ ----------------
void readSensors() {
  int raw = analogRead(PIN_SOIL);
  int pct = map(raw, CAL_DRY, CAL_WET, 0, 100);
  g_soilPct = constrain(pct, 0, 100);

  float t = dht.readTemperature();
  if (!isnan(t)) g_temp = t;

  Serial.printf("[SENSOR] raw=%d soil=%d%% temp=%.1fC\n", raw, g_soilPct, g_temp);
}

// ---------------- AUTOMATION ----------------
void applyAutomation() {
  SoilState prevState = soilState;

  if (g_soilPct < 30) soilState = DRY;
  else if (g_soilPct > 70) soilState = WET;
  else soilState = OPTIMAL;

  if (soilState == DRY && prevState != DRY) {
    beepBuzzer(); // audible alert on entering DRY
  }

  digitalWrite(PIN_LED_GREEN, (soilState == OPTIMAL || soilState == WET) ? HIGH : LOW);

  if (overrideActive) return; // manual override skips automatic hysteresis

  // Hysteresis: ON below 30%, OFF only above 40%
  if (!pumpOn && g_soilPct < 30) setPump(true);
  if (pumpOn  && g_soilPct > 40) setPump(false);
}

// ---------------- MANUAL OVERRIDE ----------------
void handleButton() {
  static bool lastBtn = false;
  bool btn = digitalRead(PIN_BUTTON) == LOW; // pressed

  if (btn && !lastBtn && !overrideActive) {
    overrideActive = true;
    overrideStartMs = millis();
    setPump(true);
    Serial.println("[OVERRIDE] Manual watering started (5s)");
  }
  lastBtn = btn;

  if (overrideActive && millis() - overrideStartMs > OVERRIDE_WATER_MS) {
    overrideActive = false;
    setPump(false);
    Serial.println("[OVERRIDE] Manual watering finished, resuming AUTO mode");
  }
}

// ---------------- OLED ----------------
void updateOled() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.printf("Soil: %d%%  Temp: %.0fC\n", g_soilPct, g_temp);
  display.printf("Status: %s%s\n", soilStateName(soilState), overrideActive ? " (OVERRIDE)" : "");
  display.println();

  // Moisture bar graph
  int barX = 0, barY = 40, barW = 120, barH = 14;
  display.drawRect(barX, barY, barW, barH, SSD1306_WHITE);
  int fillW = map(g_soilPct, 0, 100, 0, barW - 2);
  display.fillRect(barX + 1, barY + 1, fillW, barH - 2, SSD1306_WHITE);

  display.display();
}

// ---------------- SETUP / LOOP ----------------
void setup() {
  Serial.begin(115200);

  pinMode(PIN_RELAY1, OUTPUT);
  pinMode(PIN_RELAY2, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  digitalWrite(PIN_RELAY1, HIGH); // pump OFF (active LOW)
  digitalWrite(PIN_RELAY2, HIGH); // unused relay channel, held OFF
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  dht.begin();

  Wire.begin(21, 22);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("[OLED] init failed");
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("Plant Monitor booting...");
  display.display();

  readSensors();
  applyAutomation();
  lastSensorRead = millis();

  Serial.println("[BOOT] Smart Plant Watering Monitor ready");
}

void loop() {
  unsigned long now = millis();

  handleButton();

  if (now - lastSensorRead >= SENSOR_INTERVAL_MS) {
    readSensors();
    applyAutomation();
    updateOled();
    lastSensorRead = now;
  }

  // Keep OLED responsive to override state changes without waiting for
  // the next full sensor cycle
  static unsigned long lastOledRefresh = 0;
  if (now - lastOledRefresh >= 1000) {
    updateOled();
    lastOledRefresh = now;
  }
}
