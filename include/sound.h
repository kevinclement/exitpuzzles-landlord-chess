#pragma once

#include "Arduino.h"

class Logic;

class Sound {
  public:
    Sound(Logic &logic);
    void setup();
    void handle();

    void bustTriggered();
    void solved();

  private:
    Logic &_logic;
};