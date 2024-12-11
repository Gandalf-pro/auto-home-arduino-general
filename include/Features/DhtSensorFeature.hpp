#pragma once
#include <General.hpp>

#include "DHT.h"

namespace home {
class DhtSensorFeature : public Feature {
   private:
    /* data */
    float humidity = 0;
    float temp = 0;
    float heatIndex = 0;

    unsigned long lastReadAt = 0;
    DHT* dht;

    bool readData() {
        // This will make sure we only take measurements every 30 seconds
        if (millis() > (lastReadAt + 30000)) {
            return true;
        }

        float h = this->dht->readHumidity();
        float t = this->dht->readTemperature();
        if (isnan(h) || isnan(t)) {
            Serial.println(F("Failed to read from DHT sensor!"));
            return false;
        }
        float hic = this->dht->computeHeatIndex(t, h, false);
        this->humidity = h;
        this->temp = t;
        this->heatIndex = hic;
        this->lastReadAt = millis();
        return true;
    }

   public:
    DhtSensorFeature(Device& device, String name, uint8_t pin) : Feature("DhtSensorFeature", device, name) {
        this->dht = new DHT(pin, DHT11);
    }
    ~DhtSensorFeature() {
        delete this->dht;
    }

    void setup() {
        this->dht->begin();
    }

    void getData(const JsonObject& doc) {
        doc["error"] = this->readData();
        doc["humidity"] = this->humidity;
        doc["temp"] = this->temp;
        doc["heatIndex"] = this->heatIndex;
    }

    void execute(const JsonObjectConst& doc) {}
};

}  // namespace home
