#pragma once

#include "Arduino.h"
#include "rfid.h"
#include "bust.h"
#include "magnet.h"
#include "lights.h"
#include "sound.h"

class Logic {
public:
  Logic();
  Rfid rfid;
  Bust bust;
  Magnet magnet;
  Lights lights;
  Sound sound;
  
  bool debug;

  void setup();
  void handle();
  void status();
  
  void triggerFirst();
  void triggerSecond();

private:
};

