#include "dashboard/lcd_display.h"
#include "dashboard/app_state.h"
#include "dashboard/time_format.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

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
        } else if (pomodoroMode == SHORT_BREAK) {
            lcd.print("SHORT BREAK");
        } else {
            lcd.print("LONG BREAK");
        }

        lcd.setCursor(5, 1);
        lcd.print(formatTime(pomodoroRemaining));

        return;
    }

    // --------------------------------------------------
    // Custom Timer
    // --------------------------------------------------

    if (appMode == CUSTOM_TIMER_MODE) {

        lcd.setCursor(0, 0);
        lcd.print("CUSTOM TIMER");

        lcd.setCursor(5, 1);
        lcd.print(formatTime(customRemaining));

        return;
    }

    // --------------------------------------------------
    // Stopwatch
    // --------------------------------------------------

    if (appMode == STOPWATCH_MODE) {

        lcd.setCursor(0, 0);
        lcd.print("STOPWATCH");

        String time = formatTime(stopwatchElapsed);

        int cursorPosition;

        if (stopwatchElapsed >= 3600) {
            cursorPosition = 4;
        } else {
            cursorPosition = 5;
        }

        lcd.setCursor(cursorPosition, 1);
        lcd.print(time);
    }
}
