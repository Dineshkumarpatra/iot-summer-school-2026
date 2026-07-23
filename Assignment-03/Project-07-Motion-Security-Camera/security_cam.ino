/*
  Project 7: Motion-Activated Security Camera
  ESP32-CAM (AI-Thinker) + PIR HC-SR501 + Buzzer + Red LED

  Wiring:
    PIR OUT   -> GPIO13
    Red LED   -> GPIO2  (through 220Ω resistor)
    Buzzer(+) -> GPIO14
    Buzzer(-) -> GND

  Programming: connect GPIO0 to GND BEFORE power-up to enter flash mode,
  upload, then DISCONNECT GPIO0 from GND and reset before running normally.
  See README for full FTDI wiring instructions.

  Modes:
    STANDBY - camera idle, no streaming, waiting for PIR trigger
    ALERT   - motion detected: LED on, buzzer beep, MJPEG stream available,
              event logged with millis() timestamp. Returns to STANDBY
              after 60s of no further motion.

  IMPORTANT PIN CONFLICT (bonus SD card feature):
    SD_MMC on AI-Thinker boards fixes CLK=GPIO14, CMD=GPIO15, D0=GPIO2 even
    in 1-bit mode. That collides directly with the LED (GPIO2) and buzzer
    (GPIO14) pins used above. The base project (ENABLE_SD_LOGGING = false)
    works exactly as specified. If you enable SD logging, you must either:
      (a) move the LED and buzzer to different free GPIOs (e.g. GPIO12,
          GPIO13 is taken by PIR so pick another), or
      (b) accept that the LED/buzzer won't function while SD is mounted.
    This is a real AI-Thinker hardware limitation, not a code bug.
*/

#include "esp_camera.h"
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// ---------------- BONUS TOGGLE ----------------
#define ENABLE_SD_LOGGING false   // see pin-conflict warning above before enabling

#if ENABLE_SD_LOGGING
  #include "SD_MMC.h"
  #include "FS.h"
  int motionEventCount = 0;
#endif

// ---------------- AI-THINKER CAMERA PIN MAP ----------------
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ---------------- USER CONFIG ----------------
const char* WIFI_SSID     = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// ---------------- PERIPHERAL PINS ----------------
#define PIN_PIR    13
#define PIN_LED     2
#define PIN_BUZZER 14

// ---------------- STATE ----------------
enum SystemMode { STANDBY, ALERT };
SystemMode mode = STANDBY;

const unsigned long ALERT_TIMEOUT_MS = 60000; // 60s no-motion -> standby
unsigned long lastMotionMs = 0;

#define LOG_SIZE 5
unsigned long eventLog[LOG_SIZE];
int eventLogHead = 0;
int eventLogCount = 0;

volatile bool motionFlag = false;

WiFiServer server(80);

// ---------------- PIR INTERRUPT ----------------
void IRAM_ATTR pirISR() {
  motionFlag = true;
}

// ---------------- EVENT LOG ----------------
void addEvent(unsigned long t) {
  eventLog[eventLogHead] = t;
  eventLogHead = (eventLogHead + 1) % LOG_SIZE;
  if (eventLogCount < LOG_SIZE) eventLogCount++;
}

// Returns up to LOG_SIZE timestamps, most recent first
void getRecentEvents(unsigned long *out, int &count) {
  count = eventLogCount;
  for (int i = 0; i < count; i++) {
    int idx = (eventLogHead - 1 - i + LOG_SIZE) % LOG_SIZE;
    out[i] = eventLog[idx];
  }
}

// ---------------- BUZZER ----------------
void beepBuzzer() {
  digitalWrite(PIN_BUZZER, HIGH);
  delay(200);
  digitalWrite(PIN_BUZZER, LOW);
}

// ---------------- SD SNAPSHOT (bonus) ----------------
#if ENABLE_SD_LOGGING
void saveSnapshotToSD() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("[SD] Camera capture failed");
    return;
  }
  motionEventCount++;
  char filename[32];
  snprintf(filename, sizeof(filename), "/motion_%03d.jpg", motionEventCount);

  File file = SD_MMC.open(filename, FILE_WRITE);
  if (!file) {
    Serial.printf("[SD] Failed to open %s for writing\n", filename);
  } else {
    file.write(fb->buf, fb->len);
    file.close();
    Serial.printf("[SD] Saved %s (%u bytes)\n", filename, fb->len);
  }
  esp_camera_fb_return(fb);
}
#endif

// ---------------- MODE TRANSITIONS ----------------
void enterAlert() {
  if (mode != ALERT) {
    mode = ALERT;
    Serial.println("[STATE] STANDBY -> ALERT");
  }
  lastMotionMs = millis();
  digitalWrite(PIN_LED, HIGH);
  addEvent(millis());
  Serial.printf("[EVENT] Motion detected at t=%lums\n", millis());
  beepBuzzer();

#if ENABLE_SD_LOGGING
  saveSnapshotToSD();
#endif
}

void checkStandbyTimeout() {
  if (mode == ALERT && millis() - lastMotionMs > ALERT_TIMEOUT_MS) {
    mode = STANDBY;
    digitalWrite(PIN_LED, LOW);
    Serial.println("[STATE] ALERT -> STANDBY");
  }
}

