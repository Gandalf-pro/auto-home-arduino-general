#include <Features/ALedFeature.hpp>

namespace home {

template <uint8_t DATA_PIN>
ALedFeature<DATA_PIN>::ALedFeature(Device& device, String name, int numberOfLeds)
    : Feature("ALedFeature", device, name),
      numberOfLeds(numberOfLeds),
      isOn(false),
      brightness(128),
      speed(10),
      delayMs(1000 / speed),
      animationMode(kAnimationModeNone),
      startColor(CRGB::Black),
      endColor(CRGB::Black),
      currentEffect(nullptr) {
    this->eventLoop = new reactesp::EventLoop();
    this->leds = (CRGB*)malloc(numberOfLeds * sizeof(CRGB));
    FastLED.addLeds<WS2815, DATA_PIN, GRB>(this->leds, numberOfLeds);
    FastLED.setBrightness(this->brightness);
}

template <uint8_t DATA_PIN>
ALedFeature<DATA_PIN>::~ALedFeature() {
    free(this->leds);
    delete this->eventLoop;
}

template <uint8_t DATA_PIN>
void ALedFeature<DATA_PIN>::getData(const JsonObject& doc) {
    doc["isOn"] = this->isOn;
    doc["animationMode"] = this->animationMode;
    doc["speed"] = this->speed;
    doc["brightness"] = this->brightness;
    // Convert CRGB to hex
    char startColorHex[8];
    sprintf(startColorHex, "#%02X%02X%02X", startColor.r, startColor.g, startColor.b);
    doc["startColor"] = String(startColorHex);

    char endColorHex[8];
    sprintf(endColorHex, "#%02X%02X%02X", endColor.r, endColor.g, endColor.b);
    doc["endColor"] = String(endColorHex);
}

template <uint8_t DATA_PIN>
void ALedFeature<DATA_PIN>::execute(const JsonObjectConst& doc) {
    if (doc.containsKey("isOn")) {
        this->isOn = doc["isOn"];
    }
    if (doc.containsKey("animationMode")) {
        this->setAnimationMode((AnimationModes)doc["animationMode"]);
    }
    if (doc.containsKey("speed")) {
        this->speed = doc["speed"];
        this->delayMs = 1000 / this->speed;
    }
    if (doc.containsKey("brightness")) {
        this->brightness = doc["brightness"];
    }
    if (doc.containsKey("startColor")) {
        this->startColor = doc["startColor"];
    }
    if (doc.containsKey("endColor")) {
        this->endColor = doc["endColor"];
    }
}

template <uint8_t DATA_PIN>
void ALedFeature<DATA_PIN>::setAnimationMode(AnimationModes mode) {
    this->animationMode = mode;

    if (this->currentEffect != nullptr) {
        if (this->currentEffect->getAnimationMode() == mode) {
            return;
        } else {
            delete this->currentEffect;
        }
    }

    switch (mode) {
        case kAnimationModeStatic:
            this->currentEffect = new StaticColorEffect<DATA_PIN>(this);
            break;
        case kAnimationModeFade:
            this->currentEffect = new FadeColorEffect<DATA_PIN>(this);
            break;
            // case kAnimationModeRainbow:
            //     ALedEffects::rainbow(leds, numberOfLeds, millis() / 10, 7);
            //     break;
            // case kAnimationModeFire:
            //     ALedEffects::fire(leds, numberOfLeds);
            //     break;
            // case kAnimationModeBreathe:
            //     ALedEffects::breathe(leds, numberOfLeds, startColor, brightness);
            //     break;
            // case kAnimationModeBreathFade:
            //     ALedEffects::fade(leds, numberOfLeds, startColor, endColor, sin8(millis() / 4));
            //     break;
            // case kAnimationModeColorWipe:
            //     ALedEffects::colorWipe(leds, numberOfLeds, startColor, this->delayMs);
            //     break;
            // case kAnimationModeTheaterChase:
            //     ALedEffects::theaterChase(leds, numberOfLeds, startColor, this->delayMs);
            //     break;
            // case kAnimationModeTheaterChaseRainbow:
            //     ALedEffects::theaterChaseRainbow(leds, numberOfLeds, this->delayMs);
            //     break;
            // case kAnimationModeRunningLights:
            //     ALedEffects::runningLights(leds, numberOfLeds, startColor, this->delayMs);
            //     break;
            // case kAnimationModeTwinkle:
            //     ALedEffects::twinkle(leds, numberOfLeds, startColor, this->delayMs);
            //     break;
            // case kAnimationModeTwinkleRainbow:
            //     ALedEffects::twinkleRainbow(leds, numberOfLeds, this->delayMs);
            //     break;
    }
}

template <uint8_t DATA_PIN>
void ALedFeature<DATA_PIN>::setup() {
}

template <uint8_t DATA_PIN>
void ALedFeature<DATA_PIN>::loop() {
    if (!this->isOn) {
        return;
    }
    if (this->currentEffect != nullptr) {
        this->currentEffect->loop();
    }
    this->eventLoop->tick();
}

};  // namespace home