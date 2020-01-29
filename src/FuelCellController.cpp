
#define qsubd(x, b) ((x > b) ? b : 0)     // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b) ((x > b) ? x - b : 0) // Analog Unsigned subtraction macro. if result <0, then => 0

#include <FuelCellController.h>
#include "../lib/SoundPlayer/SoundPlayer.h"
#include "LEDAnimations.cpp"
#define LAST_BLOCK_TIME 1500

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
    currentPalette = OceanColors_p;
    pause = 0;
    // fillingAnimation = FillingAnimation(1);

    // fillingAnimation->update();
    //  empty animation
    //  filling animation
    //  complete animation
    // emptyAnimation = new EmptyAnimation();
    // completeAnimation = new CompleteAnimation(1);
    // currentAnimation
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
            panel->setState(PanelStateEnum::OFF);
            currentState = FuelCellState::EMPTY;
            cellCount = 0;
        }
    }
    else if (currentState == FuelCellState::FILLING)
    {
        // drawColor(CRGB::White);
        if (millis() > pause)
        { // FastLED based non-blocking delay to update/display the sequence.
            //sinlon(_leds, totalLeds, CRGB::Red); // Call our sequence.
            //plasma();
            sawtooth();
            currentPalette = PartyColors_p;
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
            currentState = FuelCellState::FULL;
            panel->setState(PanelStateEnum::ON);
        }
    }
    else if (currentState == FuelCellState::EMPTY)
    {

        //drawColor(CRGB::Red);

        if (millis() > pause)
        { // FastLED based non-blocking delay to update/display the sequence.

            //currentPalette = LavaColors_p;

            sinlon(_leds, totalLeds, CRGB::Red); // Call our sequence.
            //plasma();
            /* currentPalette = CRGBPalette16(
                CRGB::Black, CRGB::Black, CRGB::Black, CHSV(0, 255, 4),
                CHSV(0, 255, 8), CRGB::Red, CRGB::Red, CRGB::Red,
                CRGB::DarkOrange, CRGB::Orange, CRGB::Orange, CRGB::Orange,
                CRGB::Yellow, CRGB::Yellow, CRGB::Gray, CRGB::Gray);
            fire(); */
            // currentPalette = PartyColors_p;
            pause = millis() + 50;
        }

        if (lever->wasReleased())
        {
            cellCount += 1;
            Serial.println("lever was pressed state empty: " + String(totalCells) + ", " + String(cellCount));

            SoundPlayer::instance()->PlaySound(1);
            currentState = FuelCellState::FILLING;
        }
    }
}
/* 
void FuelCellController::sinlon(CRGB *leds, uint16_t numLeds, const struct CRGB &color)
{
    uint8_t thisbeat = 23; // Beats per minute for first part of dot.
    uint8_t thatbeat = 28; // Combined the above with this one.
    uint8_t thisfade = 32; // How quickly does it fade? Lower = slower fade rate.
    // uint8_t thissat = 255; // The saturation, where 255 = brilliant colours.
    uint8_t thisbri = 255; // Brightness of a sequence.
    int myhue = 0;
    fadeToBlackBy(leds, totalLeds, thisfade);
    int pos1 = beatsin16(thisbeat, 0, totalLeds);
    int pos2 = beatsin16(thatbeat, 0, totalLeds);

    leds[(pos1 + pos2) / 2] += ColorFromPalette(currentPalette, myhue++, thisbri, LINEARBLEND);
} */
void FuelCellController::sinlon(CRGB *leds, uint16_t numLeds, const struct CRGB &color)
{
    //     // Updated sinelon (no visual gaps)
    //     // a colored dot sweeping
    //     // back and forth, with
    //     // fading trails
    fadeToBlackBy(leds, numLeds, 100);
    int pos = beatsin16(13, 0, numLeds);
    prevpos = 0;
    if (pos < prevpos)
    {
        fill_solid(leds + pos, (prevpos - pos) + 1, color);
    }
    else
    {
        fill_solid(leds + prevpos, (pos - prevpos) + 1, color);
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
        int thisBright = qsuba(colorIndex, beatsin8(7, 0, 96));                                 // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..

        _leds[k] = ColorFromPalette(currentPalette, colorIndex, thisBright, LINEARBLEND); // Let's now add the foreground colour.
    }
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
void FuelCellController::fire()
{

    uint32_t xscale = 20; // How far apart they are
    uint32_t yscale = 10;
    for (int i = 0; i < totalLeds; i++)
    {
        index = inoise8(i * xscale, millis() * yscale * totalLeds / 255);                                          // X location is constant, but we move along the Y at the rate of millis()
        _leds[i] = ColorFromPalette(currentPalette, min(i * (index) >> 6, 255), i * 255 / totalLeds, LINEARBLEND); // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    }                                                                                                              // The higher the value of i => the higher up the palette index (see palette definition).
                                                                                                                   // Also, the higher the value of i => the brighter the LED.
}

FuelCellController::~FuelCellController()
{
}