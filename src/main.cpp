#include <Arduino.h>

#include "dashboard/app_state.h"
#include "dashboard/time_format.h"
#include "dashboard/buzzer.h"
#include "dashboard/lcd_display.h"
#include "dashboard/timers.h"
#include "dashboard/web_routes.h"
#include "dashboard/wifi_setup.h"

void setup() {

    Serial.begin(115200);

    // Buzzer

    pinMode(BUZZER_PIN, OUTPUT);

    noTone(BUZZER_PIN);

    // I2C

    Wire.begin(I2C_SDA, I2C_SCL);

    // LCD

    lcd.init();
    lcd.backlight();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Pomodoro Fa3");

    lcd.setCursor(0, 1);
    lcd.print("Connecting...");

    // Wi-Fi

    connectWiFi();

    // Web routes

    setupRoutes();

    Serial.println("Web server started!");

    Serial.print("Open: http://");
    Serial.println(WiFi.localIP());

    updateLCD();
}

void loop() {

    server.handleClient();

    updateTimer();

    updateBuzzer();
}
