
#ifndef FUELCELLCONTROLLER_H_
#define FUELCELLCONTROLLER_H_
#include <Arduino.h>
#include "FastLED.h" // FastLED library. Preferably the latest copy of FastLED 2.1.
#include <FastLED.h> // for short list
#include <JC_Button.h>
#include "PanelLightController.h"
// #include "../lib/TweenDuino/TweenDuino.h"

enum FuelCellState
{
    NONE,
    EMPTY,
    FILLING,
    FULL
};

class FuelCellController
{
public:
    FuelCellController(int cells, int leds);
    FuelCellState getState(void);
    void initialize(struct CRGB *data, Button *lever, PanelLightController *p);
    void update(void);
    void drawColor(CRGB color);
    void sinlon(CRGB *leds, uint16_t numLeds, const struct CRGB &color);
    virtual ~FuelCellController();

private:
    int totalCells;
    int cellCount;
    FuelCellState currentState;
    CRGB *_leds;
    Button *lever;
    unsigned long pause;
    int totalLeds;
    int prevpos;
    int index;
    PanelLightController *panel;
    void plasma();
    void sawtooth();
    void fire();
    CRGBPalette16 currentPalette; // Palette definitions
    // CRGBPalette16 targetPalette;
};

#endif /* FUELCELLCONTROLLER_H_ */