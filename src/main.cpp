#include <Arduino.h>
#include "../lib/SoundPlayer/SoundPlayer.h"
#include "../lib/PotMonitor/PotMonitor.h"
#include "../lib/FSM/FSM.h"

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

#define FUEL_SWITCH_PIN0 A1
#define FUEL_SWITCH_PIN1 A2
#define FUEL_SWITCH_PIN2 A3

#define NUM_PIPE_LEDS0 40
#define NUM_PIPE_LEDS1 20
#define NUM_PIPE_LEDS2 10

#define GO_TO_GAME_LOOP 1
#define GO_TO_START_GAME 2

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

State StateDoNothing(NULL, NULL, NULL);
Fsm fsm(&StateDoNothing);

//Setup game board
void OnStartGameEnter()
{
}

void OnStateStartGameUpdate()
{

  tubeSwitch0.read();
  tubeSwitch1.read();
  tubeSwitch2.read();

  FuelTube0.update();
  FuelTube1.update();
  FuelTube2.update();
/* 
  if (tubeSwitch0.wasReleased())
  {
    Serial.println("red is pressed");
  }
  if (tubeSwitch1.wasReleased())
  {
    Serial.println("green is pressed");
  }
  if (tubeSwitch2.wasReleased())
  {
    Serial.println("yellow is pressed");
  } */

  if (FuelTube0.getState() == FuelCellState::FULL &&
      FuelTube1.getState() == FuelCellState::FULL &&
      FuelTube2.getState() == FuelCellState::FULL)
  {

    //Serial.println("all tubes are full");
  }
}

void OnStateGameLoopEnter()
{
}

void OnStateGameLoopUpdate()
{
}

State StateStartGame(&OnStartGameEnter, &OnStateStartGameUpdate, NULL);
State StateGameLoop(&OnStateGameLoopEnter, &OnStateGameLoopUpdate, NULL);

long map2(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
}

void setup()
{
  Serial.begin(9600);
  delay(5000);

  fsm.add_transition(&StateStartGame, &StateGameLoop,
                     GO_TO_GAME_LOOP, NULL);

  fsm.add_transition(&StateGameLoop, &StateStartGame,
                     GO_TO_START_GAME, NULL);

  FastLED.addLeds<UCS1903, PIPE_DATA_PIN0, BRG>(pipeLeds0, NUM_PIPE_LEDS0);
  FastLED.addLeds<UCS1903, PIPE_DATA_PIN1, BRG>(pipeLeds1, NUM_PIPE_LEDS1);
  FastLED.addLeds<UCS1903, PIPE_DATA_PIN2, BRG>(pipeLeds2, NUM_PIPE_LEDS2);
  FastLED.setBrightness(BRIGHTNESS);

  // pinMode(FUEL_SWITCH_PIN0, INPUT);
  // pinMode(FUEL_SWITCH_PIN1, INPUT);
  // pinMode(FUEL_SWITCH_PIN2, INPUT);

  tubeSwitch0.begin();
  tubeSwitch1.begin();
  tubeSwitch2.begin();

  FuelTube0.initialize(pipeLeds0, &tubeSwitch0);
  FuelTube1.initialize(pipeLeds1, &tubeSwitch1);
  FuelTube2.initialize(pipeLeds2, &tubeSwitch2);

  soundPlayer.initialize();

  fsm.goToState(&StateStartGame);
}

void loop()
{
  if (volumePot.hasUpdated())
  {
    long volume = map2(volumePot.getValue(), 0, 1023, 0, 30);
    //Serial.println(volume);
    soundPlayer.volume(volume);
  }
  fsm.run_machine();

  FastLED.show();
  delay(1000 / FRAME_PER_SECOND);
}