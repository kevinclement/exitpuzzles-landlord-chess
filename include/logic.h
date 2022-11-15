#pragma once

#include "Arduino.h"
#include "rfid.h"
#include "bust.h"
#include "magnet.h"
#include "lights.h"
#include "sound.h"
#include "cabinet.h"
#include "cabinetLed.h"

#define DELAY_FOR_SOLVES 500

enum SOLVING_STATE {NOT_SOLVED, SOLVING, SOLVED};

class Logic {
public:
  Logic();
  Rfid rfid;
  Bust bust;
  Magnet magnet;
  Lights lights;
  Sound sound;
  Cabinet cabinet;
  CabinetLed cabinetLed;
  
  bool debug;

  void setup();
  void handle();
  void status();
  
  void triggerFirst();
  void triggerSecond();

private:
  long bust_solved_time = 0;
  long rfid_solved_time = 0;
  SOLVING_STATE bustState = NOT_SOLVED;
  SOLVING_STATE rfidState = NOT_SOLVED;
  bool solved;
};

