#include "Arduino.h"
#include "logic.h"

Logic::Logic()
  : rfid(*this), 
    bust(*this),
    magnet(*this),
    lights(*this),
    sound(*this)
{
}

void Logic::setup() {
  rfid.setup();
  bust.setup();
  magnet.setup();
  lights.setup();
  sound.setup();
}

void Logic::handle() {
  rfid.handle();
  bust.handle();
  magnet.handle();
  sound.handle();
  lights.handle();
  
  // check for bust
  if (bust.isSwitched && lights.first == INIT) {
    Serial.println("Bust turned on.  Triggering first.");
    triggerFirst();
  }

  // only mark bust finished when done animating and wait some little bit of time
  if (!bustSolved && lights.first == SOLVED && sound.state == STOPPED) {
    if (bust_solved_time == 0) {
      bust_solved_time = millis();
    } else if(millis() - bust_solved_time > DELAY_FOR_SOLVES) {
      // NOTE: if we wanted to, we could make it so toggling off bust before animation finished would reset it all
      // right now, triggering once will solve it, and thats it, nothing else should happen. 
      Serial.println("Bust finished sound and animation.  Marking solved.");
      bustSolved = true;
      status();
    }
  }
  
  // check for second final solved state
  if (!solved && rfid.nfc1.state == CORRECT && rfid.nfc2.state == CORRECT) {
    Serial.println("Both RFIDs correct.  Puzzle solved.");
    triggerSecond();
    solved = true;
    status();
  }

  // Magnet will be on when switch is disabled
  // magnet.enabled = !bust.isSwitched;


  // TODO: do I have to worry about making sure there isn't a conflict with RFID and bust both going off at the same time?
  // the odds of that are probably pretty low.
}

void Logic::status() {
  char cMsg[254];
  sprintf(cMsg, 
    "status="
      "piece_1:%s,"
      "piece_2:%s,"
      "bust:%s,"
      "bust_solved:%s,"
      "magnet:%s,"
      "solved:%s"

      "\r\n"
    ,
      rfid.nfc1.state == CORRECT  ? "true" : "false",
      rfid.nfc2.state == CORRECT  ? "true" : "false", 
      bust.isSwitched             ? "on" : "off",
      bustSolved                  ? "on" : "off",
      magnet.enabled              ? "enabled" : "disabled",
      solved                      ? "true" : "false"
  );

  Serial.print(cMsg);
}

void Logic::triggerFirst() {
  sound.bustTriggered();
  lights.triggerFirst();
}

void Logic::triggerSecond() {
  sound.solved();
  lights.triggerSecond();
}
