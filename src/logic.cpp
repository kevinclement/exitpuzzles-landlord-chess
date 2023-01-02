#include "Arduino.h"
#include "logic.h"

Logic::Logic()
  : rfid(*this), 
    bust(*this),
    magnet(*this),
    lights(*this),
    sound(*this),
    cabinet(*this),
    cabinetLed(*this), 
    speakerLed(*this)
{
}

void Logic::setup() {
  rfid.setup();
  bust.setup();
  magnet.setup();
  lights.setup();
  sound.setup();
  cabinet.setup();
  cabinetLed.setup();
  speakerLed.setup();
}

void Logic::handle() {
  rfid.handle();
  bust.handle();
  magnet.handle();
  sound.handle();
  lights.handle();
  cabinet.handle();
  cabinetLed.handle();
  speakerLed.handle();

  // NOTE: do I have to worry about making sure there isn't a conflict with RFID and 
  // bust both going off at the same time? the odds of that are probably pretty low.
  
  // check for bust
  if (bustState == NOT_SOLVED && bust.isSwitched) {
    Serial.println("Bust turned on.  Triggering first.");
    triggerFirst();
  }

  // only mark bust finished when done animating and wait some little bit of time
  if (bustState == SOLVING && sound.state == STOPPED) {
    if (bust_solved_time == 0) {
      bust_solved_time = millis();
    } else if(millis() - bust_solved_time > DELAY_FOR_SOLVES) {
      triggerFirstStageTwo();
    }
  }
  
  // check for second final solved state
  if (rfidState == NOT_SOLVED && rfid.nfc1.state == CORRECT && rfid.nfc2.state == CORRECT && !piecesDisabled) {
    Serial.println("Both RFIDs correct.  Triggering second.");    
    triggerSecond();
  }

  // check for second to finish animation
  if (rfidState == SOLVING && lights.second == PULSING && sound.state == STOPPED) {
    if (rfid_solved_time == 0) {
      rfid_solved_time = millis();
    } else if (millis() - rfid_solved_time > DELAY_FOR_SOLVES) {
      triggerSecondStageTwo();
    }
  }

  if (!solved && bustState == SOLVED && rfidState == SOLVED) {
    triggerFinal();
  }
}

void Logic::togglePiecesDisabled(bool disabled) {
  piecesDisabled = disabled;
  status();
}

void Logic::status() {
  char cMsg[254];
  sprintf(cMsg, 
    "status="
      "piece_1:%s,"
      "piece_2:%s,"
      "rfid_solved:%s,"
      "bust:%s,"
      "bust_solved:%s,"
      "magnet:%s,"
      "cabinet:%s,"
      "cabinetLed:%s,"
      "speakerLed:%s,"
      "lights:%s,"
      "piecesDisabled:%s,"
      "solved:%s"

      "\r\n"
    ,
      rfid.nfc1.state == CORRECT  ? "on"      : "off",
      rfid.nfc2.state == CORRECT  ? "on"      : "off",
      rfidState == SOLVED         ? "true"    : "false",
      bust.isSwitched             ? "on"      : "off",
      bustState == SOLVED         ? "true"    : "false",
      magnet.enabled              ? "enabled" : "disabled",
      cabinet.enabled             ? "enabled" : "disabled",
      cabinetLed.enabled          ? "enabled" : "disabled",
      speakerLed.enabled          ? "enabled" : "disabled",
      lights.second == OFF        ? "false"   : "true",
      piecesDisabled              ? "true"    : "false",
      solved                      ? "true"    : "false"
  );

  Serial.print(cMsg);
}

void Logic::triggerFirst() {
  bustState = SOLVING;
  sound.bustTriggered();
}

void Logic::triggerFirstStageTwo() {
  // NOTE: if we wanted to, we could make it so toggling off bust before animation finished would reset it all
  // right now, triggering once will solve it, and thats it, nothing else should happen. 
  Serial.println("Bust finished sound and animation.  Marking solved.");
  bustState = SOLVED;
  cabinet.enabled = false;
  cabinetLed.enabled = true;
  status();
}

void Logic::triggerSecond() {
  rfidState = SOLVING;
  sound.solved();
  lights.triggerSecond();
}

void Logic::triggerSecondStageTwo() {
  Serial.println("RFID finished sound and animation.  Marking solved.");
  rfidState = SOLVED;
  status();
}

void Logic::triggerFinal() {
  Serial.println("All parts solved.  Marking puzzle solved.");

  // NOTE: we are not turning off magnet here.  we need to control that from the pi because
  // of the sound playing from there.  Then it will handle disabling magnet using command line 
  // directly.
  
  solved = true;
  speakerLed.blinkOn();
  status();
}

void Logic::finish() {
  Serial.println("Finish triggered.  Opening door...");
  speakerLed.blinkOff();
  magnet.enabled = false;
}