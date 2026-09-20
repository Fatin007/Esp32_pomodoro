#ifndef POMODORO_WIFI_SETUP_H
#define POMODORO_WIFI_SETUP_H

#include <WiFi.h>

extern const char* WIFI_SSID;
extern const char* WIFI_PASSWORD;

void connectWiFi();

#endif
