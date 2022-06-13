#pragma once

#include "Arduino.h"
#include "rfid.h"
#include "bust.h"
#include "magnet.h"
#include "lights.h"
#include "sound.h"

#define DELAY_FOR_SOLVES 500

class Logic {
public:
  Logic();
  Rfid rfid;
  Bust bust;
  Magnet magnet;
  Lights lights;
  Sound sound;
  
  bool debug;
  bool bustSolved;
  bool solved;

  void setup();
  void handle();
  void status();
  
  void triggerFirst();
  void triggerSecond();

private:
  long bust_solved_time = 0;
  long solved_time = 0;
};

