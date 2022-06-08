#pragma once

#include "Arduino.h"
#include <PN532.h>
#include <PN532_HSU.h>

class Logic;

#define NR_OF_READERS   2
enum RFID_STATE {INCORRECT, CORRECT, MISSING, UNKNOWN};

class Rfid {
  public:
    Rfid(Logic &logic);
    void setup();
    void handle();
    RFID_STATE state[NR_OF_READERS] = { UNKNOWN, UNKNOWN };

  private:
    Logic &_logic;
    bool _isInit = false;
    bool compareTags(uint8_t index);

    PN532 nfc1;
    PN532 nfc2;

    PN532_HSU pn532hsu1;
    PN532_HSU pn532hsu2;
    
    PN532 readers[NR_OF_READERS] = { nfc1, nfc2 };

    RFID_STATE checkForTagHSU(uint8_t index, PN532 nfc);
    String prettyState(uint8_t);
    bool initReader(PN532 nfc, const char* label);

    bool tag_present_prev[NR_OF_READERS] = { false, false };
    bool tag_present[NR_OF_READERS] = { false, false };
    int error_counter[NR_OF_READERS] = { 0, 0 };
    bool tag_found[NR_OF_READERS] = { false, false };
    byte readCards[NR_OF_READERS][4];
};