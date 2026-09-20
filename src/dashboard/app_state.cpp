#include "app_state.h"

// ======================================================
// Pomodoro Settings
// ======================================================

const unsigned long FOCUS_SECONDS = 25UL * 60UL;
const unsigned long SHORT_BREAK_SECONDS = 5UL * 60UL;
const unsigned long LONG_BREAK_SECONDS = 15UL * 60UL;

// ======================================================
// Application Modes
// ======================================================

AppMode appMode = POMODORO_MODE;

// ======================================================
// Pomodoro Modes
// ======================================================

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
// Whistle
// ======================================================

bool whistleActive = false;
unsigned long whistleStartTime = 0;
unsigned long whistleLastUpdate = 0;
