#ifndef RainbowEffect__1
#define RainbowEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedEffects/ALedEffectParent.hpp>

namespace home {

class RainbowEffect : public ALedEffectParent {
   private:
    uint8_t hueOffset = 0;

   public:
    RainbowEffect(ALeadDataClass* data) : ALedEffectParent(data, kAnimationModeRainbow) {
        this->hueOffset = 0;
    }
    ~RainbowEffect() {}

    void loop() {
        ALeadDataClass* data = this->data;

        // Create rainbow pattern across all LEDs
        fill_rainbow(data->leds, data->numberOfLeds, hueOffset, 255 / data->numberOfLeds);

        // Apply brightness
        data->controller->showLeds(data->brightness);

        // Update hue offset for next frame
        hueOffset += data->speed;

        // Add delay between frames
        delay(data->delayMs);
    }

    void setup() {
        // Initial setup can be empty for rainbow effect
    }
};

}  // namespace home

#endif
