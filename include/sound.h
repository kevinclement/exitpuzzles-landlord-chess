#pragma once

#include "Arduino.h"

class Logic;

enum SOUND_STATE {STOPPED, PLAYING};

class Sound {
  public:
    SOUND_STATE state = STOPPED;

    Sound(Logic &logic);
    void setup();
    void handle();

    void beep(unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds);
    void bustTriggered();
    void solved();

  private:
    Logic &_logic;
};