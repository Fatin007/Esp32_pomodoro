#include "dashboard/timers.h"
#include "dashboard/app_state.h"
#include "dashboard/buzzer.h"
#include "dashboard/lcd_display.h"

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
// Change Application Mode
// ======================================================

void changeAppMode(int newMode) {

    timerRunning = false;

    appMode = (AppMode) newMode;

    updateLCD();
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

    } else {

        pomodoroMode = FOCUS;
    }

    pomodoroRemaining = getPomodoroDuration();

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

        pomodoroRemaining = FOCUS_SECONDS;

        completedPomodoros = 0;

    } else if (appMode == CUSTOM_TIMER_MODE) {

        customRemaining = customInitialTime;

    } else if (appMode == STOPWATCH_MODE) {

        stopwatchElapsed = 0;
    }

    updateLCD();
}

// ======================================================
// Timer Tick
// ======================================================

void updateTimer() {

    if (!timerRunning) {
        return;
    }

    unsigned long currentMillis = millis();

    if (currentMillis - lastTimerUpdate < 1000) {
        return;
    }

    lastTimerUpdate = currentMillis;

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
