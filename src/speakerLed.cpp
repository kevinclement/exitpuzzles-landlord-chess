#include "Arduino.h"
#include "speakerLed.h"
#include "logic.h"

#define BLINK_TIME_MS 150
unsigned long onTime = 0;
unsigned long offTime = 0;

SpeakerLed::SpeakerLed(Logic &logic)
: _logic(logic)
{
}

void SpeakerLed::blinkOn() {
  Serial.println("Turning speaker LED Blinking ON...");
  blinking = true;
  onTime = millis();
  enabled = true;
}

void SpeakerLed::blinkOff() {
  Serial.println("Turning speaker LED Blinking OFF...");
  blinking = false;
  onTime = 0;
  offTime = 0;
  enabled = false;
}

void SpeakerLed::setup() {
  pinMode(SPEAKER_LED_PIN, OUTPUT);
}

void SpeakerLed::handle() {
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

  digitalWrite(SPEAKER_LED_PIN, enabled ? HIGH : LOW);
}
