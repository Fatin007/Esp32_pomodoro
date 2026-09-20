#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ======================================================
// Wi-Fi
// ======================================================

const char* WIFI_SSID = "Room-601";
const char* WIFI_PASSWORD = "room601601";

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

// ======================================================
// Application Modes
// ======================================================

enum AppMode {
    POMODORO_MODE,
    CUSTOM_TIMER_MODE,
    STOPWATCH_MODE
};

AppMode appMode = POMODORO_MODE;

// ======================================================
// Pomodoro Modes
// ======================================================

enum PomodoroMode {
    FOCUS,
    SHORT_BREAK,
    LONG_BREAK
};

PomodoroMode pomodoroMode = FOCUS;

// ======================================================
// Timer State
// ======================================================

bool timerRunning = false;

unsigned long lastTimerUpdate = 0;

// ======================================================
// Pomodoro
// ======================================================

unsigned long pomodoroRemaining = FOCUS_SECONDS;

int completedPomodoros = 0;

// ======================================================
// Custom Timer
// ======================================================

unsigned long customRemaining = 0;

unsigned long customInitialTime = 0;

// ======================================================
// Stopwatch
// ======================================================

unsigned long stopwatchElapsed = 0;

// ======================================================
// Football Whistle
// ======================================================

bool whistleActive = false;

unsigned long whistleStartTime = 0;
unsigned long whistleLastUpdate = 0;

// ======================================================
// Get Pomodoro Mode Name
// ======================================================

String getPomodoroModeName() {

    switch (pomodoroMode) {

        case FOCUS:
            return "FOCUS";

        case SHORT_BREAK:
            return "SHORT BREAK";

        case LONG_BREAK:
            return "LONG BREAK";
    }

    return "FOCUS";
}

// ======================================================
// Get Pomodoro Duration
// ======================================================

unsigned long getPomodoroDuration() {

    switch (pomodoroMode) {

        case FOCUS:
            return FOCUS_SECONDS;

        case SHORT_BREAK:
            return SHORT_BREAK_SECONDS;

        case LONG_BREAK:
            return LONG_BREAK_SECONDS;
    }

    return FOCUS_SECONDS;
}

// ======================================================
// Format Time
// ======================================================

String formatTime(unsigned long totalSeconds) {

    unsigned long hours =
        totalSeconds / 3600;

    unsigned long minutes =
        (totalSeconds % 3600) / 60;

    unsigned long seconds =
        totalSeconds % 60;

    char buffer[12];

    if (hours > 0) {

        sprintf(
            buffer,
            "%02lu:%02lu:%02lu",
            hours,
            minutes,
            seconds
        );

    } else {

        sprintf(
            buffer,
            "%02lu:%02lu",
            minutes,
            seconds
        );
    }

    return String(buffer);
}

// ======================================================
// Start Football Whistle
// ======================================================

void startBuzzer() {

    whistleActive = true;

    whistleStartTime = millis();

    whistleLastUpdate = millis();

    tone(BUZZER_PIN, 2400);
}

// ======================================================
// Update Football Whistle
// ======================================================

void updateBuzzer() {

    if (!whistleActive) {
        return;
    }

    unsigned long now = millis();

    unsigned long elapsed =
        now - whistleStartTime;

    // Total whistle duration
    // 2.5 seconds

    if (elapsed >= 2500) {

        noTone(BUZZER_PIN);

        whistleActive = false;

        return;
    }

    // Repeat a rising/falling whistle
    // every 600 milliseconds

    unsigned long phase =
        elapsed % 600;

    int frequency;

    if (phase < 300) {

        // Rising pitch

        frequency =
            map(
                phase,
                0,
                300,
                2400,
                3200
            );

    } else {

        // Falling pitch

        frequency =
            map(
                phase,
                300,
                600,
                3200,
                2400
            );
    }

    // Update frequency every 20 ms

    if (now - whistleLastUpdate >= 20) {

        whistleLastUpdate = now;

        tone(
            BUZZER_PIN,
            frequency
        );
    }
}

