#include "Arduino.h"
#include "bust.h"
#include "logic.h"

#define BOUNCE_TIME_MS 250

unsigned long pressedTime = 0;
Bust::Bust(Logic &logic)
: _logic(logic)
{
}

void Bust::setup() {
  pinMode(BTN_SWITCH_PIN, INPUT_PULLDOWN);
}

void Bust::handle() {
  bool _prevSwitched = isSwitched;
  int btnState = digitalRead(BTN_SWITCH_PIN);

  // if (!btnState) {
  //     pressedTime = 0;
  //     isSwitched = false;
  // }
  // else {
  //     if (pressedTime == 0) {
  //         pressedTime = millis();
  //     }
  //     else if (millis() - pressedTime > BOUNCE_TIME_MS) {
  //         isSwitched = true;
  //     }
  // }

  if (btnState && !isSwitched) {
      if (pressedTime == 0) {
          pressedTime = millis();
      }
      else if (millis() - pressedTime > BOUNCE_TIME_MS) {
          isSwitched = true;
      }
  }
  
  if (_prevSwitched != isSwitched) {
    Serial.print("bust: ");
    Serial.println(isSwitched ? "ON" : "OFF");
    _logic.status();
  }
}