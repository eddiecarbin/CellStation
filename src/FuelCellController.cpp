
#include <FuelCellController.h>
#include "../lib/SoundPlayer/SoundPlayer.h"

FuelCellController::FuelCellController(int cells, int leds)
{
    this->totalCells = cells;
    this->totalLeds = leds;
    currentState = FuelCellState::EMPTY;
}

void FuelCellController::initialize(struct CRGB *data, Button *lever, PanelLightController *p)
{
    this->_leds = data;
    this->lever = lever;
    this->panel = p;
}

void FuelCellController::drawColor(CRGB color)
{
    for (int i = 0; i < totalLeds; i++)
    {
        _leds[i] = color;
    }
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
        drawColor(CRGB::Green);

        if (lever->wasPressed())
        {
            Serial.println("Release the fuel cells");
            SoundPlayer::instance()->PlaySound(2);
            panel->setState(PanelStateEnum::OFF);
            currentState = FuelCellState::EMPTY;
            cellCount = 0;
        }
    }
    else if (currentState == FuelCellState::FILLING)
    {
        drawColor(CRGB::White);

        if (lever->wasReleased())
        {
            SoundPlayer::instance()->PlaySound(1);

            cellCount += 1;
            Serial.println("lever was pressed filling: " + String(totalCells) + ", " + String(cellCount));
        }

        if (lever->releasedFor(2000) && cellCount >= totalCells)
        {
            Serial.println("last block is on hold");
            currentState = FuelCellState::FULL;
            panel->setState(PanelStateEnum::ON);
        }
    }
    else if (currentState == FuelCellState::EMPTY)
    {
        drawColor(CRGB::Red);
        if (lever->wasReleased())
        {
            cellCount += 1;
            Serial.println("lever was pressed state empty: " + String(totalCells) + ", " + String(cellCount));

            SoundPlayer::instance()->PlaySound(1);
            currentState = FuelCellState::FILLING;
        }
    }
}

FuelCellController::~FuelCellController()
{
}