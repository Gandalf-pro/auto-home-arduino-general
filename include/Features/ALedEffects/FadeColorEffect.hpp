#ifndef FadeColorEffect__1
#define FadeColorEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedEffects/ALedEffectParent.hpp>

namespace home {

template <uint8_t DATA_PIN>
class FadeColorEffect : public ALedEffectParent<DATA_PIN> {
   private:
   public:
    FadeColorEffect(ALedFeature<DATA_PIN>* ledFeature) : ALedEffectParent<DATA_PIN>(ledFeature, kAnimationModeFade) {
    }
    ~FadeColorEffect() {}

    void loop() {
        ALedFeature<DATA_PIN>* feat = this->getLedFeature();
        uint8_t step = millis() % 256;
        for (int i = 0; i < feat->numberOfLeds; ++i) {
            feat->leds[i] = blend(feat->startColor, feat->endColor, step);
        }
        FastLED.show();
    }

    void setup() {
    }
};

}  // namespace home

#endif