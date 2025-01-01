#ifndef StaticColorEffect__1
#define StaticColorEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedFeature.hpp>

namespace home {

class StaticColorEffect : public ALedEffectParent {
   private:
   public:
    StaticColorEffect(ALeadDataClass* data) : ALedEffectParent(data, kAnimationModeStatic) {
        this->setup();
    }
    ~StaticColorEffect() {}

    void loop() {
    }

    void runEventLoop() {
        // this->data->controller->showLedsInternal(this->data->brightness);
    }

    void setup() {
        ALeadDataClass* dataLocal = this->getData();

        fill_solid(dataLocal->leds, dataLocal->numberOfLeds, dataLocal->startColor);
        dataLocal->controller->showLeds(dataLocal->brightness);
        delay(1);
        dataLocal->controller->showLeds(dataLocal->brightness);
    }
};

}  // namespace home

#endif