#ifndef PANELLIGHTCONTROLLER_H_
#define PANELLIGHTCONTROLLER_H_

#include <Arduino.h>
#include "FastLED.h" // FastLED library. Preferably the latest copy of FastLED 2.1.
#include <FastLED.h> // for short list

#define TOTAL_LEDS 24

class PanelLightController
{
public:
    PanelLightController(int pin);
    void setState(int panelID, CRGB color);
    void initialize(struct CRGB *data);
    void update(void);
    virtual ~PanelLightController();

private:
    int pin;
    CRGB *_leds;
    void drawLed(CRGB color);
};

#endif /* PANELLIGHTCONTROLLER_H_ */