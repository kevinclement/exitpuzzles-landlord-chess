#include "Arduino.h"
#include "pn532Device.h"
#include "logic.h"

struct ChessPiece {
  Piece piece;
  Color color;
  uint8_t validLocations;
  byte rfidTag[4];
};

const char *colorStrings[] = { "WHITE", "BLACK" };
const char *pieceStrings[] = { "PAWN", "BISHOP", "KNIGHT", "ROOK", "QUEEN", "KING" };

#define NUM_PIECES 2
ChessPiece PIECES[NUM_PIECES] = {
    {QUEEN, BLACK, F2, { 0x04, 0x13, 0x21, 0x0A }},
    {KING,  WHITE, C7, { 0x04, 0x0E, 0x22, 0x0A }},
};

// {KING,   WHITE, F2,       { 0x04, 0x16, 0x22, 0x0A }},
// {QUEEN,  WHITE, F2,       { 0x04, 0xBF, 0x21, 0x0A }},
// {KING,   BLACK, C7,       { 0x04, 0xC7, 0x21, 0x0A }},
// {QUEEN,  BLACK, C7,       { 0x04, 0x97, 0x1F, 0x0A }},
// {KNIGHT, BLACK, F2,       { 0x04, 0x8F, 0x1F, 0x0A }},
// {ROOK,   WHITE, C7,       { 0x04, 0x2B, 0x21, 0x0A }},
// {PAWN,   WHITE, F2 | C7,  { 0x04, 0xF9, 0x21, 0x0A }}

// TMP: 
//   func def: 
//      byte (*validTags)[4]
//   private field: 
//      byte (*tags)[4];

PN532Device::PN532Device(Logic &logic, uint8_t irq_PIN, uint8_t ss_PIN, const char* label, Location loc)
: _logic(logic),
  _nfc(PN532_SCK, PN532_MISO, PN532_MOSI, ss_PIN), 
  _label(label),
  _IRQ_PIN(irq_PIN), 
  location(loc)
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

  if (delayed) {
    if (millis() - last_read_time > DELAY_BETWEEN_CARDS) {
      delayed = false;
      startListening();
    }
  } else {
      irq_curr = digitalRead(_IRQ_PIN);

      // When the IRQ is pulled low - the reader has got something for us.
      if (irq_curr != irq_flick) {
        irq_debounce_time = millis();
        irq_flick = irq_curr;
      }

      if ((millis() - irq_debounce_time) > 100) {
        
        if(irq_steady == HIGH && irq_curr == LOW) {
          RFID_STATE st = cardDetected();
          checkForStateChange(st);
        }

        irq_steady = irq_curr;
      }
  }
}

void PN532Device::startListening() {
  // Reset our IRQ indicators
  irq_steady = irq_flick = irq_curr =  HIGH;
  _nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
}

RFID_STATE PN532Device::cardDetected() {
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  RFID_STATE st = MISSING;
  
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)  
  if (_nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength, 100)) {
    for (uint8_t i = 0; i < 4; i++) {
      readCards[i] = uid[i];
    }
    
    last_read_time = millis();

    st = compareTags() ? CORRECT : INCORRECT;
  } else {
    // Serial.println("Card read FAILED!");
  }

  // The reader will be enabled again after DELAY_BETWEEN_CARDS ms will pass.
  delayed = true;

  return st;
}

bool PN532Device::compareTags() {
  for ( uint8_t i = 0; i < NUM_PIECES; i++ ) {
    bool cardMatch = true;
    ChessPiece p = PIECES[i];

    // filter out pieces not meant for this location
    if ( (location == F2 && ((F2 & p.validLocations) == F2)) ||
         (location == C7 && ((C7 & p.validLocations) == C7)) ) 
    {
      //Serial.printf("looking at p:%d c:%d ps: %s cs: %s\n", p.piece, p.color, pieceStrings[p.piece], colorStrings[p.color]);   

      for ( uint8_t j = 0; j < 4; j++ ) {
          cardMatch = cardMatch && (readCards[j] == p.rfidTag[j]);
      }

      if (cardMatch) {
        return true;
      }  
    } else {
      //Serial.printf("IGNORING p:%d c:%d ps: %s cs: %s\n", p.piece, p.color, pieceStrings[p.piece], colorStrings[p.color]);  
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
