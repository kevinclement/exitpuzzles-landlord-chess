#include "Arduino.h"
#include "lights.h"
#include "logic.h"
#include <FastLED.h>

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Define the array of leds
CRGB leds[NUM_LEDS];

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
int gPos = 0;
CRGBPalette16 currentPalette = RainbowColors_p;
TBlendType currentBlending = LINEARBLEND;
#define UPDATES_PER_SECOND 100

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;

      colorIndex = 0;
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}


void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void newSinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  // int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[gPos] += CHSV(gHue, 255, 192);
  gPos++;
  if (gPos >= NUM_LEDS) {
    gPos = 0;
  }
}


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

#define MAX_BRIGHTNESS 180
#define MIN_BRIGHTNESS 75
#define FADE_SPEED 19

void Lights::fadeInAndOut() {
  static boolean fadeDirection = 1;

  // Green - found in example lib, under HUE_GREEN
  static uint8_t hue = 96;
  static uint8_t sat = 255;
  static uint8_t val = 0; // off

  EVERY_N_MILLISECONDS(FADE_SPEED){
    for(int i=0; i<NUM_LEDS; i++) {
      if ((i%2 == 0 && firstHalfSolved) || (i%2 !=0 && secondHalfSolved)) {
          leds[i] = CHSV(hue,sat,val);
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

  FastLED.show();
}

void Lights::handle() {

  fadeInAndOut();
  

  // static uint8_t startIndex = 0;
  // startIndex = startIndex + 1; /* motion speed */
  // FillLEDsFromPaletteColors( startIndex);
  // FastLED.show();
  // FastLED.delay(1000 / UPDATES_PER_SECOND);

  // fadeToBlackBy(leds, NUM_LEDS, 10);
  // FastLED.show();  
  // FastLED.delay(1000/FRAMES_PER_SECOND); 

  // Call the current pattern function once, updating the 'leds' array
  // EVERY_N_MILLISECONDS( 100 ) { 
  // sinelon();
  // }
  
  // send the 'leds' array out to the actual LED strip
  // FastLED.show();
  // // insert a delay to keep the framerate modest
  // FastLED.delay(1000/FRAMES_PER_SECOND);
  
  // Call the current pattern function once, updating the 'leds' array
  // gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  // FastLED.show();  
  // // insert a delay to keep the framerate modest
  // FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  // EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  // EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

void Lights::solvedFirst() {
  firstHalfSolved = true;
}

void Lights::solvedSecond() {
  secondHalfSolved = true;
}