// ======================================================
// LCD Update
// ======================================================

void updateLCD() {

    lcd.clear();

    // --------------------------------------------------
    // Pomodoro
    // --------------------------------------------------

    if (appMode == POMODORO_MODE) {

        lcd.setCursor(0, 0);

        if (pomodoroMode == FOCUS) {

            lcd.print("FOCUS #");
            lcd.print(completedPomodoros + 1);

        }

        else if (pomodoroMode == SHORT_BREAK) {

            lcd.print("SHORT BREAK");

        }

        else {

            lcd.print("LONG BREAK");
        }

        lcd.setCursor(5, 1);

        lcd.print(
            formatTime(pomodoroRemaining)
        );

        return;
    }

    // --------------------------------------------------
    // Custom Timer
    // --------------------------------------------------

    if (appMode == CUSTOM_TIMER_MODE) {

        lcd.setCursor(0, 0);

        lcd.print("CUSTOM TIMER");

        lcd.setCursor(5, 1);

        lcd.print(
            formatTime(customRemaining)
        );

        return;
    }

    // --------------------------------------------------
    // Stopwatch
    // --------------------------------------------------

    if (appMode == STOPWATCH_MODE) {

        lcd.setCursor(0, 0);

        lcd.print("STOPWATCH");

        String time =
            formatTime(stopwatchElapsed);

        int cursorPosition;

        if (stopwatchElapsed >= 3600) {

            cursorPosition = 4;

        } else {

            cursorPosition = 5;
        }

        lcd.setCursor(
            cursorPosition,
            1
        );

        lcd.print(time);
    }
}

// ======================================================
// Move to Next Pomodoro Session
// ======================================================

void nextPomodoroSession() {

    if (pomodoroMode == FOCUS) {

        completedPomodoros++;

        if (completedPomodoros % 4 == 0) {

            pomodoroMode = LONG_BREAK;

        } else {

            pomodoroMode = SHORT_BREAK;
        }
    }

    else {

        pomodoroMode = FOCUS;
    }

    pomodoroRemaining =
        getPomodoroDuration();

    timerRunning = false;

    updateLCD();
}

// ======================================================
// Reset Current Mode
// ======================================================

void resetCurrentMode() {

    timerRunning = false;

    if (appMode == POMODORO_MODE) {

        pomodoroMode = FOCUS;

        pomodoroRemaining =
            FOCUS_SECONDS;

        completedPomodoros = 0;
    }

    else if (appMode == CUSTOM_TIMER_MODE) {

        customRemaining =
            customInitialTime;
    }

    else if (appMode == STOPWATCH_MODE) {

        stopwatchElapsed = 0;
    }

    updateLCD();
}

// ======================================================
// Timer Update
// ======================================================

void updateTimer() {

    if (!timerRunning) {
        return;
    }

    unsigned long currentMillis =
        millis();

    if (
        currentMillis - lastTimerUpdate
        < 1000
    ) {
        return;
    }

    lastTimerUpdate =
        currentMillis;

    // --------------------------------------------------
    // Pomodoro
    // --------------------------------------------------

    if (appMode == POMODORO_MODE) {

        if (pomodoroRemaining > 0) {

            pomodoroRemaining--;

            updateLCD();
        }

        if (pomodoroRemaining == 0) {

            timerRunning = false;

            startBuzzer();

            nextPomodoroSession();
        }
    }

    // --------------------------------------------------
    // Custom Timer
    // --------------------------------------------------

    else if (appMode == CUSTOM_TIMER_MODE) {

        if (customRemaining > 0) {

            customRemaining--;

            updateLCD();
        }

        if (customRemaining == 0) {

            timerRunning = false;

            startBuzzer();

            updateLCD();
        }
    }

    // --------------------------------------------------
    // Stopwatch
    // --------------------------------------------------

    else if (appMode == STOPWATCH_MODE) {

        stopwatchElapsed++;

        updateLCD();
    }
}

// ======================================================
// Change Application Mode
// ======================================================

void changeAppMode(AppMode newMode) {

    timerRunning = false;

    appMode = newMode;

    updateLCD();
}

