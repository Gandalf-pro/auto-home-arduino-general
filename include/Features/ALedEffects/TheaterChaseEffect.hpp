#ifndef TheaterChaseEffect__1
#define TheaterChaseEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedEffects/ALedEffectParent.hpp>

namespace home {

class TheaterChaseEffect : public ALedEffectParent {
   private:
    int chasePosition;

   public:
    TheaterChaseEffect(ALeadDataClass* data) : ALedEffectParent(data, kAnimationModeTheaterChase) {
        this->chasePosition = 0;
    }
    ~TheaterChaseEffect() {}

    void loop() {
        ALeadDataClass* data = this->data;

        // Clear all LEDs
        fill_solid(data->leds, data->numberOfLeds, CRGB::Black);

        // Light up every 3rd LED starting from chasePosition
        for (int i = chasePosition; i < data->numberOfLeds; i += 3) {
            data->leds[i] = data->startColor;
        }

        data->controller->showLeds(data->brightness);

        // Move chase position
        chasePosition = (chasePosition + 1) % 3;

        delay(data->delayMs);
    }

    void setup() {
        this->chasePosition = 0;
    }
};

}  // namespace home

#endif
