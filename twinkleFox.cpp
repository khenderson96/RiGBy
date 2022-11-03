#include "Section.h"

void Section::drawTwinkles(byte twinkleSpeed, byte twinkleDensity, byte autoSelectBG, byte coolLikeIncandescent){
  uint32_t clock32 = millis();
  // "PRNG16" is the pseudorandom number generator. It MUST be reset to the same starting value each time
  // this function is called, so that the sequence of 'random' numbers that it generates is (paradoxically) stable.
  uint16_t PRNG16 = 11337;
  
  // Set up the background color, "bg".
  // if autoSelectBG == 1, and the first two colors of the current palette are identical, 
  // then a deeply faded version of that color is used for the background color
  CRGB bg;
  if( (autoSelectBG == 1) && (currentPalette[0] == currentPalette[1] )) {
    bg = currentPalette[0];
    byte bglight = bg.getAverageLight();
    if( bglight > 64) { // very bright, so scale to 1/16th
      bg.nscale8_video( 16); 
    } else if ( bglight > 16) { // not that bright, so scale to 1/4th
      bg.nscale8_video( 64); 
    } else { // dim, scale to 1/3rd.
      bg.nscale8_video( 86); 
    }
  } else {
    bg = CRGB::Black; //gBackgroundColor; // just use the explicitly defined background color
  }

  byte backgroundBrightness = bg.getAverageLight();
  
  for (int i=f; i<l; i++){
    // next 'random' number
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; 

    // use that number as clock offset
    uint16_t myclockoffset16= PRNG16; 

    // next 'random' number
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; 
    
    // use that number as clock speed adjustment factor (in 8ths, from 8/8ths to 23/8ths)
    byte myspeedmultiplierQ5_3 =  ((((PRNG16 & 0xFF)>>4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
    uint32_t myclock30 = (uint32_t)((clock32 * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;

     // get 'salt' value for this pixel
    byte  myunique8 = PRNG16 >> 8;

    // We now have the adjusted 'clock' for this pixel, now we call
    // the function that computes what color the pixel should be based
    // on the "brightness = f( time )" idea.
    CRGB c = computeOneTwinkle( myclock30, myunique8, twinkleSpeed, twinkleDensity, 1);

    byte cbright = c.getAverageLight();
    int16_t deltabright = cbright - backgroundBrightness;
    if( deltabright >= 32 || (!bg)) {
      // If the new pixel is significantly brighter than the background color, use the new color.
      leds[i] = c; // pixel = c;
    } else if( deltabright > 0 ) {
      // If the new pixel brightness is greater than the background color, mix a blend of the new color and the background color
      leds[i] = blend( bg, c, deltabright * 8);
    } else { 
      // if the new pixel is not at all brighter than the background color, just use the background color.
      leds[i] = bg;
    }
  }
}

/*
 * (13) IF BUGGY: REMOVE leds FROM fullFox formal args
 */
void Section::twinkleFox(byte secondsPerPalette, byte autoSelectBG, byte coolLikeIncandescent){
  EVERY_N_SECONDS( secondsPerPalette ) { 
    chooseNextColorPalette( targetPalette ); 
  }
  
  EVERY_N_MILLISECONDS( 10 ) {
    nblendPaletteTowardPalette( currentPalette, targetPalette, 12);
  }

  drawTwinkles( twinkleSpeed, twinkleDensity, autoSelectBG, coolLikeIncandescent);
}

//  This function takes a time in pseudo-milliseconds, figures out brightness = f( time ), and also hue = f( time ).
//  The 'low digits' of the millisecond time are used as input to the brightness wave function.  
//  The 'high digits' are used to select a color, so that the color does not change over the course of the fade-in, fade-out
//  of one cycle of the brightness wave function.
//  The 'high digits' are also used to determine whether this pixel should light at all during this cycle, based on the twinkleDensity.
CRGB Section::computeOneTwinkle( uint32_t ms, byte salt, byte twinkleSpeed, byte twinkleDensity, byte coolLikeIncandescent)
{
  uint16_t ticks = ms >> (8-twinkleSpeed);
  byte fastcycle8 = ticks;
  uint16_t slowcycle16 = (ticks >> 8) + salt;
  slowcycle16 += sin8( slowcycle16);
  slowcycle16 =  (slowcycle16 * 2053) + 1384;
  byte slowcycle8 = (slowcycle16 & 0xFF) + (slowcycle16 >> 8);
  
  byte bright = 0;
  if( ((slowcycle8 & 0x0E)/2) < twinkleDensity) {
    bright = attackDecayWave8( fastcycle8);
  }

  byte hue = slowcycle8 - salt;
  CRGB c;
  if( bright > 0) {
    c = ColorFromPalette( currentPalette, hue, bright, NOBLEND);
    if( coolLikeIncandescent == 1 ) {
      coolPixel( c, fastcycle8);
    }
  } else {
    c = CRGB::Black;
  }
  return c;
}


// This function is like 'triwave8', which produces a 
// symmetrical up-and-down triangle sawtooth waveform, except that this
// function produces a triangle wave with a faster attack and a slower decay:
//
//     / \ 
//    /     \ 
//   /         \ 
//  /             \ 
//
byte Section::attackDecayWave8(byte i){
  if( i < 86) {
    return i * 3;
  } else {
    i -= 86;
    return 255 - (i + (i/2));
  }
}

// This function takes a pixel, checks if its in the 'fading down' part of the cycle, 
// and adjusts the color a little bit to mimic how incandescent bulbs redden as they dim.
void Section::coolPixel( CRGB& c, byte phase){
  if( phase < 128) return;

  byte cooling = (phase - 128) >> 4;
  c.g = qsub8( c.g, cooling);
  c.b = qsub8( c.b, cooling * 2);
}
