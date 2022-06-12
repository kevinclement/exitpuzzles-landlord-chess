#include "Arduino.h"
#include "logic.h"

#define BUZZER_PIN_1  12
#define BUZZER_PIN_2  27
#define BUZZER_CHANNEL_1 0
#define BUZZER_CHANNEL_2 1

Sound::Sound(Logic &logic)
: _logic(logic)
{
}

void Sound::setup() {
  ledcAttachPin(BUZZER_PIN_1, BUZZER_CHANNEL_1);
  ledcAttachPin(BUZZER_PIN_2, BUZZER_CHANNEL_2);
}

void Sound::handle() {
}

void Sound::bustTriggered() {
  ledcWriteNote(BUZZER_CHANNEL_1, NOTE_C, 4);
  ledcWriteNote(BUZZER_CHANNEL_2, NOTE_C, 4);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL_1, NOTE_D, 4);
  ledcWriteNote(BUZZER_CHANNEL_2, NOTE_D, 4);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL_1, NOTE_E, 4);
  ledcWriteNote(BUZZER_CHANNEL_2, NOTE_E, 4);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL_1, NOTE_F, 4);
  ledcWriteNote(BUZZER_CHANNEL_2, NOTE_F, 4);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL_1, NOTE_G, 4);
  ledcWriteNote(BUZZER_CHANNEL_2, NOTE_G, 4);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL_1, NOTE_A, 4);
  ledcWriteNote(BUZZER_CHANNEL_2, NOTE_A, 4);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL_1, NOTE_B, 4);
  ledcWriteNote(BUZZER_CHANNEL_2, NOTE_B, 4);
  delay(500);
}

void Sound::solved() {

}