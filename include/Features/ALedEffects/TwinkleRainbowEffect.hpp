#ifndef TwinkleRainbowEffect__1
#define TwinkleRainbowEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedEffects/ALedEffectParent.hpp>

namespace home {

class TwinkleRainbowEffect : public ALedEffectParent {
   private:
    uint8_t* fadeValues;  // Track fade values for each LED
    uint8_t* hues;        // Track hue values for each LED

   public:
    TwinkleRainbowEffect(ALeadDataClass* data) : ALedEffectParent(data, kAnimationModeTwinkleRainbow) {
        this->fadeValues = new uint8_t[data->numberOfLeds];
        this->hues = new uint8_t[data->numberOfLeds];
        memset(this->fadeValues, 0, data->numberOfLeds);
        memset(this->hues, 0, data->numberOfLeds);
    }

    ~TwinkleRainbowEffect() {
        delete[] this->fadeValues;
        delete[] this->hues;
    }

    void loop() {
        ALeadDataClass* data = this->data;

        // Clear all LEDs
        fill_solid(data->leds, data->numberOfLeds, CRGB::Black);

        // Fade existing twinkles
        for (int i = 0; i < data->numberOfLeds; i++) {
            if (this->fadeValues[i] > 0) {
                this->fadeValues[i] = qsub8(this->fadeValues[i], 10);
                data->leds[i] = CHSV(this->hues[i], 255, this->fadeValues[i]);
            }
        }

        // Randomly start new twinkles with random rainbow colors
        if (random8() < data->speed) {
            int ledIndex = random8(data->numberOfLeds);
            this->fadeValues[ledIndex] = 255;
            this->hues[ledIndex] = random8();
        }

        data->controller->showLeds(data->brightness);
        delay(data->delayMs);
    }

    void setup() {
        memset(this->fadeValues, 0, this->data->numberOfLeds);
        memset(this->hues, 0, this->data->numberOfLeds);
    }
};

}  // namespace home

#endif
