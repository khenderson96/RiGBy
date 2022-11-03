#include "Section.h"

/*
 * (0)
 */
void Section::fillSolid(){
 for(int i = f; i < l; i++) 
   if (i % nth == 0)
     leds[i] = CHSV(h, s, v);
}

/* 
 * (1)
 * FastLED's built-in rainbow generator
 */
void Section::rainbow(){
  fill_rainbow(leds + f, len, gHue, deltaHue);
}

/* 
 *  (2)
 * random colored speckles that blink in and fade smoothly
 */
void Section::confetti(){
  fadeToBlackBy(leds + f, len, decay);
  int pos = random16(f, f);
  leds[pos + f] += CHSV(gHue - 32 + random8(64), 200, 255);
}

/* 
 * (3)
 * noise1
 * scaleN scales noise density, changes how likely a pixel is to be the same color as its neighbor
 * noiseVal is a noise value, used to set pixel hue
 * speedt changes how fast the noise moves
 * delay is used for non blocking delay for each frame updat
 */
void Section::noise(){
  for (byte i = f; i < l; i++){
    byte noise = inoise8(i * scaleN + noiseVal, millis() / speedT);
    // byte hue = map(noise, 50, 190, 50, 255);
    // leds[i] = CHSV(hue, 255, 255);
    leds[i] = CHSV(noise, 255, 255);
  }
  FastLED.delay(delay);
} // noise1()

/* (4) bpm :
 *  colored stripes pulsing at a defined Beats-Per-Minute (BPM)
 *  modified by: speedT, currentPalette, gHue
 */