// ======================================================
// HTML Dashboard
// ======================================================

const char MAIN_PAGE[] PROGMEM = R"rawliteral(

<!DOCTYPE html>

<html>

<head>

<meta name="viewport"
      content="width=device-width, initial-scale=1">

<title>Pomodoro Timer Fa3</title>

<style>

* {
    box-sizing: border-box;
}

body {

    margin: 0;

    font-family:
        -apple-system,
        BlinkMacSystemFont,
        "Segoe UI",
        Arial,
        sans-serif;

    background:
        linear-gradient(
            135deg,
            #0f172a,
            #111827,
            #1e293b
        );

    color: white;

    min-height: 100vh;

    display: flex;

    justify-content: center;

    align-items: center;

    padding: 20px;

}

.container {

    width: 100%;

    max-width: 480px;

    background:
        rgba(31, 41, 55, 0.92);

    backdrop-filter: blur(20px);

    border: 1px solid
        rgba(255,255,255,0.08);

    border-radius: 28px;

    padding: 28px;

    box-shadow:
        0 25px 70px
        rgba(0,0,0,0.45);

}

.header {

    display: flex;

    align-items: center;

    justify-content: space-between;

    margin-bottom: 25px;

}

.logo h1 {

    font-size: 22px;

    margin: 0;

}

.connection {

    width: 10px;

    height: 10px;

    border-radius: 50%;

    background: #22c55e;

    box-shadow:
        0 0 12px
        rgba(34,197,94,0.8);

}

.tabs {

    display: grid;

    grid-template-columns:
        repeat(3, 1fr);

    background: #111827;

    border-radius: 15px;

    padding: 5px;

    margin-bottom: 28px;

}

.tab {

    border: none;

    background: transparent;

    color: #9ca3af;

    padding: 11px 5px;

    border-radius: 11px;

    cursor: pointer;

    font-size: 13px;

    font-weight: 600;

}

.tab.active {

    background: #374151;

    color: white;

}

.mode {

    text-align: center;

    font-size: 16px;

    color: #9ca3af;

    margin-bottom: 5px;

    text-transform: uppercase;

    letter-spacing: 2px;

    font-weight: 600;

}

.timer {

    text-align: center;

    font-size:
        clamp(58px, 17vw, 82px);

    line-height: 1;

    font-weight: 700;

    letter-spacing: 2px;

    margin: 22px 0;

    font-variant-numeric:
        tabular-nums;

}

.status {

    text-align: center;

    color: #9ca3af;

    font-size: 14px;

    margin-bottom: 22px;

}

.progress {

    height: 7px;

    background: #374151;

    border-radius: 10px;

    overflow: hidden;

    margin-bottom: 25px;

}

.progress-bar {

    height: 100%;

    width: 0%;

    background: #22c55e;

    border-radius: 10px;

    transition:
        width 0.5s ease;

}

.controls {

    display: grid;

    grid-template-columns:
        1fr 1fr;

    gap: 12px;

}

button.control {

    border: none;

    border-radius: 14px;

    padding: 15px;

    font-size: 16px;

    font-weight: 700;

    cursor: pointer;

    color: white;

}

.start {

    background: #22c55e;

}

.reset {

    background: #374151;

}

.skip {

    background: #2563eb;

    grid-column: span 2;

}

.custom-panel {

    display: none;

    margin-bottom: 22px;

}

.custom-panel.show {

    display: block;

}

.input-row {

    display: grid;

    grid-template-columns:
        1fr 1fr;

    gap: 12px;

}

.input-group label {

    display: block;

    color: #9ca3af;

    font-size: 13px;

    margin-bottom: 7px;

}

.input-group input {

    width: 100%;

    padding: 13px;

    border: 1px solid #374151;

    background: #111827;

    color: white;

    border-radius: 12px;

    font-size: 18px;

    text-align: center;

    outline: none;

}

.input-group input:focus {

    border-color: #60a5fa;

}

