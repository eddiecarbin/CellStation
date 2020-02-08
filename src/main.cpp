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
#define BRIGHTNESS 180

#define TUBE_LED_PIN0 2
#define TUBE_LED_PIN1 3
#define TUBE_LED_PIN2 4

#define PANEL_LED_PIN 5

#define FUEL_SWITCH_PIN0 A1
#define FUEL_SWITCH_PIN1 A2
#define FUEL_SWITCH_PIN2 A3

#define NUM_PIPE_LEDS0 11
#define NUM_PIPE_LEDS1 43
#define NUM_PIPE_LEDS2 25

#define NUM_PANEL_LED 72
#define NUM_STRIPS 4

PotMonitor volumePot(A5, 12);
//SoundPlayer soundPlayer(18, 15);

//https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples
CRGB pipeLeds0[NUM_PIPE_LEDS0];
CRGB pipeLeds1[NUM_PIPE_LEDS1];
CRGB pipeLeds2[NUM_PIPE_LEDS2];

CRGB panelLED[NUM_PANEL_LED];

//CLEDController *controllers[NUM_STRIPS];

Button tubeSwitch0(FUEL_SWITCH_PIN0, 40, false);
Button tubeSwitch1(FUEL_SWITCH_PIN1, 40, false);
Button tubeSwitch2(FUEL_SWITCH_PIN2, 40, false);

FuelCellController FuelTube0(0, 3, NUM_PIPE_LEDS0);
FuelCellController FuelTube1(2, 2, NUM_PIPE_LEDS1);
FuelCellController FuelTube2(1, 1, NUM_PIPE_LEDS2);

PanelLightController panelController0(0, CRGB::Blue);
PanelLightController panelController1(1, CRGB::Yellow);
PanelLightController panelController2(2, CRGB::Green);

//https://github.com/FastLED/FastLED/wiki/Pixel-reference

void setup()
{
  Serial.begin(9600);
  delay(5000);

  pinMode(PANEL_LED_PIN, OUTPUT);

  SoundPlayer::instance()->initialize();
  FastLED.addLeds<WS2812B, TUBE_LED_PIN0, GRB>(pipeLeds0, NUM_PIPE_LEDS0);
  FastLED.addLeds<WS2812B, TUBE_LED_PIN1, GRB>(pipeLeds1, NUM_PIPE_LEDS1);
  FastLED.addLeds<WS2812B, TUBE_LED_PIN2, GRB>(pipeLeds2, NUM_PIPE_LEDS2);

  FastLED.addLeds<WS2812B, PANEL_LED_PIN, GRB>(panelLED, NUM_PANEL_LED);

  FastLED.setBrightness(BRIGHTNESS);

  tubeSwitch0.begin();
  tubeSwitch1.begin();
  tubeSwitch2.begin();

  panelController0.initialize(panelLED);
  panelController1.initialize(panelLED);
  panelController2.initialize(panelLED);

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

  panelController0.update();
  panelController1.update();
  panelController2.update();

  panelController0.setState(PanelState::ON);
  panelController1.setState(PanelState::ON);
  panelController2.setState(PanelState::ON);
  //FastLED.show();
  FastLED[0].showLeds(BRIGHTNESS);
  FastLED[1].showLeds(BRIGHTNESS);
  FastLED[2].showLeds(BRIGHTNESS);
  FastLED[3].showLeds(80);

  if (FuelTube0.getState() == FuelCellState::FULL &&
      FuelTube1.getState() == FuelCellState::FULL &&
      FuelTube2.getState() == FuelCellState::FULL)
  {
    Serial.println("all tubes are full");
  }

  delay(FRAME_PER_SECOND);
}