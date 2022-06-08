#include "Arduino.h"
#include "rfid.h"
#include "logic.h"

#define OFFSET  1

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

Rfid::Rfid(Logic &logic)
: _logic(logic),
  pn532hsu1(Serial1),
  pn532hsu2(Serial2),
  nfc1(pn532hsu1),
  nfc2(pn532hsu2)
{
}

void Rfid::setup() {
  // Remap serial uart
  Serial1.begin(115200, SERIAL_8N1, A0, A1);

  _isInit  = initReader(readers[0], "NFC1");
  _isInit &= initReader(readers[1], "NFC2");

  if (_isInit) {
    Serial.println("\nReady to Scan...");
  } else {
    Serial.println("\nWARN: RFID readers not initialized.  Currently disabled!");
  }
}

void Rfid::handle() {
  if (!_isInit) {
    return;
  }

  for (uint8_t i = 0; i < 1; i++) {

    RFID_STATE st = UNKNOWN;
    st = checkForTagHSU(i, readers[i]);
    
    if (st != state[i]) {
      Serial.print("state changed for ");
      Serial.print(i + OFFSET);
      Serial.print(" ");
      Serial.print(prettyState(state[i]));
      Serial.print(" => ");
      Serial.println(prettyState(st));

      state[i] = st;
      _logic.status();
    }
  }
}

bool Rfid::initReader(PN532 nfc, const char* label) {
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("ERROR: ");
    Serial.print(label);
    Serial.println(": Didn't find PN53x board");
    return false;
  }
  
  Serial.print(label);
  Serial.print(": Firmware Version (PN5"); Serial.print((versiondata>>24) & 0xFF, HEX); 
  Serial.print("): "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);

  // configure board to read RFID tags
  nfc.SAMConfig();

  // mark card as init'd
  return true;
}

RFID_STATE Rfid::checkForTagHSU(uint8_t index, PN532 nfc) {
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)  
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength)) {
    for ( uint8_t i = 0; i < 4; i++) {
      readCards[index][i] = uid[i];
    }
    return compareTags(index) ? CORRECT : INCORRECT;
  } else {
    return MISSING;
  }
}

bool Rfid::compareTags(uint8_t index) {
  for ( uint8_t i = 0; i < 2; i++ ) {
    bool cardMatch = true;
    for ( uint8_t j = 0; j < 4; j++ ) {
        cardMatch = cardMatch && (readCards[index][j] == tags[index][i][j]);
    }

    if (cardMatch) {
      return true;
    }
  }

  // unknown tag, if debugging write to serial to help add it to system
  if (_logic.debug) {
    Serial.printf("WARN: Unknown tag: { 0x%02X, 0x%02X, 0x%02X, 0x%02X }\n", readCards[index][0], readCards[index][1], readCards[index][2], readCards[index][3]);
  }

  return false;
}

String Rfid::prettyState(uint8_t state) {
  return 
    state == INCORRECT ? "Incorrect" : 
    state == CORRECT ? "Correct" : 
    state == MISSING ? "Missing" : 
    "Unknown";
}