#include "Arduino.h"
#include "logic.h"
#include <melody_player.h>
#include <melody_factory.h>

#define BUZZER_PIN_1  12
#define BUZZER_PIN_2  27

MelodyPlayer player1(BUZZER_PIN_1, HIGH);
MelodyPlayer player2(BUZZER_PIN_2, HIGH);

const char bustMelodyStr[] = "bust:d=16,o=5,b=112:32p,f,a#,c6,c#6,c6,c#6,d#6,2f6";
const char solvedMelodyStr[] = "SecretOf:d=4,o=5,b=100:16p,16g,16c6,16d6,8d#.6,8d.6,8c6,16p,16g,16c6,16d6,g6,8p";
const char secretStr[] = "secret:d=8,o=5,b=185:8g5,8f#5,8d#5,8a4,8g#4,8e5,8g#5,4c6";

Melody bustMelody;
Melody solvedMelody;
Melody secretMelody;

Sound::Sound(Logic &logic)
: _logic(logic)
{
}

void Sound::setup() {
  bustMelody = MelodyFactory.loadRtttlString(bustMelodyStr);
  solvedMelody = MelodyFactory.loadRtttlString(solvedMelodyStr);
  secretMelody = MelodyFactory.loadRtttlString(secretStr);
}

void Sound::handle() {
  if (state == PLAYING && !player1.isPlaying()) {
    state = STOPPED;
  }
}

void Sound::beep(unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds)
{
  int x;   
  long delayAmount = (long)(1000000/frequencyInHertz); 
  long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2)); 
  for (x=0;x<loopTime;x++)   
  {   
      digitalWrite(speakerPin,HIGH); 
      delayMicroseconds(delayAmount); 
      digitalWrite(speakerPin,LOW); 
      delayMicroseconds(delayAmount); 
  }
}

void Sound::bustTriggered() {
  // Using old school drawer open for this one, per request
  state = PLAYING;
  beep(BUZZER_PIN_1,1200,100);
  delay(80);
  beep(BUZZER_PIN_1,1200,100);
  beep(BUZZER_PIN_1,1500,200);
}

void Sound::solved() {
  state = PLAYING;
  player1.playAsync(secretMelody);
  player2.playAsync(secretMelody);
}