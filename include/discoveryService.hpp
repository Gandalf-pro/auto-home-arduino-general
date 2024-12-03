#ifndef MQTT_DISCOVERY_SER
#define MQTT_DISCOVERY_SER

#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

#include <LittleFS.h>
#include <WiFiUdp.h>

namespace Setup {

String readMqttAddress() {
    // Open the file
    File file = LittleFS.open("/mqtta.txt", "r");

    // Check if the file exists
    if (!file) {
        // Read the file data
        Serial.println("No Saved mqttAdress!");
        return "0";
    }
    // Read the data
    String str = file.readString();

    // Close the file
    file.close();

    return str;
}

void writeMqttAddress(String address) {
    // Open the file
    File file = LittleFS.open("/mqtta.txt", "w");

    // Write to the file
    file.print(address);
    delay(1);
    // Close the file
    file.close();

    Serial.println("Write successful");
}

String dicoverMqttServer(bool ignoreSaved) {
    // If there is a saved one get it
    if (!ignoreSaved) {
        String str = readMqttAddress();
        if (!str.equals("0")) {
            Serial.println("Got mqtt adress from flash: " + str);
            return str;
        }
    }

    unsigned int localPort = 29874;
    WiFiUDP udp;
    udp.begin(localPort);

    // Send the broadcast
    IPAddress SendIP(WiFi.localIP());
    SendIP[3] = 255;
    Serial.println("Broadcast address: " + SendIP.toString());
    udp.beginPacket(SendIP, 29874);
#ifdef ESP8266
    udp.write("p");
#elif defined(ESP32)
    udp.write((uint8_t)'p');
#endif
    udp.write((uint8_t)'p');
    udp.endPacket();

    unsigned long startTime = millis();
    // Wait 15 seconds before exiting
    while (!udp.parsePacket()) {
        if (millis() - startTime > 15000) {
            return "0";
        }
        delay(50);
    }

    // Read the data
    String mqttIp = udp.readString();

    // Write to flash
    writeMqttAddress(mqttIp);

    Serial.println("Got mqtt adress: " + mqttIp);
    return mqttIp;
}

}  // namespace Setup

#endif  // MQTT_DISCOVERY_SER