.set-button {

    width: 100%;

    margin-top: 12px;

    padding: 13px;

    border: none;

    border-radius: 12px;

    background: #3b82f6;

    color: white;

    font-weight: 700;

    cursor: pointer;

}

.stats {

    display: grid;

    grid-template-columns:
        1fr 1fr;

    gap: 12px;

    margin-top: 25px;

}

.stat {

    background: #111827;

    border-radius: 15px;

    padding: 15px;

    text-align: center;

}

.stat-value {

    font-size: 23px;

    font-weight: 700;

}

.stat-label {

    font-size: 12px;

    color: #9ca3af;

    margin-top: 3px;

}

.footer {

    text-align: center;

    margin-top: 20px;

    color: #6b7280;

    font-size: 11px;

}

</style>

</head>

<body>

<div class="container">

    <div class="header">

        <div class="logo">

            <h1>Pomodoro Timer Fa3</h1>

        </div>

        <div
            class="connection"
            id="connection">
        </div>

    </div>


    <div class="tabs">

        <button
            class="tab active"
            id="pomodoroTab"
            onclick="setMode('pomodoro')">

            Pomodoro

        </button>

        <button
            class="tab"
            id="customTab"
            onclick="setMode('custom')">

            Timer

        </button>

        <button
            class="tab"
            id="stopwatchTab"
            onclick="setMode('stopwatch')">

            Stopwatch

        </button>

    </div>


    <div
        class="custom-panel"
        id="customPanel">

        <div class="input-row">

            <div class="input-group">

                <label>Minutes</label>

                <input
                    type="number"
                    id="customMinutes"
                    min="0"
                    max="999"
                    value="1">

            </div>

            <div class="input-group">

                <label>Seconds</label>

                <input
                    type="number"
                    id="customSeconds"
                    min="0"
                    max="59"
                    value="0">

            </div>

        </div>

        <button
            class="set-button"
            onclick="setCustomTime()">

            Set Custom Time

        </button>

    </div>


    <div
        class="mode"
        id="mode">

        FOCUS

    </div>


    <div
        class="timer"
        id="timer">

        25:00

    </div>


    <div
        class="status"
        id="status">

        Paused

    </div>


    <div class="progress">

        <div
            class="progress-bar"
            id="progressBar">
        </div>

    </div>


    <div class="controls">

        <button
            class="control start"
            id="startButton"
            onclick="toggleTimer()">

            Start

        </button>

        <button
            class="control reset"
            onclick="resetTimer()">

            Reset

        </button>

        <button
            class="control skip"
            id="skipButton"
            onclick="skipSession()">

            Skip Session

        </button>

    </div>


    <div class="stats">

        <div class="stat">

            <div
                class="stat-value"
                id="completed">

                0

            </div>

            <div class="stat-label">

                Pomodoros

            </div>

        </div>

        <div class="stat">

            <div
                class="stat-value"
                id="device">

                ESP32

            </div>

            <div class="stat-label">

                Device

            </div>

        </div>

    </div>


    <div class="footer">

        Local ESP32 Timer - Fa3

    </div>

</div>


<script>

let currentMode = 'pomodoro';


// ======================================================
// Get Status
// ======================================================

async function getStatus() {

    try {

        const response =
            await fetch('/status');

        const data =
            await response.json();


        document.getElementById('mode')
            .innerText = data.mode;


        document.getElementById('timer')
            .innerText = data.time;


        document.getElementById('completed')
            .innerText = data.completed;


        document.getElementById('status')
            .innerText =
                data.running
                ? 'Running'
                : 'Paused';


        document.getElementById('startButton')
            .innerText =
                data.running
                ? 'Pause'
                : 'Start';


        let percentage = 0;

        if (data.duration > 0) {

            percentage =
                (
                    (data.duration - data.remaining)
                    / data.duration
                ) * 100;

        }

        document.getElementById('progressBar')
            .style.width =
                percentage + '%';


        document.getElementById('connection')
            .style.background =
                '#22c55e';

    }

    catch (error) {

        document.getElementById('connection')
            .style.background =
                '#ef4444';

    }

}


