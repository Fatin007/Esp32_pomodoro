#include "dashboard/web_routes.h"
#include "dashboard/dashboard_page.h"
#include "dashboard/app_state.h"
#include "dashboard/timers.h"
#include "dashboard/lcd_display.h"
#include "dashboard/buzzer.h"
#include "dashboard/time_format.h"

WebServer server(80);

// ======================================================
// Root - Dashboard HTML
// ======================================================

void handleRoot() {

    server.send(200, "text/html", MAIN_PAGE);
}

// ======================================================
// Toggle Start / Pause
// ======================================================

void handleToggle() {

    timerRunning = !timerRunning;

    lastTimerUpdate = millis();

    server.send(200, "text/plain", "OK");
}

// ======================================================
// Reset
// ======================================================

void handleReset() {

    resetCurrentMode();

    server.send(200, "text/plain", "OK");
}

// ======================================================
// Skip Pomodoro
// ======================================================

void handleSkip() {

    if (appMode == POMODORO_MODE) {
        startBuzzer();
        nextPomodoroSession();
    }

    server.send(200, "text/plain", "OK");
}

// ======================================================
// Change Mode
// ======================================================

void handleMode() {

    if (!server.hasArg("value")) {

        server.send(400, "text/plain", "Missing mode");

        return;
    }

    String value = server.arg("value");

    if (value == "pomodoro") {

        changeAppMode(POMODORO_MODE);

    } else if (value == "custom") {

        changeAppMode(CUSTOM_TIMER_MODE);

    } else if (value == "stopwatch") {

        changeAppMode(STOPWATCH_MODE);
    }

    server.send(200, "text/plain", "OK");
}

// ======================================================
// Set Custom Timer
// ======================================================

void handleCustomTimer() {

    if (
        !server.hasArg("minutes") ||
        !server.hasArg("seconds")
    ) {

        server.send(400, "text/plain", "Missing time");

        return;
    }

    unsigned long minutes = server.arg("minutes").toInt();

    unsigned long seconds = server.arg("seconds").toInt();

    if (seconds > 59) {

        seconds = 59;
    }

    unsigned long totalSeconds = minutes * 60UL + seconds;

    if (totalSeconds == 0) {

        server.send(400, "text/plain", "Invalid time");

        return;
    }

    appMode = CUSTOM_TIMER_MODE;

    timerRunning = false;

    customRemaining = totalSeconds;

    customInitialTime = totalSeconds;

    updateLCD();

    server.send(200, "text/plain", "OK");
}

// ======================================================
// Status JSON
// ======================================================

void handleStatus() {

    String modeName;
    unsigned long remaining = 0;
    unsigned long duration = 0;

    if (appMode == POMODORO_MODE) {

        modeName = getPomodoroModeName();

        remaining = pomodoroRemaining;

        duration = getPomodoroDuration();

    } else if (appMode == CUSTOM_TIMER_MODE) {

        modeName = "CUSTOM TIMER";

        remaining = customRemaining;

        duration = customInitialTime;

    } else {

        modeName = "STOPWATCH";

        remaining = stopwatchElapsed;

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
    json += timerRunning ? "true" : "false";
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

    server.send(200, "application/json", json);
}

// ======================================================
// Register Routes
// ======================================================

void setupRoutes() {

    server.on("/",        handleRoot);
    server.on("/toggle",  handleToggle);
    server.on("/reset",   handleReset);
    server.on("/skip",    handleSkip);
    server.on("/mode",    handleMode);
    server.on("/custom",  handleCustomTimer);
    server.on("/status",  handleStatus);

    server.begin();
}
