#ifndef POMODORO_TIMERS_H
#define POMODORO_TIMERS_H

#include <Arduino.h>

// ======================================================
// Pomodoro helpers
// ======================================================

String getPomodoroModeName();
unsigned long getPomodoroDuration();

// ======================================================
// Mode + session control
// ======================================================

void changeAppMode(int newMode);
void nextPomodoroSession();
void resetCurrentMode();

// ======================================================
// Tick
// ======================================================

void updateTimer();

#endif
