#include <Arduino.h>
#include "FastLED.h" // FastLED library. Preferably the latest copy of FastLED 2.1.

#include <FastLED.h> // for short list

class LEDAnimations
{

public:
    static void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){};

    static void TheaterChaseRainbow(int SpeedDelay){};

    static void setPixel(int Pixel, byte red, byte green, byte blue)
    {
        // leds[Pixel].r = red;
        // leds[Pixel].g = green;
        // leds[Pixel].b = blue;
    }

    static void pause(unsigned long period)
    {
        unsigned long time_now = millis();
        while (millis() < time_now + period)
        {
            //wait approx. [period] ms
        }
    }

    static void setAll(byte red, byte green, byte blue)
    {
        // for (int i = 0; i < NUM_LEDS; i++)
        // {
        //     setPixel(i, red, green, blue);
        // }
        // showStrip();
    }
};