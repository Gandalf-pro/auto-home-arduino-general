#ifndef ALED_EFFECTS_HPP
#define ALED_EFFECTS_HPP

#include <FastLED.h>

namespace home {

class ALedEffects {
   public:
    static void staticColor(CRGB* leds, int numLeds, CRGB color) {
        fill_solid(leds, numLeds, color);
    }

    static void fade(CRGB* leds, int numLeds, CRGB startColor, CRGB endColor, uint8_t step) {
        for (int i = 0; i < numLeds; ++i) {
            leds[i] = blend(startColor, endColor, step);
        }
    }

    static void breathe(CRGB* leds, int numLeds, CRGB color, uint8_t brightness) {
        uint8_t breath = sin8(millis() / 4);  // Breathing effect
        fill_solid(leds, numLeds, color.nscale8(breath));
    }

    static void colorWipe(CRGB* leds, int numLeds, CRGB color, int delayMs) {
        for (int i = 0; i < numLeds; ++i) {
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

    static void rainbow(CRGB* leds, int numLeds, uint8_t initialHue, uint8_t deltaHue) {
        fill_rainbow(leds, numLeds, initialHue, deltaHue);
    }

    static void theaterChaseRainbow(CRGB* leds, int numLeds, int delayMs) {
        for (int q = 0; q < 3; q++) {
            for (int i = 0; i < numLeds; i += 3) {
                leds[i + q] = CHSV((i + q) * 255 / numLeds, 255, 255);
            }
            FastLED.show();
            delay(delayMs);
            for (int i = 0; i < numLeds; i += 3) {
                leds[i + q] = CRGB::Black;
            }
        }
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

#endif  // ALED_EFFECTS_HPP