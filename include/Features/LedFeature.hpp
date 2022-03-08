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

    String getData() {
        String message = "{";
        message += "\"r\":" + String(r);
        message += ", \"g\":" + String(g);
        message += ", \"b\":" + String(b);
        return message + "}";
    }

    String toJson(){
        String message = "{";
        message += "\"name\":" + this->getName();
        message += ",\"type\":" + this->type;
        message += ",\"data\":" + this->getData();
        return message + "}";
    }

    void execute(const ArduinoJson6192_F1::VariantConstRef& doc){
        if(doc.containsKey("r")){
            this->r = doc["r"];
        }
        if(doc.containsKey("g")){
            this->g = doc["g"];
        }
        if(doc.containsKey("b")){
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