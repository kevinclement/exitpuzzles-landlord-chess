#include "Arduino.h"
#include "cabinetLed.h"
#include "logic.h"

#define BLINK_TIME_MS 150

unsigned long onTime = 0;
unsigned long offTime = 0;

CabinetLed::CabinetLed(Logic &logic)
: _logic(logic)
{
}

void CabinetLed::setup() {
  pinMode(CABINET_LED_PIN, OUTPUT);
}

void CabinetLed::blinkOn() {
  Serial.println("Turning cabinet LED Blinking ON...");
  blinking = true;
  onTime = millis();
  enabled = true;
}

void CabinetLed::blinkOff() {
  Serial.println("Turning cabinet LED Blinking OFF...");
  blinking = false;
  onTime = 0;
  offTime = 0;
  enabled = false;
}

void CabinetLed::handle() {
  if (blinking && onTime > 0 && millis() - onTime > BLINK_TIME_MS) {
    enabled = false;
    onTime = 0;
    offTime = millis();
  }

  if (blinking && offTime > 0 && millis() - offTime > BLINK_TIME_MS) {
    enabled = true;
    offTime = 0;
    onTime = millis();
  }

  digitalWrite(CABINET_LED_PIN, enabled ? HIGH : LOW);
}