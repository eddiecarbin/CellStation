#include <Arduino.h>
#include "FuelCellController.h"


FuelCellController FuelTube0();
FuelCellController FuelTube1();
FuelCellController FuelTube2();


void setup() {
  FuelTube0.initialize();
  FuelTube1.initialize();
  FuelTube2.initialize();
}

void loop() {

  FuelTube0.update();
  FuelTube1.update();
  FuelTube2.update();
  // put your main code here, to run repeatedly:
}