#ifndef FireEffect__1
#define FireEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedEffects/ALedEffectParent.hpp>

namespace home {

class FireEffect : public ALedEffectParent {
   private:
    uint8_t* heat;  // Array to store heat values for each LED

   public:
    FireEffect(ALeadDataClass* data) : ALedEffectParent(data, kAnimationModeFire) {
        this->heat = new uint8_t[data->numberOfLeds];
        memset(this->heat, 0, data->numberOfLeds);
    }

    ~FireEffect() {
        delete[] this->heat;
    }

    void loop() {
        ALeadDataClass* data = this->data;

        // Cool down every cell a little
        for (int i = 0; i < data->numberOfLeds; i++) {
            this->heat[i] = qsub8(this->heat[i], random8(0, ((data->speed * 10) / data->numberOfLeds) + 2));
        }

        // Heat from each cell drifts 'up' and diffuses a little
        for (int k = data->numberOfLeds - 1; k >= 2; k--) {
            this->heat[k] = (this->heat[k - 1] + this->heat[k - 2] + this->heat[k - 2]) / 3;
        }

        // Randomly ignite new 'sparks' of heat near the bottom
        if (random8() < data->speed) {
            int y = random8(7);
            this->heat[y] = qadd8(this->heat[y], random8(160, 255));
        }

        // Convert heat to LED colors
        for (int j = 0; j < data->numberOfLeds; j++) {
            CRGB color = HeatColor(this->heat[j]);
            data->leds[j] = color;
        }

        data->controller->showLeds(data->brightness);
        delay(data->delayMs);
    }

    void setup() {
        // Initialize with some random heat
        for (int i = 0; i < this->data->numberOfLeds; i++) {
            this->heat[i] = random8(0, 128);
        }
    }
};

}  // namespace home

#endif
