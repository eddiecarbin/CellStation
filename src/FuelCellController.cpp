
#include <FuelCellController.h>

FuelCellController::FuelCellController(int cells, int panelPin)
{
    this->totalCells = cells;
    this->completePanelPin = panelPin;
}

void FuelCellController::initialize(struct CRGB *data)
{
    this->_leds = data;
}

FuelCellState FuelCellController::getState()
{
    return currentState;
}

void FuelCellController::update()
{
    if (currentState == FuelCellState::FULL)
    {
    }
}

FuelCellController::~FuelCellController()
{
}