#pragma once

#include <FastLED.h>
#include <ReactESP.h>

#include <Features/ALedEffects/AnimationModes.hpp>

namespace home {
class ALeadDataClass {
   public:
    int numberOfLeds;
    CRGB* leds;
    uint8_t brightness = 128;
    uint8_t speed = 5;
    int delayMs = 1000 / 5;
    AnimationModes animationMode = kAnimationModeNone;
    CRGB startColor;
    CRGB endColor;
    reactesp::EventLoop* eventLoop;
    CLEDController* controller;

    ALeadDataClass(int numberOfLeds) {
        this->numberOfLeds = numberOfLeds;
        this->leds = (CRGB*)malloc(numberOfLeds * sizeof(CRGB));
        this->eventLoop = new reactesp::EventLoop();
        this->startColor = CRGB::Black;
        this->endColor = CRGB::Black;
    }

    void setController(CLEDController* controller) {
        this->controller = controller;
    }

    ~ALeadDataClass() {
        delete this->eventLoop;
        free(this->leds);
    }
};

};  // namespace home