
#include "PanelLightController.h"


#define TOTAL_PANELS 3

PanelLightController::PanelLightController(int pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

void PanelLightController::setState(int panelID, CRGB color)
{
    /* for (int i = 0; i < TOTAL_LEDS * 3; ++i)
    {
        //_leds[i] = CRGB::Red;
    } */
    for (int i = 0; i < TOTAL_LEDS; ++i)
    {
        _leds[(panelID * TOTAL_LEDS) + i] = color;
    }

}

void PanelLightController::initialize(struct CRGB *data)
{
    this->_leds = data;

    for (int i = 0; i < TOTAL_LEDS * 3; ++i)
    {
        //_leds[i] = CRGB::Yellow;
    }
}

void PanelLightController::update()
{
    /* for (int i = 0; i < TOTAL_LEDS * 3; ++i)
    {
        _leds[i] = CRGB::Yellow;
    }
 */
    FastLED[3].showLeds(10);
}

PanelLightController::~PanelLightController()
{
}