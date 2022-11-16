#include "Section.h"

/*
 * (0)
 */
void Section::fillSolid(){
 for(int i = f; i < l; i++) 
   if (i % nth == 0)
     leds[i] = CHSV(h, s, v);
}

/* fill_grad
 * By: Andrew Tuline
 * Date: August, 2015
 * 
 * FORWARD_HUES: hue always goes clockwise
 * BACKWARD_HUES: hue always goes counter-clockwise
 * SHORTEST_HUES: hue goes whichever way is shortest and is the DEFAULT
 * LONGEST_HUES: hue goes whichever way is longest
 *
 * When the start and end colours are animated, the sweep will flip in the other direction at certain points, thus causing a disruption of the smooth gradient.
 * This routine demonstrates one method to animate the start/end hues and to ensure the fill_gradient remains consistently smooth.
 *
 */
void Section::fillGradient() {
  
  uint8_t starthue = beatsin8(5, 0, 255);
  uint8_t endhue = beatsin8(7, 0, 255);
  
  if (starthue < endhue) {
    fill_gradient(leds, len, CHSV(starthue,255,255), CHSV(endhue,255,255), FORWARD_HUES);    // If we don't have this, the colour fill will flip around. 
  } else {
    fill_gradient(leds, len, CHSV(starthue,255,255), CHSV(endhue,255,255), BACKWARD_HUES);
  }
  
} // fillGradient()

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
  int pos = random16(f, l);
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

/*(9)
 *
 */
void Section::fire2012wPal(){  
  // Step 1.  Cool down every cell a little
  for (int i = f; i < l; i++)
    heat[i] = qsub8(heat[i], random8(0, ((decay * 10) / (len)) + 2));

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for (int k = l - 1; k >= f + 1; k--)
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if (random8() < chance){
    int y = random8(7);
    heat[f + y] = qadd8(heat[y], random8(160, 255));
  }

  // Step 4.  Map from heat cells to LED colors
  for (int j = f; j < l; j++){
    // Scale the heat value from 0-255 down to 0-240 for best results with color palettes.
    byte colorindex = scale8(heat[j], 240);
    leds[j] = ColorFromPalette(currentPalette, colorindex);
  }
}

//  (9)
// Fire2012 with programmable Color Palette
// The dynamic palette shows how you can change the basic 'hue' of the
// color palette every time through the loop, producing "rainbow fire".
// Looks best on a high-density LED setup (60+ pixels/meter).
void Section::fire2012wPalLoop(byte FPS){
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy(random16());

  // set up a new palette every time through the loop, based on a new hue.
  // The palette is a gradient from black, to a dark color based on the hue,
  // to a light color based on the hue, to white.
  if (rotatePalette){
     h++;
     CRGB darkcolor  = CHSV(h,255,192); // pure hue, three-quarters brightness
     CRGB lightcolor = CHSV(h,128,255); // half 'whitened', full brightness
     currentPalette = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);
  }

  fire2012wPal(); // run simulation frame, using palette colors
  FastLED.delay(1000 / FPS); // * or / by speedT?
}

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

/*
 * (15)
 * ColorWavesWithPalettes by Mark Kriegsman: https://gist.github.com/kriegsman/8281905786e8b2632aeb
 * This function draws color waves with an ever-changing,
 * widely-varying set of parameters, using a color palette.
 */
void Section::colorWaves(CRGBPalette16 &palette){
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  // byte sat8 = beatsin88( 87, 220, 250);
  byte brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  byte msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16; // gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 300, 1500);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = f; i < l; i++){
    hue16 += hueinc16;
    byte hue8 = hue16 / 256;
    uint16_t h16_128 = hue16 >> 7;
    if (h16_128 & 0x100){
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    byte bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    byte index = hue8;
    // index = triwave8( index);
    index = scale8(index, 240);

    CRGB newcolor = ColorFromPalette(palette, index, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (l - 1) - pixelnumber;

    nblend(leds[pixelnumber], newcolor, 128);
  }
}
