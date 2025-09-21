#ifndef ColorWipeEffect__1
#define ColorWipeEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedEffects/ALedEffectParent.hpp>

namespace home {

class ColorWipeEffect : public ALedEffectParent {
   private:
    int currentPosition;

   public:
    ColorWipeEffect(ALeadDataClass* data) : ALedEffectParent(data, kAnimationModeColorWipe) {
        this->currentPosition = 0;
    }
    ~ColorWipeEffect() {}

    void loop() {
        ALeadDataClass* data = this->data;

        // Clear all LEDs
        fill_solid(data->leds, data->numberOfLeds, CRGB::Black);

        // Light up LEDs up to current position
        for (int i = 0; i <= currentPosition && i < data->numberOfLeds; i++) {
            data->leds[i] = data->startColor;
        }

        data->controller->showLeds(data->brightness);

        // Move to next position
        currentPosition++;
        if (currentPosition >= data->numberOfLeds) {
            currentPosition = 0;
        }

        delay(data->delayMs);
    }

    void setup() {
        this->currentPosition = 0;
    }
};

}  // namespace home

#endif
