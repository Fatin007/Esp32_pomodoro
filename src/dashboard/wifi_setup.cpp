#include "wifi_setup.h"

const char* WIFI_SSID = "Room-601";
const char* WIFI_PASSWORD = "room601601";

void connectWiFi() {

    Serial.println();

    Serial.print("Connecting to WiFi: ");

    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {

        delay(500);

        Serial.print(".");
    }

    Serial.println();

    Serial.println("WiFi connected!");

    Serial.print("ESP32 IP address: ");

    Serial.println(WiFi.localIP());
}
