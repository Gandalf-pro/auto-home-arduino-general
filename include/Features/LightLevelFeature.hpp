#ifndef LightLevelFeature__
#define LightLevelFeature__

#include <General.hpp>

int maxLightLevel = 1200;

namespace home {
class LightLevelFeature : public Feature {
   private:
    /* data */
    int lightLevel = 0;

    // Pins
    uint8_t pin;

    int getLightLevel(int V) {
        if (V > maxLightLevel) {
            V = maxLightLevel;
        }
        return map(V, 0, maxLightLevel, 0, 100);
    }

   public:
    LightLevelFeature(Device& device, String name, uint8_t pin);
    ~LightLevelFeature();

    void getData(const JsonObject& doc) {
        int res = analogRead(this->pin);
        doc["original"] = res;
        doc["lightLevel"] = this->getLightLevel(res);
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