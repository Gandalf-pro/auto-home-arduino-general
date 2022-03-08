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

    String getData() {
        String message = "{";
        message += "\"lightLevel\":" + String(lightLevel);
        return message + "}";
    }

    String toJson(){
        String message = "{";
        message += "\"name\":" + this->getName();
        message += ",\"type\":" + this->type;
        message += ",\"data\":" + this->getData();
        return message + "}";
    }
};

LightLevelFeature::LightLevelFeature(Device& device, String name) : Feature(device, name) {
}

LightLevelFeature::~LightLevelFeature() {
}

}  // namespace home

#endif