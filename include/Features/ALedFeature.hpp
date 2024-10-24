#ifndef ALedFeature__
#define ALedFeature__

#include <FastLED.h>

#include <Features/ALedEffects/ALedEffects.hpp>
#include <General.hpp>

enum AnimationModes {
    kAnimationModeStatic,
    kAnimationModeRainbow,
    kAnimationModeFire,
    kAnimationModeFade,
    kAnimationModeBreathe,
    kAnimationModeBreathFade,
    kAnimationModeColorWipe,
    kAnimationModeTheaterChase,
    kAnimationModeTheaterChaseRainbow,
    kAnimationModeRunningLights,
    kAnimationModeTwinkle,
    kAnimationModeTwinkleRainbow,
};

namespace home {
template <uint8_t DATA_PIN>
class ALedFeature : public Feature {
   private:
    /* data */
    int numberOfLeds = 0;
    CRGB* leds = nullptr;
    uint8_t brightness = 50;
    uint8_t speed = 10;
    AnimationModes animationMode = kAnimationModeStatic;
    CRGB startColor = CRGB::Black;  // For static, wipe, chase, running lights, twinkle
    CRGB endColor = CRGB::Black;    // For fade, breath fade

   public:
    ALedFeature(Device& device, String name, int numberOfLeds, uint8_t brightness, AnimationModes animationMode);
    ~ALedFeature();

    void getData(const JsonObject& doc) {
        // doc["r"] = this->getR();
        // doc["g"] = this->getG();
        // doc["b"] = this->getB();
    }

    void execute(const JsonObjectConst& doc) {
        // if (doc.containsKey("r")) {
        //     this->setR(doc["r"]);
        // }
        // if (doc.containsKey("g")) {
        //     this->setG(doc["g"]);
        // }
        // if (doc.containsKey("b")) {
        //     this->setB(doc["b"]);
        // }
    }

    void loop() {
        switch (animationMode) {
            case kAnimationModeStatic:
                ALedEffects::staticColor(leds, numberOfLeds, startColor);
                break;
            case kAnimationModeRainbow:
                ALedEffects::rainbow(leds, numberOfLeds, millis() / 10, 7);
                break;
            case kAnimationModeFire:
                ALedEffects::fire(leds, numberOfLeds);
                break;
            case kAnimationModeFade:
                ALedEffects::fade(leds, numberOfLeds, startColor, endColor, millis() % 256);
                break;
            case kAnimationModeBreathe:
                ALedEffects::breathe(leds, numberOfLeds, startColor, brightness);
                break;
            case kAnimationModeBreathFade:
                ALedEffects::fade(leds, numberOfLeds, startColor, endColor, sin8(millis() / 4));
                break;
            case kAnimationModeColorWipe:
                ALedEffects::colorWipe(leds, numberOfLeds, startColor, 50);
                break;
            case kAnimationModeTheaterChase:
                ALedEffects::theaterChase(leds, numberOfLeds, startColor, 50);
                break;
            case kAnimationModeTheaterChaseRainbow:
                ALedEffects::theaterChaseRainbow(leds, numberOfLeds, 50);
                break;
            case kAnimationModeRunningLights:
                ALedEffects::runningLights(leds, numberOfLeds, startColor, 50);
                break;
            case kAnimationModeTwinkle:
                ALedEffects::twinkle(leds, numberOfLeds, startColor, 50);
                break;
            case kAnimationModeTwinkleRainbow:
                ALedEffects::twinkleRainbow(leds, numberOfLeds, 50);
                break;
        }
        FastLED.show();
    }
};

template <uint8_t DATA_PIN>
ALedFeature<DATA_PIN>::ALedFeature(Device& device, String name, int numberOfLeds, uint8_t brightness, AnimationModes animationMode) : Feature("ALedFeature", device, name) {
    this->numberOfLeds = numberOfLeds;
    this->leds = (CRGB*)malloc(numberOfLeds * sizeof(CRGB));
    this->brightness = brightness;
    this->animationMode = animationMode;
    FastLED.addLeds<WS2815, DATA_PIN, GRB>(this->leds, numberOfLeds);
    FastLED.setBrightness(brightness);
}

template <uint8_t DATA_PIN>
ALedFeature<DATA_PIN>::~ALedFeature() {
    free(this->leds);
}

}  // namespace home

#endif
