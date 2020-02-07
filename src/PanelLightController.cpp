
#include "PanelLightController.h"

PanelLightController::PanelLightController(int idx, CRGB data)
{
    this->color = data;
    this->index = idx;

    this->startIdx = index * TOTAL_LEDS;
}

void PanelLightController::setState(PanelState state)
{
    if (this->state == state)
        return;

    this->state = state;
}

void PanelLightController::initialize(struct CRGB *data)
{
    this->_leds = data;
}

void PanelLightController::update()
{

    if (state == PanelState::ON)
    {
        for (int i = startIdx; i < TOTAL_LEDS * 3; ++i)
        {
            _leds[i] = color;
        }
    }
    else
    {
        EVERY_N_SECONDS(5)
        {
            for (int i = startIdx; i < TOTAL_LEDS * 3; ++i)
            {
                _leds[i] = CRGB::Red;
            }
        }
        for (int i = startIdx; i < TOTAL_LEDS; ++i)
        {
            _leds[i].fadeToBlackBy(3);
        }
    }
}

PanelLightController::~PanelLightController()
{
}