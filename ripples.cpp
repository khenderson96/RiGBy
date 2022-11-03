#include "Section.h"

// int color;
// int center = first;
// int step = -1;
// int maxSteps = 16;
// float fadeRate = 0.8;
// int diff;

// //background color
// uint32_t currentBg = random(256);
// uint32_t nextBg = currentBg;

// void drawRipples(byte si, byte maxSteps, float fadeRate) {
 
//     if (currentBg == nextBg) {
//       nextBg = random(256);
//     } 
//     else if (nextBg > currentBg) {
//       currentBg++;
//     } else {
//       currentBg--;
//     }
//     for(uint16_t l = Sect[si].f; l < Sect[si].l; l++) {
//       leds[l] = CHSV(currentBg, 255, 50);         // strip.setPixelhue(l, Wheel(currentBg, 0.1));
//     }
 
//   if (step == -1) {
//     center = random(Sect[si].f, Sect[si].l);
//     color = random(256);
//     step = 0;
//   }
 
//   if (step == 0) {
//     leds[center] = CHSV(hue, 255, 255);         // strip.setPixelColor(center, Wheel(color, 1));
//     step ++;
//   } 
//   else {
//     if (step < maxSteps) {
//       Serial.println(pow(fadeRate,step));
 
//       leds[wrap(center + step)] = CHSV(hue, 255, pow(fadeRate, step)*255);       //   strip.setPixelColor(wrap(center + step), Wheel(color, pow(fadeRate, step)));
//       leds[wrap(center - step)] = CHSV(hue, 255, pow(fadeRate, step)*255);       //   strip.setPixelColor(wrap(center - step), Wheel(color, pow(fadeRate, step)));
//       if (step > 3) {
//         leds[wrap(center + step - 3)] = CHSV(hue, 255, pow(fadeRate, step - 2)*255);     //   strip.setPixelColor(wrap(center + step - 3), Wheel(color, pow(fadeRate, step - 2)));
//         leds[wrap(center - step + 3)] = CHSV(hue, 255, pow(fadeRate, step - 2)*255);     //   strip.setPixelColor(wrap(center - step + 3), Wheel(color, pow(fadeRate, step - 2)));
//       }
//       step ++;
//     } 
//     else {
//       step = -1;
//     }
//   }
  
//   LEDS.show();
//   delay(50);
// }
 
 
// int wrap(int step) {
//   if(step < 0) return last + step;
//   if(step > last - 1) return step - last;
//   return step;
// }
 
 
// void one_color_allHSV(byte first, byte last, int ahue, int abright) {                // SET ALL LEDS TO ONE COLOR (HSV)
//   for (int i = first ; i < last; i++ ) {
//     leds[i] = CHSV(ahue, 255, abright);
//   }
// }
 
 
 
// // Input a value 0 to 255 to get a color value.
// // The colours are a transition r - g - b - back to r.
// /* uint32_t Wheel(byte WheelPos, float opacity) {
  
//   if(WheelPos < 85) {
//     return strip.Color((WheelPos * 3) * opacity, (255 - WheelPos * 3) * opacity, 0);
//   } 
//   else if(WheelPos < 170) {
//     WheelPos -= 85;
//     return strip.Color((255 - WheelPos * 3) * opacity, 0, (WheelPos * 3) * opacity);
//   } 
//   else {
//     WheelPos -= 170;
//     return strip.Color(0, (WheelPos * 3) * opacity, (255 - WheelPos * 3) * opacity);
//   }
// }
// */
