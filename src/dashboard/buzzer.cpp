#include <Arduino.h>
#include "dashboard/buzzer.h"
#include "dashboard/app_state.h"

// ======================================================
// Start Whistle
// ======================================================

void startBuzzer() {

    whistleActive = true;

    whistleStartTime = millis();

    whistleLastUpdate = millis();

    tone(BUZZER_PIN, 2400);
}

// ======================================================
// Update Whistle
// ======================================================

void updateBuzzer() {

    if (!whistleActive) {
        return;
    }

    unsigned long now = millis();

    unsigned long elapsed = now - whistleStartTime;

    // Total whistle duration: 2.5 seconds

    if (elapsed >= 2500) {

        noTone(BUZZER_PIN);

        whistleActive = false;

        return;
    }

    // Rising/falling pitch every 600 ms

    unsigned long phase = elapsed % 600;

    int frequency;

    if (phase < 300) {
        // Rising pitch: 2400 -> 3200 Hz
        frequency = map(phase, 0, 300, 2400, 3200);
    } else {
        // Falling pitch: 3200 -> 2400 Hz
        frequency = map(phase, 300, 600, 3200, 2400);
    }

    // Update frequency every 20 ms

    if (now - whistleLastUpdate >= 20) {

        whistleLastUpdate = now;

        tone(BUZZER_PIN, frequency);
    }
}
