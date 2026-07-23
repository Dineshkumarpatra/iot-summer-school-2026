# Project 10 — Full IoT Home Automation Hub

ESP32-based smart hostel-room controller that reads four sensors, auto-controls
a fan and a light via relays, runs a gas safety shutdown, serves a live web
dashboard, publishes to MQTT, and cycles status on an OLED.

## 1. System Architecture

```
                         ┌───────────────────────────────┐
                         │            ESP32               │
                         │                                 │
  DHT11 ── data ────────►│ GPIO15                          │
  MQ-2  ── AOUT ─────────►│ GPIO35 (ADC)                    │
  LDR   ── divider ──────►│ GPIO34 (ADC)                    │
  PIR   ── OUT ───────────►│ GPIO13                          │
  BTN_FAN  ───────────────►│ GPIO0  (INPUT_PULLUP)           │
  BTN_LIGHT ──────────────►│ GPIO32 (INPUT_PULLUP)           │
                         │                                 │
                         │        ┌─────────────────────┐  │
                         │        │  applyAutomationRules│  │
                         │        │  (every 5s, non-     │  │
                         │        │   blocking millis)   │  │
                         │        └──────────┬──────────┘  │
                         │                   │              │
     RELAY1 (FAN)  ◄─────│ GPIO26 ◄──────────┤              │
     RELAY2 (LIGHT) ◄────│ GPIO27 ◄──────────┤              │
     BUZZER         ◄────│ GPIO14 ◄──────────┤              │
     LED_RED (alert)◄────│ GPIO25 ◄──────────┤              │
     LED_GREEN (OK) ◄────│ GPIO2  ◄──────────┘              │
                         │                                 │
                         │   OLED (I2C) ── SDA21/SCL22 ────┼──► 3 rotating screens
                         │                                 │
                         │   WiFi ──► WebServer :80 ───────┼──► Dashboard + toggle
                         │   WiFi ──► MQTT (HiveMQ) ────────┼──► iitjammu/dinesh/home
                         └───────────────────────────────┘
```

**Data flow:** sensors → global state → automation engine → relay/LED/buzzer
outputs, in parallel with OLED refresh (5s), MQTT publish (30s), and the web
dashboard which reads the same global state on every page load / `/data` call.

## 2. Automation Rules

| # | Rule | Condition (ON) | Condition (OFF) | Notes |
|---|------|-----------------|------------------|-------|
| 1 | Fan (Relay 1) | Temp > 32°C | Temp < 28°C | Hysteresis band prevents relay chatter around the threshold. Skipped while a manual override is active. |
| 2 | Light (Relay 2) | LDR < 20% **and** PIR motion detected | LDR > 60% (daylight) **or** no motion for 3 minutes | Skipped while a manual override is active. |
| 3 | Gas safety shutdown | MQ-2 > 60% | Automatically clears when gas drops back ≤ 60% | Highest priority — forces both relays OFF, fires buzzer + red LED, publishes an immediate MQTT alert, and **cancels any active manual override** so safety always wins. |

Sensors are read and rules re-evaluated every 5 seconds using a non-blocking
`millis()` check in `loop()` — no `delay()` calls block the web server, MQTT
loop, or OLED refresh.

## 3. Manual Override Logic

- Pressing **BTN_FAN** (GPIO0) or **BTN_LIGHT** (GPIO32) toggles that relay
  immediately and puts it into a 10-minute manual-override window
  (`fanManualUntil` / `lightManualUntil`, tracked via `millis()`).
- While a relay is in manual mode, the corresponding automation rule (Rule 1
  or Rule 2) is skipped entirely — the relay stays exactly where the user put
  it.
- After 10 minutes the override silently expires and the relay returns to
  automatic control on the next sensor cycle.
- The same toggle can be triggered from the web dashboard (`/toggleFan`,
  `/toggleLight` POST routes), which sets the same manual-override state.
- **Gas alert overrides everything**, including an active manual override —
  if MQ-2 crosses 60%, both relays are forced off and any manual override in
  progress is cancelled, so a resident can't accidentally leave the fan/light
  on during a gas event.
- **Factory reset combo**: holding both buttons together for 5 seconds clears
  all saved `Preferences` (fan/light manual state and last relay state) and
  blinks the red LED as confirmation — see Bonus section below.

## 4. Web Dashboard

- Served by the built-in synchronous `WebServer` library on port 80, polled
  via `server.handleClient()` on every `loop()` iteration (see Section 11 for
  why this replaces the `ESPAsyncWebServer` named in the original spec).
