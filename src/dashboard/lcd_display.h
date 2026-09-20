#ifndef POMODORO_LCD_DISPLAY_H
#define POMODORO_LCD_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void updateLCD();

#endif
