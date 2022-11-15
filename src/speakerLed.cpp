#include "Arduino.h"
#include "speakerLed.h"
#include "logic.h"

SpeakerLed::SpeakerLed(Logic &logic)
: _logic(logic)
{
}

void SpeakerLed::setup() {
  pinMode(SPEAKER_LED_PIN, OUTPUT);
}

void SpeakerLed::handle() {
  digitalWrite(SPEAKER_LED_PIN, enabled ? HIGH : LOW);

  if (_prevEnabled != enabled) {
    Serial.print("speaker led change: ");
    Serial.println(enabled ? "ENABLED" : "DISABLED");

    _prevEnabled = enabled;
    _logic.status();
  }
}