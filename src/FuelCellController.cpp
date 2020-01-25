
#include <FuelCellController.h>

FuelCellController::FuelCellController(int cells, int panelPin)
{
    this->totalCells = cells;
    this->completePanelPin = panelPin;
}

void FuelCellController::initialize(struct CRGB *data, Button *lever)
{
    this->_leds = data;
    this->lever = lever;
}

FuelCellState FuelCellController::getState()
{
    return currentState;
}

void FuelCellController::update()
{
    lever->read();

    if (currentState == FuelCellState::FULL)
    {
        if (lever->wasPressed())
        {
            Serial.println("Release the fuel cells");
            currentState = FuelCellState::EMPTY;
        }
    }
}

FuelCellController::~FuelCellController()
{
}