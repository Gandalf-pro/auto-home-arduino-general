#ifndef FadeColorEffect__1
#define FadeColorEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedEffects/ALedEffectParent.hpp>

namespace home {

class FadeColorEffect : public ALedEffectParent {
   private:
    int lastStep = 0;
    bool isIncreasing = true;

   public:
    FadeColorEffect(ALeadDataClass* data) : ALedEffectParent(data, kAnimationModeFade) {
        this->lastStep = 0;
        this->isIncreasing = true;
    }
    ~FadeColorEffect() {}

    int getStep() {
        int old = this->lastStep;
        int newStep = old + (this->isIncreasing ? 1 : -1);

        if (newStep > 255) {
            this->isIncreasing = !this->isIncreasing;
            newStep = 254;
        }
        if (newStep < 0) {
            this->isIncreasing = !this->isIncreasing;
            newStep = 1;
        }

        this->lastStep = newStep;
        return old;
    }

    void loop() {
        ALeadDataClass* data = this->data;
        uint8_t step = this->getStep();

        CRGB blendedColor = blend(data->startColor, data->endColor, step);
        for (int i = 0; i < data->numberOfLeds; ++i) {
            data->leds[i] = blendedColor;
        }
        data->controller->showLeds(data->brightness);
        delay(this->data->delayMs);
        // unsigned long start = millis();
        // do {
        //     delay(1);
        //     data->controller->showLeds(data->brightness);
        //     yield();
        // } while ((millis() - start) < this->data->delayMs);
    }

    void setup() {
    }
};

}  // namespace home

#endif