#ifndef RunningLightsEffect__1
#define RunningLightsEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedEffects/ALedEffectParent.hpp>

namespace home {

class RunningLightsEffect : public ALedEffectParent {
   private:
    int position = 0;

   public:
    RunningLightsEffect(ALeadDataClass* data) : ALedEffectParent(data, kAnimationModeRunningLights) {
        this->position = 0;
    }
    ~RunningLightsEffect() {}

    void loop() {
        ALeadDataClass* data = this->data;

        // Clear all LEDs
        fill_solid(data->leds, data->numberOfLeds, CRGB::Black);

        // Calculate the position of the running light
        int pos = position;

        // Create running light effect with fading trail
        for (int i = 0; i < data->numberOfLeds; i++) {
            // Calculate distance from current position
            int distance = abs(i - pos);
            if (distance < 5) {  // Trail length
                // Fade based on distance
                uint8_t intensity = 255 - (distance * 51);  // 255/5 = 51
                data->leds[i] = data->startColor;
                data->leds[i].fadeToBlackBy(255 - intensity);
            }
        }

        data->controller->showLeds(data->brightness);

        // Move position
        position += data->speed / 5 + 1;  // Speed control
        if (position >= data->numberOfLeds) {
            position = 0;
        }

        delay(data->delayMs);
    }

    void setup() {
        // Initial setup can be empty
    }
};

}  // namespace home

#endif
