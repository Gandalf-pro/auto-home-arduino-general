#ifndef AnimationModes__1
#define AnimationModes__1

#include <Arduino.h>

#include <Features/ALedFeature.hpp>

namespace home {
enum AnimationModes {
    kAnimationModeNone,
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

}  // namespace home

#endif