#ifndef LightFeature__
#define LightFeature__

#include <General.hpp>

namespace home {
class LightFeature : public Feature {
   private:
    /* data */
    bool state = false;

    // Pins
    uint8_t relayPin;

   public:
    LightFeature(Device& device, String name, uint8_t relayPin);
    ~LightFeature();

    void getData(const JsonObject& doc) {
        doc["state"] = this->state;
    }

    void setState(bool state) {
        if (state && !this->state) {
            Serial.println("Setting relay to high");
            digitalWrite(this->relayPin, HIGH);
        } else if (!state && this->state) {
            Serial.println("Setting relay to low");
            digitalWrite(this->relayPin, LOW);
        }
        this->state = state;
    }

    void execute(const JsonObjectConst& doc) {
        if (doc.containsKey("state")) {
            this->setState(doc["state"]);
        }
    }
};

LightFeature::LightFeature(Device& device, String name, uint8_t relayPin) : Feature("LightFeature", device, name) {
    this->relayPin = relayPin;
}

LightFeature::~LightFeature() {
}

}  // namespace home

#endif