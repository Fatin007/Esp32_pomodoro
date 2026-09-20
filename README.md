# ESP32 Pomodoro Timer Fa3

A multi-mode productivity timer on ESP32. Combines an LCD readout with a web dashboard you can open from any phone on the same Wi-Fi.

Three modes:

- **Pomodoro** — automatic 25 / 5 / 15 minute rotation with a long break every 4th cycle.
- **Custom Timer** — set any countdown up to 999 minutes 59 seconds.
- **Stopwatch** — counts up from zero.

A buzzer plays a short whistle when a session ends.

![Circuit](images/circuit_image.png)

## Hardware

- ESP32 Dev Board
- 16x2 I2C LCD (address 0x27)
- Buzzer

## Wiring

| Signal | GPIO |
|---|---|
| I2C SDA | D21 |
| I2C SCL | D22 |
| I2C VCC | VIN |
| I2C GND | GND |
| Buzzer VCC | D25 |
| Buzzer GND | GND |

## Build & Upload

```bash
pio run -t upload
pio device monitor -b 115200
```

After Wi-Fi connects, the serial monitor prints the IP address. Open `http://<ip>/` in any browser on the same network.

## Configuration

Edit the top of `src/main.cpp`:

```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ======================================================
// Wi-Fi
// ======================================================

const char* WIFI_SSID = "YOUR-WIFI-SSID";
const char* WIFI_PASSWORD = "YOUR-WIFI-PASSWORD";

// ======================================================
// Pins
// ======================================================

#define I2C_SDA 21
#define I2C_SCL 22
#define BUZZER_PIN 25

// ======================================================
// LCD
// ======================================================

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ======================================================
// Web Server
// ======================================================

WebServer server(80);

// ======================================================
// Pomodoro Settings
// ======================================================

const unsigned long FOCUS_SECONDS = 25UL * 60UL;
const unsigned long SHORT_BREAK_SECONDS = 5UL * 60UL;
const unsigned long LONG_BREAK_SECONDS = 15UL * 60UL;
```

## Screenshots

### Web Dashboard

Pomodoro mode:
![Pomodoro Dashboard](images/Screenshot/Pomodoro%20Timer.png)

Custom Timer mode:
![Custom Timer Dashboard](images/Screenshot/Custom%20Timer.png)

Stopwatch mode:
![Stopwatch Dashboard](images/Screenshot/Stop%20Watch.png)

### Real Device

Pomodoro:
![Pomodoro Device](images/Real/Pomodoro%20Timer.jpg)

Custom Timer:
![Custom Timer Device](images/Real/Custom%20Timer.jpg)

Stopwatch:
![Stopwatch Device](images/Real/Stop%20Watch.jpg)

## Web API

| Path | Purpose |
|---|---|
| `GET /` | Dashboard HTML |
| `GET /status` | JSON snapshot |
| `GET /toggle` | Start/Pause |
| `GET /reset` | Reset current mode |
| `GET /skip` | Skip Pomodoro phase |
| `GET /mode?value=pomodoro\|custom\|stopwatch` | Switch mode |
| `GET /custom?minutes=N&seconds=N` | Set custom timer |

## License

Personal and educational use.