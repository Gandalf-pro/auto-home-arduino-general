#ifndef ALedEffectParent__1
#define ALedEffectParent__1

#include <Arduino.h>

#include <Features/ALedEffects/ALeadDataClass.hpp>

namespace home {

class ALedEffectParent {
   private:
    AnimationModes mode;

   public:
    ALeadDataClass* data;
    ALedEffectParent(ALeadDataClass* data, AnimationModes mode) {
        this->data = data;
        this->mode = mode;
    }
    ~ALedEffectParent() {
    }

    ALeadDataClass* getData() {
        return this->data;
    }

    AnimationModes getAnimationMode() {
        return this->mode;
    }

    virtual void loop() {};
    virtual void setup() {};
};

}  // namespace home

#endif