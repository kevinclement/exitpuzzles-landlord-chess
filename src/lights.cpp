#include "Arduino.h"
#include "lights.h"
#include "logic.h"
#include <FastLED.h>

#define MAX_BRIGHTNESS  180
#define MIN_BRIGHTNESS  75
#define FADE_SPEED      19
#define HUE_GREEN       96  // Green: found in example lib, under HUE_GREEN  
#define MAX_LOOPS       5

CRGB leds[NUM_LEDS];      // the array of leds
int loops = 0;            // how many loops we've done on a solve
uint8_t pos = NUM_LEDS;   // tracks position during a sweep
bool sweepRight = true;   // should we sweep right or left

Lights::Lights(Logic &logic)
: _logic(logic)
{
}

void Lights::setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS)
         .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
}

void Lights::handle() {

  if (attractMode) {
    fancyPants();
  } else if (solvingFirst || solvingSecond) {
    sweep();
  } else {
    fadeInAndOut();
  }

  FastLED.show();
}

void Lights::triggerAttractMode() {
  attractMode = true;
}

void Lights::triggerFirst() {
  pos = NUM_LEDS;
  sweepRight = true;
  solvingFirst = true;
  attractMode = false;
  FastLED.clear();
}

void Lights::triggerSecond() {
  pos = 0;
  sweepRight = false;
  solvingSecond = true;
  attractMode = false;
  FastLED.clear();
}

void Lights::sweep()
{
  EVERY_N_MILLISECONDS(16){
    fadeToBlackBy(leds, NUM_LEDS, 20);

    if (loops <= MAX_LOOPS) {
      leds[pos] += CHSV(HUE_GREEN, 255, 192);
          
      if (sweepRight) {
        pos--;
      } else {
        pos++;
      }
      
      if (sweepRight && pos <= 0) {
        pos = NUM_LEDS;
        loops++;
      } else if (!sweepRight && pos >= NUM_LEDS) {
        pos = 0;
        loops++;
      }
    } else {
      // check to see if we've fully faded after sweeping
      if (leds[NUM_LEDS-1].r == 0 && leds[NUM_LEDS-1].g == 0 && leds[NUM_LEDS-1].b == 0)
      {
        if (solvingFirst) {
          solvingFirst = false;
          solvedFirst = true;
        } else {
          solvingSecond = false;
          solvedSecond = true;
        }
        loops = 0;
      }
    }
  }
}

void Lights::fadeInAndOut() {
  static boolean fadeDirection = 1;
  static uint8_t val = 0; // off

  EVERY_N_MILLISECONDS(FADE_SPEED){
    for(int i=0; i<NUM_LEDS; i++) {
      if ((i%2 == 0 && solvedFirst) || (i%2 !=0 && solvedSecond)) {
          leds[i] = CHSV(HUE_GREEN,255,val);
      }
    }
    
    if (fadeDirection == 1) {
      val += 1;      
    } else {
      val -= 1;
    }

    if ((fadeDirection == 1 && val == MAX_BRIGHTNESS) || 
        (fadeDirection == 0 && val == MIN_BRIGHTNESS)) {
      fadeDirection = !fadeDirection;  //reverse direction
    }
  }
}

void Lights::fancyPants() {

  EVERY_N_MILLISECONDS(120) {
    static uint8_t gHue = 0;
    gHue++;

    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy(leds, NUM_LEDS, 3);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV(gHue + random8(64), 200, 50);
  }
  
}