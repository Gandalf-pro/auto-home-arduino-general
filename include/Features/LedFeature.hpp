#ifndef LedFeature__
#define LedFeature__

#include <General.hpp>

namespace home {
class LedFeature : public Feature {
   private:
    /* data */
    String type = "LedFeature";
    uint8 r = 0;
    uint8 g = 0;
    uint8 b = 0;

   public:
    LedFeature(Device& device, String name);
    ~LedFeature();

    void getData(const JsonObject& doc) {
        doc["r"] = this->r;
        doc["g"] = this->g;
        doc["b"] = this->b;
    }

    void execute(const JsonObject& doc) {
        if (doc.containsKey("r")) {
            this->r = doc["r"];
        }
        if (doc.containsKey("g")) {
            this->g = doc["g"];
        }
        if (doc.containsKey("b")) {
            this->b = doc["b"];
        }
    }
};

LedFeature::LedFeature(Device& device, String name) : Feature(device, name) {
}

LedFeature::~LedFeature() {
}

}  // namespace home

#endif