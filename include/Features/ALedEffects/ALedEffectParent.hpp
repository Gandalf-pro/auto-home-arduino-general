#ifndef ALedEffectParent__1
#define ALedEffectParent__1

#include <Arduino.h>

#include <Features/ALedFeature.hpp>

namespace home {

template <uint8_t DATA_PIN>
class ALedEffectParent {
   private:
    AnimationModes mode;
    ALedFeature<DATA_PIN>* ledFeature;

   public:
    ALedEffectParent(ALedFeature<DATA_PIN>* ledFeature, AnimationModes mode) {
        this->ledFeature = ledFeature;
        this->mode = mode;
    }
    ~ALedEffectParent() {
    }

    ALedFeature<DATA_PIN>* getLedFeature() {
        return this->ledFeature;
    }

    AnimationModes getAnimationMode() {
        return this->mode;
    }

    virtual void loop() {};
    virtual void setup() {};
};

}  // namespace home

#endif