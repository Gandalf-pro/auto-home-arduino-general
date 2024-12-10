#ifndef StaticColorEffect__1
#define StaticColorEffect__1

#include <Arduino.h>
#include <FastLED.h>
#include <ReactESP.h>

#include <Features/ALedFeature.hpp>

namespace home {

class StaticColorEffect : public ALedEffectParent {
   private:
    reactesp::RepeatEvent* event = nullptr;

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
        ALeadDataClass* data = this->getData();

        fill_solid(data->leds, data->numberOfLeds, data->startColor);

        this->data->controller->showLedsInternal(this->data->brightness);

        // if (this->event != nullptr) {
        //     this->getData()->eventLoop->remove(this->event);
        // }
        // this->event = this->getData()->eventLoop->onRepeat(16, std::bind(&StaticColorEffect::runEventLoop, this));
    }
};

}  // namespace home

#endif