#pragma once

#include "Arduino.h"
#include <Adafruit_PN532.h>

class Logic;

#define DELAY_BETWEEN_CARDS 500

#define PN532_SCK   SCK
#define PN532_MOSI  MOSI
#define PN532_MISO  MISO

enum RFID_STATE {INCORRECT, CORRECT, MISSING, UNKNOWN};

class PN532Device {
  public:
    RFID_STATE state = UNKNOWN;
    
    PN532Device(Logic &logic, uint8_t irq_PIN, uint8_t ss_PIN, const char* label);
    void setup();
    void handle();

  private:
    Logic &_logic;
    Adafruit_PN532 _nfc;

    uint8_t _IRQ_PIN;
    const char* _label;
    bool disabled = false;
    bool isInit = false;
    
    int irq_prev = 0;
    int irq_curr = 0;
    long last_read_time = 0;

    byte readCards[4];

    void startListening();
    RFID_STATE cardDetected();
    bool compareTags();
    void checkForStateChange(RFID_STATE st);
    String prettyState(uint8_t state);
};