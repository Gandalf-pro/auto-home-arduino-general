#ifndef TheaterChaseRainbowEffect__1
#define TheaterChaseRainbowEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedEffects/ALedEffectParent.hpp>

namespace home {

class TheaterChaseRainbowEffect : public ALedEffectParent {
   private:
    int chasePosition;
    uint8_t hueOffset;

   public:
    TheaterChaseRainbowEffect(ALeadDataClass* data) : ALedEffectParent(data, kAnimationModeTheaterChaseRainbow) {
        this->chasePosition = 0;
        this->hueOffset = 0;
    }
    ~TheaterChaseRainbowEffect() {}

    void loop() {
        ALeadDataClass* data = this->data;

        // Clear all LEDs
        fill_solid(data->leds, data->numberOfLeds, CRGB::Black);

        // Light up every 3rd LED with rainbow colors
        for (int i = chasePosition; i < data->numberOfLeds; i += 3) {
            uint8_t hue = hueOffset + (i * 255 / data->numberOfLeds);
            data->leds[i] = CHSV(hue, 255, 255);
        }

        data->controller->showLeds(data->brightness);

        // Move chase position
        chasePosition = (chasePosition + 1) % 3;

        // Update hue for rainbow cycling
        hueOffset += data->speed;

        delay(data->delayMs);
    }

    void setup() {
        this->chasePosition = 0;
        this->hueOffset = 0;
    }
};

}  // namespace home

#endif
