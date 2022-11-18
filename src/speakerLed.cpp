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
}