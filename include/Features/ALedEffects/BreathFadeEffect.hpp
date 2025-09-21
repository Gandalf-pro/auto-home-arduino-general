#ifndef BreathFadeEffect__1
#define BreathFadeEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedEffects/ALedEffectParent.hpp>

namespace home {

class BreathFadeEffect : public ALedEffectParent {
   private:
    uint8_t phase;

   public:
    BreathFadeEffect(ALeadDataClass* data) : ALedEffectParent(data, kAnimationModeBreathFade) {
        this->phase = 0;
    }
    ~BreathFadeEffect() {}

    void loop() {
        ALeadDataClass* data = this->data;

        // Update phase for smooth breathing animation
        phase += data->speed;

        // Calculate breathing blend amount using sine wave (0-255 range)
        uint8_t blendAmount = sin8(phase);

        // Blend between start and end colors based on the sine wave
        CRGB blendedColor = blend(data->startColor, data->endColor, blendAmount);

        // Apply to all LEDs
        fill_solid(data->leds, data->numberOfLeds, blendedColor);
        data->controller->showLeds(data->brightness);

        delay(data->delayMs);
    }

    void setup() {
        this->phase = 0;
    }
};

}  // namespace home

#endif
