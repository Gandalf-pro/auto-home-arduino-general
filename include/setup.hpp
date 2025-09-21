#ifndef setup__1
#define setup__1

#include <ArduinoOTA.h>
#include <DNSServer.h>

#ifdef ESP8266
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#elif ESP32
#include <WiFi.h>
#endif

#include <WiFiManager.h>

#include <discoveryService.hpp>

namespace Setup {

// Global WiFiManager instance
WiFiManager wifiManager;

// WiFi monitoring constants
const unsigned long WIFI_CHECK_INTERVAL = 10000;   // 10 seconds
const unsigned long WIFI_RECONNECT_DELAY = 30000;  // 30 seconds

void setupOta(const char* hostName, const char* pass) {
    // OTA setting
    ArduinoOTA.setHostname(hostName);
    ArduinoOTA.setPassword(pass);

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else {  // U_FS
            type = "filesystem";
        }

        // NOTE: if updating FS this would be the place to unmount FS using FS.end()
        Serial.println("Start updating " + type);
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
            Serial.println("End Failed");
        }
    });

    ArduinoOTA.begin();
}

void setup_wifi() {
    delay(10);
    WiFi.mode(WIFI_STA);  // explicitly set mode, esp defaults to STA+AP
    WiFi.hostname(HOSTNAME);
    // WiFi Manager

    // Reset after 2 minutes
    wifiManager.setTimeout(120);
    // Wifi ap mode ssid
    bool wifiConnected = wifiManager.autoConnect("Arduino", "setup666");
#ifdef ESP32
    wifiManager.setDarkMode(true);
#endif

    if (wifiConnected) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("Failed to connect restarting");
        ESP.restart();
    }

    randomSeed(micros());
}
static unsigned long lastCheckTime = 0;
static unsigned long disconnectTime = 0;
static bool connectionLost = false;

// We dont need this in esp8266 since it does it automatically
#ifdef ESP32
void loop_wifi() {
    unsigned long currentTime = millis();

    // Check WiFi status every 10 seconds
    if (currentTime - lastCheckTime >= WIFI_CHECK_INTERVAL) {
        lastCheckTime = currentTime;

        if (WiFi.status() != WL_CONNECTED) {
            if (!connectionLost) {
                // First time detecting disconnection
                connectionLost = true;
                disconnectTime = currentTime;
                Serial.println("WiFi connection lost, waiting 30 seconds before retry...");
            } else if (currentTime - disconnectTime >= 30000) {
                // 30 seconds have passed, try to reconnect
                Serial.println("Attempting to reconnect to WiFi...");
                if (WiFi.reconnect()) {
                    Serial.println("WiFi reconnected successfully");
                    connectionLost = false;
                } else {
                    Serial.println("WiFi reconnection failed, restarting device...");
                    wifiManager.reboot();
                }
            }
        } else {
            // WiFi is connected
            if (connectionLost) {
                Serial.println("WiFi connection restored");
                connectionLost = false;
            }
        }
    }
}
#endif

}  // namespace Setup

#endif