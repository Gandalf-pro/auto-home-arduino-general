#ifndef DATA_HANDLER_1_FS
#define DATA_HANDLER_1_FS
#include <Arduino.h>
#include <LittleFS.h>

#include <General.hpp>

class FSDataHandler {
   private:
    /* data */
    home::Device* device;
    String roomName = "default";
    String deviceName = "default";
    bool dataRead = false;

    void writeData() {
        // Open the file
        File file = LittleFS.open("/dataHandler.txt", "w");

        // Write to the file
        file.print(this->roomName);
        delay(1);
        file.print(this->deviceName);
        delay(1);

        // Close the file
        file.close();

        Serial.println("Write successful");
    }

    void isReady() {
        unsigned long startTime = millis();
        // Timeout after 15 seconds
        while (!this->dataRead && millis() - startTime < 15000) {
            delay(100);
        }
    }

   public:
    FSDataHandler(home::Device* device);
    FSDataHandler(home::Device* device, const String& deviceName, const String& roomName);
    ~FSDataHandler();

    void readData() {
        // Open the file
        File file = LittleFS.open("/dataHandler.txt", "r");

        // Check if the file exists
        if (!file) {
            // Read the file data
            Serial.println("No Saved dataHandler!");
            this->dataRead = true;
            return;
        }
        // Read the data
        String roomTmp = file.readString();
        String deviceTmp = file.readString();

        // Set the data
        this->roomName = roomTmp;
        this->deviceName = deviceTmp;

        // Close the file
        file.close();
        this->dataRead = true;
    }

    void setRoomName(String roomName) {
        this->roomName = roomName;
        this->device->setRoom(roomName);
    }
    void setDeviceName(String deviceName) {
        this->deviceName = deviceName;
        this->device->setName(deviceName);
    }
    String getRoomName() {
        this->isReady();
        return this->roomName;
    }
    String getDeviceName() {
        this->isReady();
        return this->deviceName;
    }
};

FSDataHandler::FSDataHandler(home::Device* device) {
    this->device = device;
}
FSDataHandler::FSDataHandler(home::Device* device, const String& deviceName, const String& roomName) {
    this->device = device;
    this->deviceName = deviceName;
    this->roomName = roomName;
}

FSDataHandler::~FSDataHandler() {
}

#endif  // DATA_HANDLER_1_FS