// ======================================================
// Change Mode
// ======================================================

async function setMode(mode) {

    currentMode = mode;

    await fetch(
        '/mode?value=' + mode
    );


    document
        .querySelectorAll('.tab')
        .forEach(
            tab =>
                tab.classList.remove('active')
        );


    if (mode === 'pomodoro') {

        document
            .getElementById('pomodoroTab')
            .classList.add('active');

        document
            .getElementById('customPanel')
            .classList.remove('show');

        document
            .getElementById('skipButton')
            .style.display = 'block';

    }

    else if (mode === 'custom') {

        document
            .getElementById('customTab')
            .classList.add('active');

        document
            .getElementById('customPanel')
            .classList.add('show');

        document
            .getElementById('skipButton')
            .style.display = 'none';

    }

    else {

        document
            .getElementById('stopwatchTab')
            .classList.add('active');

        document
            .getElementById('customPanel')
            .classList.remove('show');

        document
            .getElementById('skipButton')
            .style.display = 'none';
    }


    getStatus();
}


// ======================================================
// Start / Pause
// ======================================================

async function toggleTimer() {

    await fetch('/toggle');

    getStatus();
}


// ======================================================
// Reset
// ======================================================

async function resetTimer() {

    await fetch('/reset');

    getStatus();
}


// ======================================================
// Skip
// ======================================================

async function skipSession() {

    await fetch('/skip');

    getStatus();
}


// ======================================================
// Set Custom Time
// ======================================================

async function setCustomTime() {

    let minutes =
        parseInt(
            document.getElementById(
                'customMinutes'
            ).value
        ) || 0;


    let seconds =
        parseInt(
            document.getElementById(
                'customSeconds'
            ).value
        ) || 0;


    if (minutes < 0) {
        minutes = 0;
    }

    if (seconds < 0) {
        seconds = 0;
    }

    if (seconds > 59) {
        seconds = 59;
    }


    if (
        minutes === 0 &&
        seconds === 0
    ) {

        alert(
            'Please enter a time greater than 0.'
        );

        return;
    }


    await fetch(
        '/custom?minutes='
        + minutes
        + '&seconds='
        + seconds
    );


    getStatus();
}


// ======================================================
// Auto Refresh
// ======================================================

setInterval(
    getStatus,
    1000
);

getStatus();

</script>

</body>

</html>

)rawliteral";

// ======================================================
// Root
// ======================================================

void handleRoot() {

    server.send(
        200,
        "text/html",
        MAIN_PAGE
    );
}

// ======================================================
// Toggle Start / Pause
// ======================================================

void handleToggle() {

    timerRunning = !timerRunning;

    lastTimerUpdate = millis();

    server.send(
        200,
        "text/plain",
        "OK"
    );
}

// ======================================================
// Reset
// ======================================================

void handleReset() {

    resetCurrentMode();

    server.send(
        200,
        "text/plain",
        "OK"
    );
}

// ======================================================
// Skip Pomodoro
// ======================================================

void handleSkip() {

    if (appMode == POMODORO_MODE) {

        startBuzzer();

        nextPomodoroSession();
    }

    server.send(
        200,
        "text/plain",
        "OK"
    );
}

// ======================================================
// Change Mode
// ======================================================

void handleMode() {

    if (!server.hasArg("value")) {

        server.send(
            400,
            "text/plain",
            "Missing mode"
        );

        return;
    }

    String value =
        server.arg("value");


    if (value == "pomodoro") {

        changeAppMode(
            POMODORO_MODE
        );
    }

    else if (value == "custom") {

        changeAppMode(
            CUSTOM_TIMER_MODE
        );
    }

    else if (value == "stopwatch") {

        changeAppMode(
            STOPWATCH_MODE
        );
    }


    server.send(
        200,
        "text/plain",
        "OK"
    );
}

// ======================================================
// Set Custom Timer
// ======================================================

