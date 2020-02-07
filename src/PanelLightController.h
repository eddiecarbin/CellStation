#ifndef PANELLIGHTCONTROLLER_H_
#define PANELLIGHTCONTROLLER_H_

#include <Arduino.h>
#include "FastLED.h" // FastLED library. Preferably the latest copy of FastLED 2.1.
#include <FastLED.h> // for short list

#define TOTAL_LEDS 24

enum PanelState
{
    ON,
    OFF
};

class PanelLightController
{
public:
    PanelLightController(int idx, CRGB data);
    void setState(PanelState state);
    void initialize(struct CRGB *data);
    void update(void);
    virtual ~PanelLightController();

private:
    CRGB *_leds;
    CRGB color;
    int index;
    PanelState state;
    int startIdx;
    void drawLed(CRGB color);
};

#endif /* PANELLIGHTCONTROLLER_H_ */