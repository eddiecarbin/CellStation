
#include "PanelLightController.h"

PanelLightController::PanelLightController(int pin)
{

    this->pin = pin;
    pinMode(pin, OUTPUT);
}


PanelStateEnum PanelLightController::getState()
{
    return currentState;
}

void PanelLightController::setState(PanelStateEnum data)
{
    if (this->currentState == data)
        return;

    currentState = data;

    digitalWrite(pin, currentState);
}

void PanelLightController::initialize()
{
}

void PanelLightController::update()
{
}

PanelLightController::~PanelLightController()
{
}