#ifndef ALedFeature__
#define ALedFeature__

#include <FastLED.h>
#include <ReactESP.h>

#include <Features/ALedEffects/ALeadDataClass.hpp>
#include <Features/ALedEffects/ALedEffects.hpp>
#include <General.hpp>

namespace home {

template <uint8_t DATA_PIN>
class ALedFeature : public Feature {
   private:
   public:
    ALeadDataClass* data;
    ALedEffectParent* currentEffect = nullptr;
    ALedFeature(Device& device, String name, int numberOfLeds);
    ~ALedFeature();

    void getData(const JsonObject& doc);
    void execute(const JsonObjectConst& doc);
    void setAnimationMode(AnimationModes mode);
    void setup();
    void loop();
};

}  // namespace home

namespace home {

template <uint8_t DATA_PIN>
ALedFeature<DATA_PIN>::ALedFeature(Device& device, String name, int numberOfLeds)
    : Feature("ALedFeature", device, name) {
    this->data = new ALeadDataClass(numberOfLeds);

    this->data->setController(&FastLED.addLeds<WS2815, DATA_PIN, RGB>(this->data->leds, numberOfLeds));
    this->data->controller->setCorrection(TypicalSMD5050);

    this->data->controller->clearLeds(numberOfLeds);
    delay(1);
    this->data->controller->showLeds(0);
}

template <uint8_t DATA_PIN>
ALedFeature<DATA_PIN>::~ALedFeature() {
    delete this->data;
}

template <uint8_t DATA_PIN>
void ALedFeature<DATA_PIN>::getData(const JsonObject& doc) {
    doc["animationMode"] = (uint8_t)this->data->animationMode;
    doc["speed"] = this->data->speed;
    doc["brightness"] = this->data->brightness;
    // Convert CRGB to hex
    char startColorHex[8];
    sprintf(startColorHex, "#%02X%02X%02X", this->data->startColor.r, this->data->startColor.g, this->data->startColor.b);
    doc["startColor"] = String(startColorHex);

    char endColorHex[8];
    sprintf(endColorHex, "#%02X%02X%02X", this->data->endColor.r, this->data->endColor.g, this->data->endColor.b);
    doc["endColor"] = String(endColorHex);
}

template <uint8_t DATA_PIN>
void ALedFeature<DATA_PIN>::execute(const JsonObjectConst& doc) {
    if (doc.containsKey("speed")) {
        this->data->speed = doc["speed"];
        if (this->data->speed == 0) {
            this->data->speed = 1;
        }

        this->data->delayMs = 1000 / this->data->speed;
    }
    if (doc.containsKey("brightness")) {
        this->data->brightness = doc["brightness"];
    }
    if (doc.containsKey("startColor")) {
        const char* colorString = doc["startColor"];
        int hasHash = 0;
        if (colorString[0] == '#') {
            hasHash = 1;
        }

        long color = strtol(colorString + hasHash, NULL, 16);

        this->data->startColor = color;
    }
    if (doc.containsKey("endColor")) {
        const char* colorString = doc["endColor"];
        int hasHash = 0;
        if (colorString[0] == '#') {
            hasHash = 1;
        }

        long color = strtol(colorString + hasHash, NULL, 16);

        this->data->endColor = color;
    }

    if (doc.containsKey("animationMode")) {
        this->setAnimationMode((AnimationModes)doc["animationMode"]);
    }
}

template <uint8_t DATA_PIN>
void ALedFeature<DATA_PIN>::setAnimationMode(AnimationModes mode) {
    this->data->animationMode = mode;

    if (this->currentEffect != nullptr) {
        if (this->currentEffect->getAnimationMode() == mode) {
            this->currentEffect->setup();
            return;
        } else {
            delete this->currentEffect;
            this->currentEffect = nullptr;
        }
    }

    switch (mode) {
        case kAnimationModeNone:
            this->data->controller->clearLeds(this->data->numberOfLeds);
            break;
        case kAnimationModeStatic:
            this->currentEffect = new StaticColorEffect(this->data);
            break;
        case kAnimationModeFade:
            this->currentEffect = new FadeColorEffect(this->data);
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
    if (this->currentEffect != nullptr) {
        this->currentEffect->loop();
        // this->data->eventLoop->tick();
    } else {
        delay(100);
    }
}

};  // namespace home

#endif
