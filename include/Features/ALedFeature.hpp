#ifndef ALedFeature__
#define ALedFeature__

#include <FastLED.h>
#include <ReactESP.h>

#include <Features/ALedEffects/AnimationModes.hpp>
#include <General.hpp>

namespace home {
template <uint8_t DATA_PIN>
class ALedEffectParent;

template <uint8_t DATA_PIN>
class ALedFeature : public Feature {
   private:
    int numberOfLeds;
    bool isOn;
    CRGB* leds;
    uint8_t brightness;
    uint8_t speed;
    int delayMs;
    AnimationModes animationMode;
    CRGB startColor;
    CRGB endColor;
    reactesp::EventLoop* eventLoop;
    ALedEffectParent<DATA_PIN>* currentEffect;

   public:
    ALedFeature(Device& device, String name, int numberOfLeds);
    ~ALedFeature();

    void getData(const JsonObject& doc);
    void execute(const JsonObjectConst& doc);
    void setAnimationMode(AnimationModes mode);
    void setup();
    void loop();
};

}  // namespace home

#include <Features/ALedEffects/ALedEffects.hpp>

#endif
