#ifndef StaticColorEffect__1
#define StaticColorEffect__1

#include <Arduino.h>
#include <FastLED.h>

#include <Features/ALedEffects/ALedEffectParent.hpp>
#include <Features/ALedFeature.hpp>

namespace home {

template <uint8_t DATA_PIN>
class StaticColorEffect : public ALedEffectParent<DATA_PIN> {
   private:
   public:
    StaticColorEffect(ALedFeature<DATA_PIN>* ledFeature) : ALedEffectParent<DATA_PIN>(ledFeature, kAnimationModeStatic) {
    }
    ~StaticColorEffect() {}

    void loop() {
        FastLED.show();
    }
    void setup() {
        ALedFeature<DATA_PIN>* feat = this->getLedFeature();

        fill_solid(feat->leds, feat->numberOfLeds, feat->startColor);
    }
};

}  // namespace home

#endif