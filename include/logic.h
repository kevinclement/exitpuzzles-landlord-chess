#pragma once

#include "Arduino.h"
#include "rfid.h"
#include "bust.h"
#include "magnet.h"
#include "lights.h"
#include "sound.h"
#include "cabinet.h"
#include "cabinetLed.h"
#include "speakerLed.h"

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
  SpeakerLed speakerLed;

  SOLVING_STATE bustState = NOT_SOLVED;
  SOLVING_STATE rfidState = NOT_SOLVED;
  
  bool debug;

  void setup();
  void handle();
  void status();
  void togglePiecesDisabled(bool);
  
  void triggerFirst();
  void triggerFirstStageTwo();
  void triggerSecond();
  void triggerSecondStageTwo();
  void triggerFinal();
  void finish();

private:
  unsigned long bust_solved_time = 0;
  unsigned long rfid_solved_time = 0;
  unsigned long final_solved_time = 0;
  bool piecesDisabled = false;
  bool solved;
};

