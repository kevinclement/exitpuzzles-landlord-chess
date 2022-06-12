#include "Arduino.h"
#include "logic.h"
#include <melody_player.h>
#include <melody_factory.h>

#define BUZZER_PIN_1  12
#define BUZZER_PIN_2  27
#define BUZZER_CHANNEL_1 0
#define BUZZER_CHANNEL_2 1

// specify the buzzer's pin and the standby voltage level
MelodyPlayer player(BUZZER_PIN_1, HIGH);

Sound::Sound(Logic &logic)
: _logic(logic)
{
}

void Sound::setup() {
 
}

void Sound::handle() {
}

void Sound::bustTriggered() {
  // const int nNotes = 8;
  // String notes[nNotes] = { "C4", "G3", "G3", "A3", "G3", "SILENCE", "B3", "C4" };
  // const int timeUnit = 175;
  // // create a melody
  // Melody melody = MelodyFactory.load("Nice Melody", timeUnit, notes, nNotes);

  // // get basic info about the melody
  // Serial.println(String(" Title:") + melody.getTitle());
  // Serial.println(String(" Time unit:") + melody.getTimeUnit());
  // Serial.print("Start playing in non-blocking mode...");
  // player.playAsync(melody);
  // Serial.println(" Melody is playing!");

  // Serial.print("Start playing in blocking mode... ");
  // player.play(melody);
  // Serial.println("Melody ends!");

  // delay(1000);

  // ledcAttachPin(BUZZER_PIN_1, BUZZER_CHANNEL_1);
  // ledcAttachPin(BUZZER_PIN_2, BUZZER_CHANNEL_2);

  // ledcWriteNote(BUZZER_CHANNEL_1, NOTE_C, 4);
  // ledcWriteNote(BUZZER_CHANNEL_2, NOTE_C, 4);
  // delay(500);
  // ledcWriteNote(BUZZER_CHANNEL_1, NOTE_D, 4);
  // ledcWriteNote(BUZZER_CHANNEL_2, NOTE_D, 4);
  // delay(500);
  // ledcWriteNote(BUZZER_CHANNEL_1, NOTE_E, 4);
  // ledcWriteNote(BUZZER_CHANNEL_2, NOTE_E, 4);
  // delay(500);
  // ledcWriteNote(BUZZER_CHANNEL_1, NOTE_F, 4);
  // ledcWriteNote(BUZZER_CHANNEL_2, NOTE_F, 4);
  // delay(500);
  // ledcWriteNote(BUZZER_CHANNEL_1, NOTE_G, 4);
  // ledcWriteNote(BUZZER_CHANNEL_2, NOTE_G, 4);
  // delay(500);
  // ledcWriteNote(BUZZER_CHANNEL_1, NOTE_A, 4);
  // ledcWriteNote(BUZZER_CHANNEL_2, NOTE_A, 4);
  // delay(500);
  // ledcWriteNote(BUZZER_CHANNEL_1, NOTE_B, 4);
  // ledcWriteNote(BUZZER_CHANNEL_2, NOTE_B, 4);
  // delay(500);

  // ledcDetachPin(BUZZER_PIN_1);
  // ledcDetachPin(BUZZER_PIN_2);
}

void Sound::solved() {
  ledcWriteNote(BUZZER_CHANNEL_1, NOTE_C, 4);
  ledcWriteNote(BUZZER_CHANNEL_2, NOTE_C, 4);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL_1, NOTE_D, 4);
  ledcWriteNote(BUZZER_CHANNEL_2, NOTE_D, 4);
  delay(500);
  ledcWriteNote(BUZZER_CHANNEL_1, NOTE_E, 4);
  ledcWriteNote(BUZZER_CHANNEL_2, NOTE_E, 4);
  delay(500);
}