void Section::BPM(){
  byte beat = beatsin8(speedT, 64, 255);
  for (byte i=f; i<l; i++){ // 9948
    leds[i] = ColorFromPalette(currentPalette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

/* (5)
 *  eight colored dots, weaving in and out of sync with each other
 *  modified by: decay
 */
void Section::juggle(){
  /*  TODO #2:
   *    - use colors array and/or palettes
   *    - set number of colored dots
   *    - individual strips
   *    - dot offset (per dot?)
   */
  fadeToBlackBy(leds + f, len, decay);
  byte dothue = 0;
  for (byte i = 0; i < 8; i++)
  {
    leds[beatsin16(i + 7, 0, len - 1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

// 6
// a colored dot sweeping back and forth, with fading trails
// modified by decay, speedT, gHue, ActivePaletteList 
void Section::sinelon(){
  fadeToBlackBy(leds + f, len, decay);
  int pos = beatsin16(speedT, f, l - 1);
  static int prevpos = 0;
  CRGB color = ColorFromPalette(currentPalette, gHue, v, currentBlending);
  if (pos < prevpos){
    fill_solid(leds + pos, (prevpos - pos) + 1, color);
  } else {
    fill_solid(leds + prevpos, (pos - prevpos) + 1, color);
  }
  prevpos = pos;
}

/*
 * (7)
 * takes
 * default deltaHue is 8
 */
void Section::rainbowBeat(){
  byte beatA = beatsin8(17, 0, 255); // Starting hue
  byte beatB = beatsin8(13, 0, 255);
  fill_rainbow(leds + f, l, (beatA + beatB) / 2, deltaHue); // Use FastLED's fill_rainbow routine.
}

/*
 * (8)
 * speed should be 1-3 float
 */
void Section::pride(){
  byte speed = 1.5;
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  byte sat8 = beatsin88(87, 220, 250);
  byte brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  byte msmultiplier = beatsin88(147, 23, 60) * speed;

  uint16_t hue16 = sHue16; // gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (byte i=f; i<l; i++){
    hue16 += hueinc16;
    byte hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    byte bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (l - 1) - pixelnumber;

    nblend(leds[pixelnumber], newcolor, 64);
  }
} // pride

/*
 * (10)
 */
void Section::noise8(){
  for (int i = f; i < l; i++){                                                                      // Just ONE loop to fill up the LED array as all of the pixels change.
    byte index = inoise8(i * xScale, dist + i * yScale) % 255;           // Get a  from the noise function. I'm using both x and y axis.
    leds[i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND); // With that , look up the 8 bit colour palette value and assign it to the current LED.
  }
  dist += beatsin8(10, 1, 4); // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.
                              // In some sketches, I've used millis() instead of an incremented counter. Works a treat.
} // fillnoise8()

/*
 * (11)
 */
void Section::INoise8Pal(){
  EVERY_N_MILLISECONDS_I(speed, 100){
    speed.setPeriod(hueSpeed);
    byte maxChanges = 24;
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges); // AWESOME palette blending capability.
    noise8();                         // Update the LED array with noise at the new location
  }

  EVERY_N_SECONDS(5){ // Change the target palette to a random one every 5 seconds.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 192, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)));
  }
} // inoise8_pal_demo()

// (12)
void Section::rainbowMarch(){                                                       // The fill_rainbow call doesn't support brightness levels.
  byte thishue = millis() * (255 - delay) / 255;    // To change the rate, add a beat or something to the result. 'thisdelay' must be a fixed value.
  thishue = beat8(50);                                  // This uses a FastLED sawtooth generator. Again, the '50' should not change on the fly.
  thishue = beatsin8(50, 0, 255);                       // This can change speeds on the fly. You can also add these to each other.
  fill_rainbow(leds + f, len, thishue, deltaHue); // Use FastLED's fill_rainbow routine.
} // rainbowMarch()


// (14)
void Section::RGBflip(){
  byte first = f;
  byte last = l;
  byte middle = first + (first - last) / 2;
  for (int i = first; i < middle; i++){
    // fade everything out
    fadeToBlackBy(leds + f, len, decay);

//    // let's set an led value
//    leds[i] = CHSV(h++, 255, 255);
//
//    // now, let's first 20 leds to the top 20 leds,
//    leds(middle, last) = leds(middle - 1, first);
    FastLED.delay(33);
  }
}

void Section::fadeInLoop(){
  EVERY_N_MILLISECONDS(100) {                                                 // FastLED based non-blocking FIXED delay.
    uint8_t maxChanges = 24;
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);    // AWESOME palette blending capability.
  }

  EVERY_N_SECONDS(5) {                                                        // Change the target palette to a random one every 5 seconds.
    uint8_t baseC = random8(255);                                             // Use the built-in random number generator as we are re-initializing the FastLED one.
    targetPalette = CRGBPalette16(CHSV(baseC+random8(0,32), 255, random8(128, 255)), CHSV(baseC+random8(0,32), 255, random8(128, 255)), CHSV(baseC+random8(0,32), 192, random8(128, 255)), CHSV(baseC+random8(0,32), 255, random8(128, 255)));
  }

  random16_set_seed(535);                                                           // The randomizer needs to be re-set each time through the loop in order for the 'random' numbers to be the same each time through.
  for (int i = f; i<l; i++) {
    uint8_t fader = sin8(millis()/random8(10,20));                                  // The random number for each 'i' will be the same every time.
    leds[i] = ColorFromPalette(currentPalette, i*20, fader, currentBlending);       // Now, let's run it through the palette lookup.
  }
  random16_set_seed(millis());                                                      // Re-randomizing the random number seed for other routines.
  
}

void Section::addGlitter(fract8 chanceOfGlitter, byte whiteSparkles) {
   if( random8() < chanceOfGlitter) {
     if (whiteSparkles){
       leds[ random16(len) ] += CRGB::White;
     } else {
       leds[ random16(len) ] += CHSV(random8(), 255, 255);
     }
   }
 }

  // sparkles(duration, amount, spread)
  // duration: how long a sparkel lasts [max 255]
  // amount: how often a new random pixel lights up [0-100]
  // spread: how spread out a sparkle becomes [0-172]
//   sparkles(random8(80,200), 100, 30);  // sparkle duration, amount, and spread
void Section::sparkles(byte duration, byte amount, byte spread) {
  static uint8_t sparkle_pixel;
  EVERY_N_MILLISECONDS_I( timingObj, 1) {
    timingObj.setPeriod( duration);
    leds[sparkle_pixel] = CRGB::Black;
    uint8_t previous_pixel = sparkle_pixel;
    while (previous_pixel == sparkle_pixel) {  // pixel can't repeat
      sparkle_pixel = random8(f, l);
    }
    if (random8(100) < amount) {
      //leds[sparkle_pixel] = CRGB(random8(), random8(), random8());
      leds[sparkle_pixel] = CHSV(random8(), random8(20,200), random8(50,255));
    }
  }
  EVERY_N_MILLISECONDS(10) {
    //fadeToBlackBy(leds, NUM_LEDS, 1);  // fade out a bit over time
    blur1d(leds+f, len, spread);  // spreads and fades out color over time
  }
}
