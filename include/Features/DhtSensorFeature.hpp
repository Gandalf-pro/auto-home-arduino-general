#pragma once
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <General.hpp>

namespace home {
class DhtSensorFeature : public Feature {
   private:
    /* data */
    float humidity = 0;
    float temp = 0;
    float heatIndex = 0;

    unsigned long lastReadAt = 0;
    DHT* dht;

    int readData() {
        // This will make sure we only take measurements every 30 seconds
        if ((millis() - this->lastReadAt) < 30000) {
            return 1;
        }

        float h = this->dht->readHumidity();
        float t = this->dht->readTemperature();
        if (isnan(h) || isnan(t)) {
            Serial.println(F("Failed to read from DHT sensor!"));
            return 0;
        }
        float hic = this->dht->computeHeatIndex(t, h, false);
        this->humidity = h;
        this->temp = t;
        this->heatIndex = hic;
        this->lastReadAt = millis();
        return 2;
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
        doc["current"] = millis();
        doc["status"] = this->readData();
        doc["lastReadAt"] = this->lastReadAt;
        doc["humidity"] = this->humidity;
        doc["temp"] = this->temp;
        doc["heatIndex"] = this->heatIndex;
    }

    void execute(const JsonObjectConst& doc) {}
};

}  // namespace home
