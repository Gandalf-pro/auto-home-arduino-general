#ifndef LightLevelFeature__
#define LightLevelFeature__

#include <General.hpp>

namespace home {
class LightLevelFeature : public Feature {
   private:
    /* data */
    int lightLevel = 0;
    const long A = 200;  // Dark resistance in KΩ
    const int B = 5;     // Light resistance (10 Lux) in KΩ
    const int Rc = 10;   // Calibration resistance in KΩ
    const int maxLightLevel = ((long)1023 * A * 10) / ((long)B * Rc * (1024 - 1023));

    // Pins
    uint8_t pin;

    int getLightLevel(int V) {
        return map(((long)V * A * 10) / ((long)B * Rc * (1024 - V)), 0, maxLightLevel, 0, 100);
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