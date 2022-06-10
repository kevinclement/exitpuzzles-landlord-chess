#include "Arduino.h"
#include "pn532Device.h"
#include "logic.h"

// TODO: move into rfid, and pass single array to constructor
#define TODO_FIX_INDEX 0
byte tags [2][2][4] = {
  {
    { 0xF7, 0x6F, 0x8C, 0xF2 },
    { 0x87, 0x93, 0x8E, 0xF2 }
  },
  {
    { 0x27, 0x8F, 0x8E, 0xF2 },
    { 0xF7, 0x92, 0x8E, 0xF2 }
  }
};

PN532Device::PN532Device(Logic &logic, uint8_t irq_PIN, uint8_t ss_PIN, const char* label)
: _logic(logic),
  _nfc(PN532_SCK, PN532_MISO, PN532_MOSI, ss_PIN), 
  _label(label),
  _IRQ_PIN(irq_PIN)
{
}

void PN532Device::setup() {
  pinMode(_IRQ_PIN, INPUT);

  _nfc.begin();

  uint32_t versiondata = _nfc.getFirmwareVersion();
  if (versiondata) {
    Serial.print(_label);
    Serial.print(": Firmware Version (PN5"); Serial.print((versiondata>>24) & 0xFF, HEX); 
    Serial.print("): "); Serial.print((versiondata>>16) & 0xFF, DEC); 
    Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

    // Set the max number of retry attempts to read from a card
    // This prevents us from waiting forever for a card, which is
    // the default behaviour of the PN532.
    _nfc.setPassiveActivationRetries(0xFF);

    // configure board to read RFID tags
    _nfc.SAMConfig();
    isInit = true;
  } else {
    Serial.print("ERROR: ");
    Serial.print(_label);
    Serial.println(": Didn't find PN53x board");
    isInit = false;
  }

  startListening();
}

void PN532Device::handle() {
  // If we didn't initialize, NOOP
  if (!isInit) {
    return;
  }

  // If we've done a read, and its been a set period of time, mark our state missing
  if (last_read_time > 0 && state != MISSING && millis() - last_read_time > 1000) {
    checkForStateChange(MISSING);
  }

  if (disabled) {
    if (millis() - last_read_time > DELAY_BETWEEN_CARDS) {
      disabled = false;
      startListening();
    }
  } else {
      irq_curr = digitalRead(_IRQ_PIN);

      // When the IRQ is pulled low - the reader has got something for us.
      if (irq_curr == LOW && irq_prev == HIGH) {
        RFID_STATE st = cardDetected();
        checkForStateChange(st);
      }

      irq_prev = irq_curr;
  }
}

void PN532Device::startListening() {
  // Reset our IRQ indicators
  irq_prev = irq_curr =  HIGH;
  _nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
}

RFID_STATE PN532Device::cardDetected() {
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  RFID_STATE st = MISSING;
  
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)  
  if (_nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength)) {
    for (uint8_t i = 0; i < 4; i++) {
      readCards[i] = uid[i];
    }
    
    last_read_time = millis();

    st = compareTags() ? CORRECT : INCORRECT;
  }

  // The reader will be enabled again after DELAY_BETWEEN_CARDS ms will pass.
  disabled = true;

  return st;
}

// TODO: pass readCards
bool PN532Device::compareTags() {
  for ( uint8_t i = 0; i < 2; i++ ) {
    bool cardMatch = true;
    for ( uint8_t j = 0; j < 4; j++ ) {
        // TODO: DONT HARDCODE
        cardMatch = cardMatch && (readCards[j] == tags[TODO_FIX_INDEX][i][j]);
    }

    if (cardMatch) {
      return true;
    }
  }

  // unknown tag, if debugging write to serial to help add it to system
  if (_logic.debug) {
    Serial.printf("WARN: Unknown tag: { 0x%02X, 0x%02X, 0x%02X, 0x%02X }\n", readCards[0], readCards[1], readCards[2], readCards[3]);
  }

  return false;
}

void PN532Device::checkForStateChange(RFID_STATE st) {
  if (st != state) {
    Serial.print("state changed for ");
    Serial.print(_label);
    Serial.print(" ");
    Serial.print(prettyState(state));
    Serial.print(" => ");
    Serial.println(prettyState(st));

    state = st;
    _logic.status();
  }
}

String PN532Device::prettyState(uint8_t state) {
  return 
    state == INCORRECT ? "Incorrect" : 
    state == CORRECT ? "Correct" : 
    state == MISSING ? "Missing" : 
    "Unknown";
}
