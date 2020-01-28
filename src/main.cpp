#include <Arduino.h>
#include "../lib/SoundPlayer/SoundPlayer.h"
#include "../lib/PotMonitor/PotMonitor.h"
#include "Utils.cpp"
#include <JC_Button.h>
#include "PanelLightController.h"
#include "FuelCellController.h"
#include "FastLED.h" // FastLED library. Preferably the latest copy of FastLED 2.1.

#include <FastLED.h> // for short list
#define FRAME_PER_SECOND 1000 / 120
#define BRIGHTNESS 100

#define TUBE_LED_PIN0 2
#define TUBE_LED_PIN1 3
#define TUBE_LED_PIN2 4

#define PANEL_LED_PIN0 5
#define PANEL_LED_PIN1 6
#define PANEL_LED_PIN2 7

#define FUEL_SWITCH_PIN0 A1
#define FUEL_SWITCH_PIN1 A2
#define FUEL_SWITCH_PIN2 A3

#define NUM_PIPE_LEDS0 20
#define NUM_PIPE_LEDS1 30
#define NUM_PIPE_LEDS2 20

PotMonitor volumePot(A5, 12);
//SoundPlayer soundPlayer(18, 15);

//https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples
CRGB pipeLeds0[NUM_PIPE_LEDS0];
CRGB pipeLeds1[NUM_PIPE_LEDS1];
CRGB pipeLeds2[NUM_PIPE_LEDS2];

Button tubeSwitch0(FUEL_SWITCH_PIN0, 40, false);
Button tubeSwitch1(FUEL_SWITCH_PIN1, 40, false);
Button tubeSwitch2(FUEL_SWITCH_PIN2, 40, false);

FuelCellController FuelTube0(3, NUM_PIPE_LEDS0);
FuelCellController FuelTube1(2, NUM_PIPE_LEDS1);
FuelCellController FuelTube2(1, NUM_PIPE_LEDS2);

PanelLightController panelController0(PANEL_LED_PIN0);
PanelLightController panelController1(PANEL_LED_PIN1);
PanelLightController panelController2(PANEL_LED_PIN2);

void setup()
{
  Serial.begin(9600);
  delay(5000);

  SoundPlayer::instance()->initialize();
  FastLED.addLeds<WS2812B, TUBE_LED_PIN0, BRG>(pipeLeds0, NUM_PIPE_LEDS0);
  FastLED.addLeds<WS2812B, TUBE_LED_PIN1, BRG>(pipeLeds1, NUM_PIPE_LEDS1);
  FastLED.addLeds<WS2812B, TUBE_LED_PIN2, BRG>(pipeLeds2, NUM_PIPE_LEDS2);

  FastLED.setBrightness(BRIGHTNESS);

  tubeSwitch0.begin();
  tubeSwitch1.begin();
  tubeSwitch2.begin();

  // SoundPlayer::instance()->PlaySound(1);

  FuelTube0.initialize(pipeLeds0, &tubeSwitch0, &panelController0);
  FuelTube1.initialize(pipeLeds1, &tubeSwitch1, &panelController1);
  FuelTube2.initialize(pipeLeds2, &tubeSwitch2, &panelController2);
}

void loop()
{
  if (volumePot.hasUpdated())
  {
    long volume = Utils::map(volumePot.getValue(), 0, 1023, 0, 30);
    SoundPlayer::instance()->volume(volume);
  }

  SoundPlayer::instance()->update();
  FuelTube0.update();
  FuelTube1.update();
  FuelTube2.update();

  if (FuelTube0.getState() == FuelCellState::FULL &&
      FuelTube1.getState() == FuelCellState::FULL &&
      FuelTube2.getState() == FuelCellState::FULL)
  {

    Serial.println("all tubes are full");
  }

  FastLED.show();

  delay(FRAME_PER_SECOND);
}