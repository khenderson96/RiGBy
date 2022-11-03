#include "Section.h"

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
