#ifndef TwinkleEffect__1
#define TwinkleEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedEffects/ALedEffectParent.hpp>

namespace home {

class TwinkleEffect : public ALedEffectParent {
   private:
    uint8_t* fadeValues;  // Track fade values for each LED

   public:
    TwinkleEffect(ALeadDataClass* data) : ALedEffectParent(data, kAnimationModeTwinkle) {
        this->fadeValues = new uint8_t[data->numberOfLeds];
        memset(this->fadeValues, 0, data->numberOfLeds);
    }

    ~TwinkleEffect() {
        delete[] this->fadeValues;
    }

    void loop() {
        ALeadDataClass* data = this->data;

        // Clear all LEDs
        fill_solid(data->leds, data->numberOfLeds, CRGB::Black);

        // Fade existing twinkles
        for (int i = 0; i < data->numberOfLeds; i++) {
            if (this->fadeValues[i] > 0) {
                this->fadeValues[i] = qsub8(this->fadeValues[i], 10);
                data->leds[i] = data->startColor;
                data->leds[i].fadeToBlackBy(255 - this->fadeValues[i]);
            }
        }

        // Randomly start new twinkles
        if (random8() < data->speed) {
            int ledIndex = random8(data->numberOfLeds);
            this->fadeValues[ledIndex] = 255;
        }

        data->controller->showLeds(data->brightness);
        delay(data->delayMs);
    }

    void setup() {
        memset(this->fadeValues, 0, this->data->numberOfLeds);
    }
};

}  // namespace home

#endif
