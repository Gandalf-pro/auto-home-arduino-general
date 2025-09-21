#ifndef BreathFadeEffect__1
#define BreathFadeEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedEffects/ALedEffectParent.hpp>

namespace home {

class BreathFadeEffect : public ALedEffectParent {
   private:
    unsigned long startTime;

   public:
    BreathFadeEffect(ALeadDataClass* data) : ALedEffectParent(data, kAnimationModeBreathFade) {
        this->startTime = millis();
    }
    ~BreathFadeEffect() {}

    void loop() {
        ALeadDataClass* data = this->data;

        // Calculate breathing pattern using sine wave
        unsigned long elapsed = millis() - startTime;
        uint8_t blendAmount = sin8(elapsed / 4);

        // Blend between start and end colors based on the sine wave
        CRGB blendedColor = blend(data->startColor, data->endColor, blendAmount);

        // Apply to all LEDs
        fill_solid(data->leds, data->numberOfLeds, blendedColor);
        data->controller->showLeds(data->brightness);

        delay(data->delayMs);
    }

    void setup() {
        this->startTime = millis();
    }
};

}  // namespace home

#endif