- `GET /` — full HTML dashboard: temperature, humidity, gas %, LDR %, motion
  state, fan/light status (with auto/manual tag), and uptime. Auto-refreshes
  every 5 seconds.
- `GET /data` — same data as raw JSON, for scripting/testing.
- `POST /toggleFan`, `POST /toggleLight` — manual relay toggle buttons on the
  dashboard hit these routes (302/303 redirect back to `/` after toggling).

## 5. MQTT

- Broker: `broker.hivemq.com:1883` (public test broker)
- Topic: `iitjammu/dinesh/home`
- Published every 30 seconds, plus immediately on a gas alert transition.
- Payload:
```json
{
  "temp": 29.5,
  "humidity": 55.0,
  "gas": 12,
  "pir": true,
  "light": 34,
  "fan": false,
  "light_relay": true,
  "alert": false
}
```

## 6. OLED — 3 Rotating Screens (5s each)

1. **Climate** — temperature + humidity
2. **Safety/Light** — gas %, PIR state, LDR %
3. **System** — fan/light relay state + uptime (`Nd:NNh:NNm`)

## 7. Bonus — Persistent Settings & Factory Reset

- Uses the ESP32 `Preferences` library (namespace `"homehub"`) to persist
  `fanManual`, `lightManual`, `fanOn`, `lightOn` across reboots — if a
  resident manually turned the fan on before a power cut, it comes back on
  in manual mode after reboot.
- Holding **both** override buttons for 5 seconds triggers `factoryReset()`:
  clears all saved preferences, forces both relays off, and blinks the red
  LED 3 times as visual confirmation.

## 8. Repo Structure

```
/home-assignments/p10-home-hub/
├── sketch.ino          # main firmware
├── diagram.json        # Wokwi wiring diagram
├── libraries.txt        # Arduino Library Manager dependencies for Wokwi build
├── README.md            # this file
├── demo-video-script.md  # shot-by-shot script used to record the demo
├── demo-video.mp4        # 2-3 min demo (all rules triggering)
└── screenshots/
    ├── dashboard.png     # web dashboard, all readings + relay states visible
    └── mqtt-explorer.png # MQTT Explorer showing JSON arriving on the topic
```

## 9. Pin Map

| Component | ESP32 Pin |
|---|---|
| DHT11 data | GPIO15 |
| MQ-2 AOUT | GPIO35 |
| LDR divider | GPIO34 |
| PIR OUT | GPIO13 |
| Relay 1 (fan) IN | GPIO26 (active LOW) |
| Relay 2 (light) IN | GPIO27 (active LOW) |
| Buzzer | GPIO14 |
| LED Red (gas alert) | GPIO25 |
| LED Green (system OK) | GPIO2 |
| Button (fan override) | GPIO0 |
| Button (light override) | GPIO32 |
| OLED SDA / SCL | GPIO21 / GPIO22 (I2C 0x3C) |

## 10. Required Libraries

Install via Arduino Library Manager (also listed in `libraries.txt` for the
Wokwi build):

- `DHT sensor library`
- `Adafruit Unified Sensor`
- `Adafruit SSD1306`
- `Adafruit GFX Library`
- `PubSubClient`

`WebServer` and `Preferences` ship with the ESP32 board core — no install
needed.

## 11. Engineering Deviations from the Original Spec

- **Web server library.** The spec calls for `ESPAsyncWebServer`. That
  library (together with its `AsyncTCP` dependency) fails to compile in
  Wokwi's build environment — it calls deprecated mbedtls functions
  (`mbedtls_md5_starts_ret` etc.) that were renamed in the ESP32 core version
  Wokwi currently builds against, and a duplicate-library conflict on
  `AsyncTCP.h` compounds the issue. This is swapped for the ESP32 core's
  built-in synchronous `WebServer` library instead. It serves the identical
  dashboard, JSON endpoint, and toggle routes — the only functional
  difference is that it's polled once per `loop()` iteration
  (`server.handleClient()`) rather than event-driven, which has no visible
  effect here since nothing else in `loop()` blocks.

## 12. Known Simulation Notes

- Wokwi has no DHT11 timing model, so `diagram.json` uses the `wokwi-dht22`
  part and the sketch's `DHTTYPE` is set to `DHT22` for simulation. On real
  DHT11 hardware, change `DHTTYPE` to `DHT11`.
- `WIFI_SSID` is set to `Wokwi-GUEST` (the simulator's built-in open network)
  for in-sim testing; replace with real router credentials for hardware.