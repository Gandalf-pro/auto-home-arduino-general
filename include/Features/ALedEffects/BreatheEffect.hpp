#ifndef BreatheEffect__1
#define BreatheEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedEffects/ALedEffectParent.hpp>

namespace home {

class BreatheEffect : public ALedEffectParent {
   private:
    unsigned long startTime;

   public:
    BreatheEffect(ALeadDataClass* data) : ALedEffectParent(data, kAnimationModeBreathe) {
        this->startTime = millis();
    }
    ~BreatheEffect() {}

    void loop() {
        ALeadDataClass* data = this->data;

        // Calculate breathing pattern using sine wave
        unsigned long elapsed = millis() - startTime;
        uint8_t brightness = (sin8(elapsed / 4) * data->brightness) / 255;

        // Apply the breathing brightness to all LEDs
        fill_solid(data->leds, data->numberOfLeds, data->startColor);
        data->controller->showLeds(brightness);

        delay(data->delayMs);
    }

    void setup() {
        this->startTime = millis();
    }
};

}  // namespace home

#endif
