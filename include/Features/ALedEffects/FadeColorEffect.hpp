#ifndef FadeColorEffect__1
#define FadeColorEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedEffects/ALedEffectParent.hpp>

namespace home {

class FadeColorEffect : public ALedEffectParent {
   private:
    int lastStep = 0;

   public:
    FadeColorEffect(ALeadDataClass* data) : ALedEffectParent(data, kAnimationModeFade) {
    }
    ~FadeColorEffect() {}

    int getStep() {
        int old = this->lastStep;
        this->lastStep = (old + 1) % 256;
        return old;
    }

    void loop() {
        ALeadDataClass* data = this->data;
        uint8_t step = this->getStep();

        CRGB blendedColor = blend(data->startColor, data->endColor, step);
        for (int i = 0; i < data->numberOfLeds; ++i) {
            data->leds[i] = blendedColor;
        }
        data->controller->showLedsInternal(data->brightness);
        // CFastLED::delay();
        delay(this->data->delayMs);
    }

    void setup() {
    }
};

}  // namespace home

#endif