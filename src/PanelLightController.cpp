
#include "PanelLightController.h"

PanelLightController::PanelLightController(int pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
}


void PanelLightController::setState(int panelID,  CRGB color)
{
for (int i = 0; i < TOTAL_LEDS * 3; ++i)
    {
        _leds[i] = CRGB::Red;
    }
    for (int i = 0; i < TOTAL_LEDS; ++i)
    {
        //_leds[(panelID * TOTAL_LEDS) + i] = color;
    }
}

void PanelLightController::initialize(struct CRGB *data)
{
    this->_leds = data;

    for (int i = 0; i < TOTAL_LEDS * 3; ++i)
    {
        _leds[i] = CRGB::Red;
    }
}

void PanelLightController::update()
{
}

PanelLightController::~PanelLightController()
{
}