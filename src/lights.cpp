#include "Arduino.h"
#include "lights.h"
#include "logic.h"
#include <FastLED.h>

#define MAX_BRIGHTNESS  180
#define MIN_BRIGHTNESS  75
#define FADE_SPEED      19
#define HUE_GREEN       96  // Green: found in example lib, under HUE_GREEN  
#define MAX_LOOPS       3

CRGB leds[NUM_LEDS];      // the array of leds
int loops = 0;            // how many loops we've done on a solve
int pos = NUM_LEDS;       // tracks position during a sweep
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
  } else if (first == SWEEPING || second == SWEEPING) {
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
  pos = NUM_LEDS - 1;
  sweepRight = true;
  first = SWEEPING;
  attractMode = false;
  FastLED.clear(true);
}

void Lights::triggerSecond() {
  pos = -1;
  sweepRight = false;
  second = SWEEPING;
  attractMode = false;
  FastLED.clear(true);
}

void Lights::sweep()
{
  EVERY_N_MILLISECONDS(16){
    fadeToBlackBy(leds, NUM_LEDS, 20);

    if (loops <= MAX_LOOPS) {
      if (sweepRight) {
        pos--;
      } else {
        pos++;
      }

      leds[pos] += CHSV(HUE_GREEN, 255, 192);  
      
      if (sweepRight && pos == 0) {
        pos = NUM_LEDS;
        loops++;
      } else if (!sweepRight && pos == NUM_LEDS) {
        pos = -1;
        loops++;
      }
    } else {
        if (leds[0].r == 0 && leds[0].g == 0 && leds[0].b == 0 && leds[NUM_LEDS-1].r == 0 && 
            leds[NUM_LEDS-1].g == 0 && leds[NUM_LEDS-1].b == 0) {
          if (first == SWEEPING) {
            first = PULSING;
          } else if (second == SWEEPING) {
            second = PULSING;
          }
          loops = 0;
          FastLED.clear(true);
        }
    }
  }
}

void Lights::fadeInAndOut() {
  static boolean fadeDirection = 1;
  static uint8_t val = 0; // off

  EVERY_N_MILLISECONDS(FADE_SPEED){
    for(int i=0; i<NUM_LEDS; i++) {
      if ((i%2 == 0 && first == PULSING) || (i%2 !=0 && second == PULSING)) {
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