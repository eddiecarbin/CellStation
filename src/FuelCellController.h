
#ifndef FUELCELLCONTROLLER_H_
#define FUELCELLCONTROLLER_H_
#include <Arduino.h>
#include "FastLED.h" // FastLED library. Preferably the latest copy of FastLED 2.1.
#include <FastLED.h> // for short list


enum FuelCellState{
    FULL,
    EMPTY
};

class FuelCellController{
    public:
        FuelCellController(int cells);
        FuelCellState getState(void);
        void initialize(void);
        void update(void);
        virtual ~FuelCellController();
    private:
        int totalCells;
        int cellCount;
        int pipeArrayPin;
        int completePanelPin;
}; 


#endif /* FUELCELLCONTROLLER_H_ */