void handleCustomTimer() {

    if (
        !server.hasArg("minutes") ||
        !server.hasArg("seconds")
    ) {

        server.send(
            400,
            "text/plain",
            "Missing time"
        );

        return;
    }


    unsigned long minutes =
        server.arg("minutes")
            .toInt();


    unsigned long seconds =
        server.arg("seconds")
            .toInt();


    if (seconds > 59) {

        seconds = 59;
    }


    unsigned long totalSeconds =
        minutes * 60UL + seconds;


    if (totalSeconds == 0) {

        server.send(
            400,
            "text/plain",
            "Invalid time"
        );

        return;
    }


    appMode =
        CUSTOM_TIMER_MODE;

    timerRunning = false;


    customRemaining =
        totalSeconds;

    customInitialTime =
        totalSeconds;


    updateLCD();


    server.send(
        200,
        "text/plain",
        "OK"
    );
}

// ======================================================
// Status JSON
// ======================================================

void handleStatus() {

    String modeName;

    unsigned long remaining = 0;

    unsigned long duration = 0;


    if (appMode == POMODORO_MODE) {

        modeName =
            getPomodoroModeName();

        remaining =
            pomodoroRemaining;

        duration =
            getPomodoroDuration();
    }

    else if (
        appMode == CUSTOM_TIMER_MODE
    ) {

        modeName =
            "CUSTOM TIMER";

        remaining =
            customRemaining;

        duration =
            customInitialTime;
    }

    else {

        modeName =
            "STOPWATCH";

        remaining =
            stopwatchElapsed;

        duration = 0;
    }


    String json = "{";


    json += "\"mode\":\"";

    json += modeName;

    json += "\",";


    json += "\"time\":\"";

    json += formatTime(remaining);

    json += "\",";


    json += "\"running\":";

    json +=
        timerRunning
        ? "true"
        : "false";

    json += ",";


    json += "\"completed\":";

    json += completedPomodoros;

    json += ",";


    json += "\"remaining\":";

    json += remaining;

    json += ",";


    json += "\"duration\":";

    json += duration;

    json += "}";


    server.send(
        200,
        "application/json",
        json
    );
}

// ======================================================
// Wi-Fi Connection
// ======================================================

void connectWiFi() {

    Serial.println();

    Serial.print(
        "Connecting to WiFi: "
    );

    Serial.println(WIFI_SSID);


    WiFi.begin(
        WIFI_SSID,
        WIFI_PASSWORD
    );


    while (
        WiFi.status() != WL_CONNECTED
    ) {

        delay(500);

        Serial.print(".");
    }


    Serial.println();

    Serial.println(
        "WiFi connected!"
    );


    Serial.print(
        "ESP32 IP address: "
    );


    Serial.println(
        WiFi.localIP()
    );
}

// ======================================================
// Setup
// ======================================================

void setup() {

    Serial.begin(115200);


    // Buzzer

    pinMode(
        BUZZER_PIN,
        OUTPUT
    );

    noTone(
        BUZZER_PIN
    );


    // I2C

    Wire.begin(
        I2C_SDA,
        I2C_SCL
    );


    // LCD

    lcd.init();

    lcd.backlight();

    lcd.clear();


    lcd.setCursor(0, 0);

    lcd.print(
        "Pomodoro Fa3"
    );


    lcd.setCursor(0, 1);

    lcd.print(
        "Connecting..."
    );


    // Wi-Fi

    connectWiFi();


    // Web routes

    server.on(
        "/",
        handleRoot
    );

    server.on(
        "/toggle",
        handleToggle
    );

    server.on(
        "/reset",
        handleReset
    );

    server.on(
        "/skip",
        handleSkip
    );

    server.on(
        "/mode",
        handleMode
    );

    server.on(
        "/custom",
        handleCustomTimer
    );

    server.on(
        "/status",
        handleStatus
    );


    server.begin();


    Serial.println(
        "Web server started!"
    );


    Serial.print(
        "Open: http://"
    );

    Serial.println(
        WiFi.localIP()
    );


    updateLCD();
}

// ======================================================
// Main Loop
// ======================================================

void loop() {

    server.handleClient();

    updateTimer();

    updateBuzzer();
}