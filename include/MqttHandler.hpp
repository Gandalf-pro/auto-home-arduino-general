#ifndef MqttHandler_5645
#define MqttHandler_5645

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <FSDataHandler.hpp>
#include <discoveryService.hpp>

#include "ArduinoJson.h"

#ifndef MQTT_USER
#define MQTT_USER "deneme"
#endif

#ifndef MQTT_PASS
#define MQTT_PASS "123"
#endif

#ifndef MQTT_PORT
#define MQTT_PORT 1883
#endif

#ifndef HOSTNAME
#define HOSTNAME "Default"
#endif

void gotMessage(char* topic, byte* payload, unsigned int length);

WiFiClient wifiClient;
PubSubClient pubClient(wifiClient);

class MqttHandler {
   private:
    WiFiClient* espClient;
    PubSubClient* client;
    FSDataHandler* dataHandler;
    home::Device* device;
    char mqttServerIp[24] = {0};
    char topicToSubStr[50] = {0};
    /* data */
   public:
    PubSubClient* getClient() {
        return this->client;
    }

    void publish(const char* topic, const char* payload) {
        this->client->publish(topic, payload);
    }
    void mqttReconnect() {
        delay(100);
        uint8 tried = 0;
        // Loop until we're reconnected or tried 5 times
        while (!this->client->connected() && tried < 5) {
            tried++;
            Serial.print("Attempting MQTT connection...");
            // Create a random this->client->ID
            String clientId = "ESP8266Client-";

            clientId += String(random(0xffff), HEX);
            // Attempt to connect
            if (this->client->connect(clientId.c_str())) {
                Serial.println("connected");
                // Once connected, publish an announcement...
                Serial.println("Publishing");
                this->client->publish("devices", ("home/ozgur/" + String(HOSTNAME)).c_str());
                // ... and resubscribe
                String topicToSub = this->dataHandler->getRoomName() + "/" + this->dataHandler->getDeviceName();
                Serial.println("Topic to sub:" + topicToSub);
                // Copy                
                strcpy(topicToSubStr, topicToSub.c_str());
                this->client->subscribe(this->topicToSubStr);
                this->client->subscribe("all/data");
                // Send the register info
                this->client->publish("register-device", this->device->getData().c_str());
                this->client->publish("devices-debug", "12");
            } else {
                Serial.print("failed, rc=");
                Serial.print(this->client->state());
                Serial.println(" try again in 5 seconds");
                // Wait 5 seconds before retrying
                yield();
                delay(5000);
            }
        }
    }

    void mqttSetup(String serverIp) {
        strcpy(this->mqttServerIp, serverIp.c_str());
        Serial.println("Seting up mqtt");
        this->client->setServer(this->mqttServerIp, MQTT_PORT);
        this->client->setCallback(gotMessage);
        this->client->setBufferSize(1024);
    }

    void mqttLoop() {
        bool connected = true;
        if (!this->client->connected()) {
            connected = false;
            // If cant connect 5 times get the ip again
            this->mqttReconnect();
            if (!this->client->connected()) {
                // Get the ip again and try on the next loop call
                String ip = Setup::dicoverMqttServer(true);
                if (!ip.equals("0")) {
                    strcpy(this->mqttServerIp, ip.c_str());
                    this->client->setServer(this->mqttServerIp, MQTT_PORT);
                }
            } else {
                connected = true;
            }
        }
        if (connected) {
            this->client->loop();
        }
    }

    MqttHandler(FSDataHandler* dataHandler, home::Device* device);
    ~MqttHandler();
};

MqttHandler::MqttHandler(FSDataHandler* dataHandler, home::Device* device) {
    this->device = device;
    this->dataHandler = dataHandler;
    // this->espClient = new WiFiClient();
    // this->client = new PubSubClient(*espClient);
    this->espClient = &wifiClient;
    this->client = &pubClient;
}

MqttHandler::~MqttHandler() {
}

#endif