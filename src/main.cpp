#include <Arduino.h>
#include "../lib/SoundPlayer/SoundPlayer.h"
#include "../lib/PotMonitor/PotMonitor.h"
#include <JC_Button.h>

#include "FuelCellController.h"
#include "FastLED.h" // FastLED library. Preferably the latest copy of FastLED 2.1.

#include <FastLED.h> // for short list

#define FRAME_PER_SECOND 120
#define BRIGHTNESS 240

#define PIPE_DATA_PIN0 2
#define PIPE_DATA_PIN1 3
#define PIPE_DATA_PIN2 4

#define PANEL_LED_PIN0 5
#define PANEL_LED_PIN1 6
#define PANEL_LED_PIN2 7

#define FUEL_SWITCH_PIN0 A0
#define FUEL_SWITCH_PIN1 A1
#define FUEL_SWITCH_PIN2 A2

#define NUM_PIPE_LEDS0 10
#define NUM_PIPE_LEDS1 10
#define NUM_PIPE_LEDS2 10

PotMonitor volumePot(A5, 12);
SoundPlayer soundPlayer(18, 15);

//https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples
CRGB pipeLeds0[NUM_PIPE_LEDS0];
CRGB pipeLeds1[NUM_PIPE_LEDS1];
CRGB pipeLeds2[NUM_PIPE_LEDS2];

Button tubeSwitch0(FUEL_SWITCH_PIN0);
Button tubeSwitch1(FUEL_SWITCH_PIN1);
Button tubeSwitch2(FUEL_SWITCH_PIN2);

FuelCellController FuelTube0(3, PANEL_LED_PIN0);
FuelCellController FuelTube1(2, PANEL_LED_PIN1);
FuelCellController FuelTube2(1, PANEL_LED_PIN2);

long map2(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
}

void setup()
{

  FastLED.addLeds<UCS1903, PIPE_DATA_PIN0, BRG>(pipeLeds0, NUM_PIPE_LEDS0);
  FastLED.addLeds<UCS1903, PIPE_DATA_PIN1, BRG>(pipeLeds1, NUM_PIPE_LEDS1);
  FastLED.addLeds<UCS1903, PIPE_DATA_PIN2, BRG>(pipeLeds2, NUM_PIPE_LEDS2);
  FastLED.setBrightness(BRIGHTNESS);

  tubeSwitch0.begin();
  tubeSwitch1.begin();
  tubeSwitch2.begin();

  FuelTube0.initialize(pipeLeds0);
  FuelTube1.initialize(pipeLeds1);
  FuelTube2.initialize(pipeLeds2);


  soundPlayer.initialize();
}

void loop()
{

  if (volumePot.hasUpdated())
  {
    long volume = map2(volumePot.getValue(), 0, 1023, 0, 30);
    //Serial.println(volume);
    soundPlayer.volume(volume);
  }

  FuelTube0.update();
  FuelTube1.update();
  FuelTube2.update();
  // put your main code here, to run repeatedly:

  if (FuelTube0.getState() == FuelCellState::FULL &&
      FuelTube1.getState() == FuelCellState::FULL &&
      FuelTube2.getState() == FuelCellState::FULL){

    Serial.println("all tubes are full");
  }

  FastLED.show();
  delay(1000 / FRAME_PER_SECOND);
}