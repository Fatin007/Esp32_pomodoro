#ifndef POMODORO_APP_STATE_H
#define POMODORO_APP_STATE_H

#include <Arduino.h>

// ======================================================
// Pins
// ======================================================

#define I2C_SDA 21
#define I2C_SCL 22
#define BUZZER_PIN 25

// ======================================================
// Pomodoro Settings
// ======================================================

extern const unsigned long FOCUS_SECONDS;
extern const unsigned long SHORT_BREAK_SECONDS;
extern const unsigned long LONG_BREAK_SECONDS;

// ======================================================
// Application Modes
// ======================================================

enum AppMode {
    POMODORO_MODE,
    CUSTOM_TIMER_MODE,
    STOPWATCH_MODE
};

extern AppMode appMode;

// ======================================================
// Pomodoro Modes
// ======================================================

enum PomodoroMode {
    FOCUS,
    SHORT_BREAK,
    LONG_BREAK
};

extern PomodoroMode pomodoroMode;

// ======================================================
// Timer State
// ======================================================

extern bool timerRunning;
extern unsigned long lastTimerUpdate;

// ======================================================
// Pomodoro
// ======================================================

extern unsigned long pomodoroRemaining;
extern int completedPomodoros;

// ======================================================
// Custom Timer
// ======================================================

extern unsigned long customRemaining;
extern unsigned long customInitialTime;

// ======================================================
// Stopwatch
// ======================================================

extern unsigned long stopwatchElapsed;

// ======================================================
// Whistle
// ======================================================

extern bool whistleActive;
extern unsigned long whistleStartTime;
extern unsigned long whistleLastUpdate;

#endif
