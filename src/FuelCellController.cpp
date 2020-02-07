
#define qsubd(x, b) ((x > b) ? b : 0)     // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b) ((x > b) ? x - b : 0) // Analog Unsigned subtraction macro. if result <0, then => 0

#include <FuelCellController.h>
#include "../lib/SoundPlayer/SoundPlayer.h"
#define LAST_BLOCK_TIME 1500


FuelCellController::FuelCellController(int id, int cells, int leds)
{
    this->totalCells = cells;
    this->totalLeds = leds;
    this->id = id;
    currentState = FuelCellState::EMPTY;
}

void FuelCellController::initialize(struct CRGB *data, Button *lever, PanelLightController *p)
{
    this->_leds = data;
    this->lever = lever;
    this->panel = p;
    currentPalette = OceanColors_p;
    pause = 0;
    timebase = millis();

    panel->setState(PanelState::OFF);
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

        if (millis() > pause)
        { // FastLED based non-blocking delay to update/display the sequence.
            //sinlon(_leds, totalLeds, CRGB::Red); // Call our sequence.
            currentPalette = OceanColors_p;

            plasma();
            pause = millis() + 50;
        }

        if (lever->wasPressed())
        {
            Serial.println("Release the fuel cells");
            SoundPlayer::instance()->PlaySound(2);
            panel->setState(PanelState::OFF);
            FastLED[id].clearLedData();
            //FastLED.clear();
            currentState = FuelCellState::EMPTY;

            timebase = millis();
            cellCount = 0;
        }
    }
    else if (currentState == FuelCellState::FILLING)
    {
        if (millis() > pause)
        { // FastLED based non-blocking delay to update/display the sequence.
            //sinlon(_leds, totalLeds, CRGB::Red); // Call our sequence.
            //plasma();
            currentPalette = CloudColors_p;
            sawtooth();
            pause = millis() + 50;
        }

        if (lever->wasReleased())
        {
            SoundPlayer::instance()->PlaySound(1);

            cellCount += 1;
            Serial.println("lever was pressed filling: " + String(totalCells) + ", " + String(cellCount));
        }

        if (lever->releasedFor(LAST_BLOCK_TIME) && cellCount >= totalCells)
        {
            Serial.println("last block is on hold");
            //FastLED.clear();
            FastLED[id].clearLedData();

            currentState = FuelCellState::FULL;
            timebase = millis();
            //
            panel->setState(PanelState::ON);
        }
    }
    else if (currentState == FuelCellState::EMPTY)
    {
        if (millis() > pause)
        { // FastLED based non-blocking delay to update/display the sequence.

            sinlon(CRGB::Red); // Call our sequence.
            // test();
            pause = millis() + 50;
        }
        // panel->setState(id, CRGB::Red);

        if (lever->wasReleased())
        {
            cellCount += 1;
            Serial.println("lever was pressed state empty: " + String(totalCells) + ", " + String(cellCount));

            SoundPlayer::instance()->PlaySound(1);
            //FastLED.clear();
            FastLED[id].clearLedData();

            timebase = millis();

            currentState = FuelCellState::FILLING;
        }
    }
}

void FuelCellController::sinlon(const struct CRGB &color)
{
    //     // Updated sinelon (no visual gaps)
    //     // a colored dot sweeping
    //     // back and forth, with
    //     // fading trails
    fadeToBlackBy(_leds, totalLeds, 50);
    int pos = beatsin16(13, 0, totalLeds, timebase);
    if (pos < prevpos)
    {
        fill_solid(_leds + pos, (prevpos - pos) + 1, color);
    }
    else
    {
        fill_solid(_leds + prevpos, (pos - prevpos) + 1, color);
    }
    prevpos = pos;
}

//https://github.com/atuline/FastLED-Demos/blob/master/plasma/plasma.ino

void FuelCellController::plasma()
{ // This is the heart of this program. Sure is short. . . and fast.

    int thisPhase = beatsin8(6, -64, 64); // Setting phase change for a couple of waves.
    int thatPhase = beatsin8(7, -64, 64);

    for (int k = 0; k < totalLeds; k++)
    { // For each of the LED's in the strand, set a brightness based on a wave as follows:

        int colorIndex = cubicwave8((k * 23) + thisPhase) / 2 + cos8((k * 15) + thatPhase) / 2; // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
        int thisBright = qsuba(colorIndex, beatsin8(7, 0, 96, timebase));                       // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..

        _leds[k] = ColorFromPalette(currentPalette, colorIndex, thisBright, LINEARBLEND); // Let's now add the foreground colour.
    }
}

void FuelCellController::test()
{
}

//https://github.com/atuline/FastLED-Demos/tree/master/sawtooth

void FuelCellController::sawtooth()
{

    int bpm = 20;
    int ms_per_beat = 60000 / bpm; // 500ms per beat, where 60,000 = 60 seconds * 1000 ms
    int ms_per_led = 60000 / bpm / totalLeds;

    int cur_led = ((millis() % ms_per_beat) / ms_per_led) % (totalLeds); // Using millis to count up the strand, with %NUM_LEDS at the end as a safety factor.

    if (cur_led == 0)
        fill_solid(_leds, totalLeds, CRGB::Black);
    else
        _leds[cur_led] = ColorFromPalette(currentPalette, 0, 255, LINEARBLEND); // I prefer to use palettes instead of CHSV or CRGB assignments.

} // sawtooth()

//https://github.com/atuline/FastLED-Demos/blob/master/inoise8_fire/inoise8_fire.ino

FuelCellController::~FuelCellController()
{
}