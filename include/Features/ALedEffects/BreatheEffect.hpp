#ifndef BreatheEffect__1
#define BreatheEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedEffects/ALedEffectParent.hpp>

namespace home {

class BreatheEffect : public ALedEffectParent {
   private:
    uint8_t phase;

   public:
    BreatheEffect(ALeadDataClass* data) : ALedEffectParent(data, kAnimationModeBreathe) {
        this->phase = 0;
    }
    ~BreatheEffect() {}

    void loop() {
        ALeadDataClass* data = this->data;

        // Update phase for smooth breathing animation
        phase += data->speed;

        // Calculate breathing brightness using sine wave (0-255 range)
        uint8_t sineValue = sin8(phase);

        // Scale the sine value to the desired brightness range
        // This goes from 0 to data->brightness
        uint8_t breathBrightness = scale8(sineValue, data->brightness);

        // Apply the breathing brightness to all LEDs
        fill_solid(data->leds, data->numberOfLeds, data->startColor);
        data->controller->showLeds(breathBrightness);

        delay(data->delayMs);
    }

    void setup() {
        this->phase = 0;
    }
};

}  // namespace home

#endif
