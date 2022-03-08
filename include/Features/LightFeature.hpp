#ifndef LightFeature__
#define LightFeature__

#include <General.hpp>

namespace home {
class LightFeature : public Feature {
   private:
    /* data */
    bool state = false;
    String type = "LightFeature";

    // Pins
    uint8 relayPin;

   public:
    LightFeature(Device& device, String name, uint8 relayPin);
    ~LightFeature();

    String getData() {
        String message = "{";
        message += "\"state\":" + String(state);
        return message + "}";
    }

    String toJson(){
        StaticJsonDocument<256> doc;
        doc["name"] = this->getName();
        doc["type"] = this->type;
        auto data = doc.createNestedObject("data");

        data["state"] = this->state;

        String output;
        serializeJson(doc, output);
        return output;
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

    void execute(const ArduinoJson6192_F1::VariantConstRef& doc) {
        if (doc.containsKey("state")) {
            this->setState(doc["state"]);
        }
    }
};

LightFeature::LightFeature(Device& device, String name, uint8 relayPin) : Feature(device, name) {
    this->relayPin = relayPin;
}

LightFeature::~LightFeature() {
}

}  // namespace home

#endif