#pragma once

#include "Arduino.h"

class Logic;

#define BTN_SWITCH_PIN  14
#define ON              LOW
#define OFF             HIGH

class Bust {
  public:
    bool isSwitched = false;
    
    Bust(Logic &logic);
    void setup();
    void handle();

  private:
    Logic &_logic;
};