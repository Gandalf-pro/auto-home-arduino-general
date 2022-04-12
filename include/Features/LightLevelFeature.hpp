#ifndef LightLevelFeature__
#define LightLevelFeature__

#include <General.hpp>

namespace home {
class LightLevelFeature : public Feature {
   private:
    /* data */
    int lightLevel = 0;

    // Pins
    uint8_t pin;

    int getLightLevel() {
        int sensorValue = analogRead(this->pin);
        return map(sensorValue, 0, 1023, 0, 100);
    }

   public:
    LightLevelFeature(Device& device, String name, uint8_t pin);
    ~LightLevelFeature();

    void getData(const JsonObject& doc) {
        doc["lightLevel"] = this->getLightLevel();
    }

    void execute(const JsonObjectConst& doc) {}
};

LightLevelFeature::LightLevelFeature(Device& device, String name, uint8_t pin) : Feature("LightLevelFeature", device, name) {
    this->pin = pin;
}

LightLevelFeature::~LightLevelFeature() {
}

}  // namespace home

#endif