// ---------------- CAMERA INIT ----------------
bool initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk  = XCLK_GPIO_NUM;
  config.pin_pclk  = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href  = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn  = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format  = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size   = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count      = 2;
  } else {
    config.frame_size   = FRAMESIZE_SVGA;
    config.jpeg_quality = 15;
    config.fb_count      = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("[CAM] Init failed: 0x%x\n", err);
    return false;
  }
  return true;
}

// ---------------- WEB PAGES ----------------
String buildDashboardHtml() {
  String html = "<!DOCTYPE html><html><head><title>Security Cam</title>";
  html += "<style>body{font-family:sans-serif;background:#111;color:#eee;padding:20px}";
  html += "h1{color:#4fc3f7}.card{background:#222;padding:12px 18px;margin:10px 0;border-radius:8px}";
  html += ".alert{color:#f44336;font-weight:bold}.standby{color:#888}";
  html += "img{max-width:100%;border-radius:8px;border:2px solid #444}</style></head><body>";
  html += "<h1>ESP32-CAM Security Monitor</h1>";
  html += "<div class='card'>Status: <span id='mode'>...</span></div>";
  html += "<div class='card'><img id='stream' src='/stream'></div>";
  html += "<div class='card'>Last 5 detections (ms since boot):<ul id='log'></ul></div>";
  html += "<script>";
  html += "function poll(){fetch('/status').then(r=>r.json()).then(d=>{";
  html += "document.getElementById('mode').className=(d.mode=='ALERT')?'alert':'standby';";
  html += "document.getElementById('mode').innerText=d.mode;";
  html += "let ul=document.getElementById('log');ul.innerHTML='';";
  html += "d.events.forEach(t=>{let li=document.createElement('li');li.innerText=t+' ms';ul.appendChild(li);});";
  html += "});}setInterval(poll,2000);poll();";
  html += "</script></body></html>";
  return html;
}

void sendDashboard(WiFiClient &client) {
  String html = buildDashboardHtml();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.printf("Content-Length: %d\r\n\r\n", html.length());
  client.print(html);
}

void sendStatusJson(WiFiClient &client) {
  unsigned long recent[LOG_SIZE];
  int count = 0;
  getRecentEvents(recent, count);

  String json = "{\"mode\":\"";
  json += (mode == ALERT) ? "ALERT" : "STANDBY";
  json += "\",\"events\":[";
  for (int i = 0; i < count; i++) {
    json += String(recent[i]);
    if (i < count - 1) json += ",";
  }
  json += "]}";

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.printf("Content-Length: %d\r\n\r\n", json.length());
  client.print(json);
}

void sendStandbyPlaceholder(WiFiClient &client) {
  String msg = "System is in STANDBY - no motion detected. Stream is only active during an ALERT.";
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println("Connection: close");
  client.printf("Content-Length: %d\r\n\r\n", msg.length());
  client.print(msg);
}

// MJPEG multipart stream — only serves frames while in ALERT mode
void streamMJPEG(WiFiClient &client) {
  if (mode != ALERT) {
    sendStandbyPlaceholder(client);
    return;
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: multipart/x-mixed-replace; boundary=frame");
  client.println();

  while (client.connected() && mode == ALERT) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("[CAM] Frame capture failed");
      break;
    }

    client.println("--frame");
    client.println("Content-Type: image/jpeg");
    client.printf("Content-Length: %u\r\n\r\n", fb->len);
    client.write(fb->buf, fb->len);
    client.println();

    esp_camera_fb_return(fb);

    checkStandbyTimeout(); // stop streaming as soon as we time out to STANDBY
    delay(100); // ~10 fps, keeps things light on ESP32-CAM
  }
}

// ---------------- HTTP REQUEST HANDLING ----------------
void handleClient() {
  WiFiClient client = server.available();
  if (!client) return;

  String reqLine = client.readStringUntil('\r');
  client.readStringUntil('\n');
  // consume remaining headers
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line.length() <= 1) break;
  }

  if (reqLine.indexOf("GET /stream") >= 0) {
    streamMJPEG(client);
  } else if (reqLine.indexOf("GET /status") >= 0) {
    sendStatusJson(client);
  } else {
    sendDashboard(client);
  }

  delay(1);
  client.stop();
}

// ---------------- WIFI ----------------
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
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi connected. Dashboard: http://");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi FAILED - check credentials");
  }
}

// ---------------- SETUP / LOOP ----------------
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector (camera current spikes)

  Serial.begin(115200);
  Serial.println("\n[BOOT] Motion-Activated Security Camera starting...");

  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  attachInterrupt(digitalPinToInterrupt(PIN_PIR), pirISR, RISING);

  if (!initCamera()) {
    Serial.println("[FATAL] Camera init failed. Halting.");
    while (true) delay(1000);
  }

#if ENABLE_SD_LOGGING
  if (!SD_MMC.begin("/sdcard", true)) { // true = 1-bit mode
    Serial.println("[SD] Mount failed - check card / pin conflicts noted at top of file");
  } else {
    Serial.println("[SD] Mounted OK");
  }
#endif

  connectWiFi();
  server.begin();

  Serial.println("[STATE] Booted into STANDBY");
}

void loop() {
  if (motionFlag) {
    motionFlag = false;
    enterAlert();
  }

  checkStandbyTimeout();
  handleClient();
}
