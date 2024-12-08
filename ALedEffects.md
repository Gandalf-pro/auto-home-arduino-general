#ifndef ALED_EFFECTS_HPP
#define ALED_EFFECTS_HPP

#include <FastLED.h>
#include <ReactESP.h>

namespace home {

class ALedEffects {
   public:
    static void staticColor(CRGB* leds, int numLeds, CRGB color) {
        fill_solid(leds, numLeds, color);
        FastLED.show();
    }

    static void fade(reactesp::EventLoop* event_loop, CRGB* leds, int numLeds, CRGB startColor, CRGB endColor, int duration) {
        uint8_t step = 0;
        event_loop->onRepeat(duration / 255, [=]() mutable {
            for (int i = 0; i < numLeds; ++i) {
                leds[i] = blend(startColor, endColor, step);
            }
            FastLED.show();
            step++;
            if (step >= 255) {
                event_loop->cancelOnRepeat();
            }
        });
    }

    static void breathe(reactesp::EventLoop* event_loop, CRGB* leds, int numLeds, CRGB color, uint8_t brightness) {
        event_loop->onRepeat(16, [=]() {
            uint8_t breath = sin8(millis() / 4);
            fill_solid(leds, numLeds, color.nscale8(breath));
            FastLED.setBrightness(brightness);
            FastLED.show();
        });
    }

    static void colorWipe(reactesp::EventLoop* event_loop, CRGB* leds, int numLeds, CRGB color, int delayMs) {
        int i = 0;
        event_loop->onRepeat(delayMs, [=]() mutable {
            if (i < numLeds) {
                leds[i] = color;
                FastLED.show();
                i++;
            } else {
                event_loop->cancelOnRepeat();
            }
        });
    }

    static void theaterChase(reactesp::EventLoop* event_loop, CRGB* leds, int numLeds, CRGB color, int delayMs) {
        int q = 0;
        event_loop->onRepeat(delayMs, [=]() mutable {
            for (int i = 0; i < numLeds; i += 3) {
                leds[i + q] = color;
            }
            FastLED.show();
            event_loop->onDelay(delayMs / 2, [=]() {
                for (int i = 0; i < numLeds; i += 3) {
                    leds[i + q] = CRGB::Black;
                }
            });
            q = (q + 1) % 3;
        });
    }

    static void rainbow(reactesp::EventLoop* event_loop, CRGB* leds, int numLeds, uint8_t initialHue, uint8_t deltaHue) {
        event_loop->onRepeat(50, [=]() mutable {
            fill_rainbow(leds, numLeds, initialHue, deltaHue);
            FastLED.show();
            initialHue += deltaHue;
        });
    }
    static void fire(reactesp::EventLoop* event_loop, CRGB* leds, int numLeds) {
        static byte heat[256];
        event_loop->onRepeat(15, [=]() {
            for (int i = 0; i < numLeds; i++) {
                heat[i] = qsub8(heat[i], random8(0, ((55 * 10) / numLeds) + 2));
            }
            for (int k = numLeds - 1; k >= 2; k--) {
                heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
            }
            if (random8() < 120) {
                int y = random8(7);
                heat[y] = qadd8(heat[y], random8(160, 255));
            }
            for (int j = 0; j < numLeds; j++) {
                byte colorindex = scale8(heat[j], 240);
                leds[j] = ColorFromPalette(HeatColors_p, colorindex);
            }
            FastLED.show();
        });
    }

    static void runningLights(CRGB* leds, int numLeds, CRGB color, int waveDelay) {
        int position = 0;
        for (int i = 0; i < numLeds; i++) {
            leds[i] = color * ((sin8(position + i * 8) / 255.0) * 0.5 + 0.5);
        }
        FastLED.show();
        delay(waveDelay);
    }

    static void twinkle(CRGB* leds, int numLeds, CRGB color, int twinkleDelay) {
        for (int i = 0; i < numLeds; i++) {
            if (random8() < 50) {
                leds[i] = color;
            } else {
                leds[i] = CRGB::Black;
            }
        }
        FastLED.show();
        delay(twinkleDelay);
    }

    static void twinkleRainbow(CRGB* leds, int numLeds, int twinkleDelay) {
        for (int i = 0; i < numLeds; i++) {
            if (random8() < 50) {
                leds[i] = CHSV(random8(), 255, 255);
            } else {
                leds[i] = CRGB::Black;
            }
        }
        FastLED.show();
        delay(twinkleDelay);
    }

    static void fire(CRGB* leds, int numLeds) {
        static byte heat[256];
        for (int i = 0; i < numLeds; i++) {
            heat[i] = qsub8(heat[i], random8(0, ((55 * 10) / numLeds) + 2));
        }
        for (int k = numLeds - 1; k >= 2; k--) {
            heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
        }
        if (random8() < 120) {
            int y = random8(7);
            heat[y] = qadd8(heat[y], random8(160, 255));
        }
        for (int j = 0; j < numLeds; j++) {
            byte colorindex = scale8(heat[j], 240);
            leds[j] = ColorFromPalette(HeatColors_p, colorindex);
        }
        FastLED.show();
        delay(15);
    }

    static void fade(CRGB* leds, int numLeds, CRGB startColor, CRGB endColor, uint8_t progress) {
        for (int i = 0; i < numLeds; i++) {
            leds[i] = blend(startColor, endColor, progress);
        }
        FastLED.show();
    }

    static void breathe(CRGB* leds, int numLeds, CRGB color, uint8_t brightness) {
        uint8_t breath = sin8(millis() / 4);
        for (int i = 0; i < numLeds; i++) {
            leds[i] = color;
            leds[i].nscale8(breath);
        }
        FastLED.setBrightness(brightness);
        FastLED.show();
    }

    static void colorWipe(CRGB* leds, int numLeds, CRGB color, int delayMs) {
        for (int i = 0; i < numLeds; i++) {
            leds[i] = color;
            FastLED.show();
            delay(delayMs);
        }
    }

    static void theaterChase(CRGB* leds, int numLeds, CRGB color, int delayMs) {
        for (int q = 0; q < 3; q++) {
            for (int i = 0; i < numLeds; i += 3) {
                leds[i + q] = color;
            }
            FastLED.show();
            delay(delayMs);
            for (int i = 0; i < numLeds; i += 3) {
                leds[i + q] = CRGB::Black;
            }
        }
    }
};

}  // namespace home

#endif  // ALED_
