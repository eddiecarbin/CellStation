
#ifndef FUELCELLCONTROLLER_H_
#define FUELCELLCONTROLLER_H_
#include <Arduino.h>
#include "FastLED.h" // FastLED library. Preferably the latest copy of FastLED 2.1.
#include <FastLED.h> // for short list
#include <JC_Button.h>

enum FuelCellState
{
    NONE,
    FULL,
    EMPTY,
    FILLING
};

class FuelCellController
{
public:
    FuelCellController(int cells, int panelPin);
    FuelCellState getState(void);
    void initialize(struct CRGB *data, Button* lever);
    void update(void);
    virtual ~FuelCellController();

private:
    int totalCells;
    int cellCount;
    FuelCellState currentState;
    CRGB *_leds;
    Button* lever;
    int completePanelPin;
};

#endif /* FUELCELLCONTROLLER_H_ */