#ifndef LedFeature__
#define LedFeature__

#include <General.hpp>

namespace home {
class LedFeature : public Feature {
   private:
    /* data */
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    // Pins
    uint8_t rPin;
    uint8_t gPin;
    uint8_t bPin;

    int rgbToPwmSignal(uint8_t color) {
        long fetOffset = map(color, 0, 255, 0, 64);
        return map(fetOffset, 0, 255, 0, 1023);
    }

   public:
    LedFeature(Device& device, String name, uint8_t rPin, uint8_t gPin, uint8_t bPin);
    ~LedFeature();

    uint8_t getR() {
        return this->r;
    }
    uint8_t getG() {
        return this->g;
    }
    uint8_t getB() {
        return this->b;
    }
    void setR(uint8_t r) {
        analogWrite(this->rPin, rgbToPwmSignal(r));
        this->r = r;
    }
    void setG(uint8_t g) {
        analogWrite(this->gPin, rgbToPwmSignal(g));
        this->g = g;
    }
    void setB(uint8_t b) {
        analogWrite(this->bPin, rgbToPwmSignal(b));
        this->b = b;
    }

    void getData(const JsonObject& doc) {
        doc["r"] = this->getR();
        doc["g"] = this->getG();
        doc["b"] = this->getB();
    }

    void execute(const JsonObjectConst& doc) {
        if (doc.containsKey("r")) {
            this->setR(doc["r"]);
        }
        if (doc.containsKey("g")) {
            this->setG(doc["g"]);
        }
        if (doc.containsKey("b")) {
            this->setB(doc["b"]);
        }
    }
};

LedFeature::LedFeature(Device& device, String name, uint8_t rPin, uint8_t gPin, uint8_t bPin) : Feature("LedFeature", device, name) {
    this->rPin = rPin;
    this->gPin = gPin;
    this->bPin = bPin;
    analogWrite(rPin, 0);
    analogWrite(gPin, 0);
    analogWrite(bPin, 0);
}

LedFeature::~LedFeature() {
}

}  // namespace home

#endif