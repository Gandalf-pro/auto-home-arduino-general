#ifndef LightLevelFeature__
#define LightLevelFeature__

#include <General.hpp>

namespace home {
class LightLevelFeature : public Feature {
   private:
    /* data */
    String type = "LightLevelFeature";
    int lightLevel = 0;

   public:
    LightLevelFeature(Device& device, String name);
    ~LightLevelFeature();

    void getData(const JsonObject& doc) {
        doc["lightLevel"] = this->lightLevel;
    }

};

LightLevelFeature::LightLevelFeature(Device& device, String name) : Feature(device, name) {
}

LightLevelFeature::~LightLevelFeature() {
}

}  // namespace home

#endif