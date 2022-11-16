#include "Section.h"

/*
 * 
 */
Section::Section(CRGB &L, byte *heat, byte first, byte last) {
  this->leds = &L;
  this->f = first;
  this->l = last;
  this->len = last-first;
//  *this->heat = *heat;
  this->dist = random16(1234);
  this->currentBlending = LINEARBLEND;
  this->currentPalette = RainbowColors_p;
  this->targetPalette = PartyColors_p;
}


/*
 * 
 */
void Section::runAnimation(){
  switch(currentAnimation) {
    case 0: {
      fillSolid();
      break; 
    }
    case 1: {
      rainbow();
      break;
    }
    case 2: {
      confetti();
      break;
    }
    case 3: {
      noise();
      break;        
    }
    case 4: {  
      BPM();
      break;  
    }
    case 5: {  
      juggle();
      break;  
    }
    case 6: {  
      sinelon();
      break;  
    }
    case 7: {  
      rainbowBeat();
      break;  
    }
    case 8: {  
      pride();
      break;  
    }
    case 9: {  
      fire2012wPalLoop(60); //fps
      break;  
    }
    case 10: {  
      noise8();
      break;  
    }  
    case 11: {  
      INoise8Pal();
      break;  
    }
    case 12: {  
      rainbowMarch();
      break;  
    }
    case 13: {  
      //rotateSectionHue();
      break;  
    }
    case 14: {  
      //drawRBGflip(); //40 is defailt decay value
      break;  
    }    
    case 15: {  
      colorWaves(currentPalette);
      break;  
    }
    case 16: {  
      //ripples(16, 0.8);
      break;  
    }  
    case 17: {
      twinkleFox(30, 1, 1); //secondsPerPalette, autoSelectBG, coolLikeIncandescent
      break;
    }    
    case 18: {
      fadeInLoop();
      break;
    }
    case 19: {
      sparkles(random8(80,200), 100, 30); // duration [0-255], amount [0-100], spread [0-172]
      break;
    }
    case 20: {
      noise();
      break;
    }
    case 21: {
      pacifica();
      break;
    }
  }
}

/*
 * 
 */
String Section::setAnimation(char* topic, byte message){
  String str = (String)topic;
  String returnMsg = "Changing animation ";
  if (str.indexOf("animationButton") >= 0) {
    if (message == 1){
      returnMsg += "forward";
      currentAnimation++;
      if (currentAnimation > NUM_ANIMATIONS) {
        currentAnimation = 0;
      }
    } else if (message == 2){
      returnMsg += "random";
      currentAnimation = random8(NUM_ANIMATIONS);
    } else if (message == 0){
      returnMsg += "back";
      currentAnimation--;
      if (currentAnimation < 0){
        currentAnimation = NUM_ANIMATIONS;
      }
    }
  } else {
    currentAnimation = message;
  }

  returnMsg += " to animation number: ";
  returnMsg += currentAnimation;

  // resets variables that are recycled across different 
  // animations to their respective initial conditions
  // setStartConditions(sections[i].currentAnimation);
  
  return returnMsg;
}


/*
 * TODO: ambiguous overload for 'operator=' (operand types are 'CRGBPalette16' and 'uint32_t {aka unsigned int}')
 */
String Section::setPalette(byte message){
  String s = "Changing palette ";
  if (message == 1){
    s += "forward";
    paletteIndex++;
    if (paletteIndex > NUM_ACTIVE_PALETTES) {
      paletteIndex = 0;
    }
//    currentPalette = *ActivePaletteList[paletteIndex];
  } else if (message == 2){
    s += "to randomized colors ";
    SetupRandomPalette();
  } else {
    s += "back to paletteIndex: ";
    paletteIndex--;
    if (paletteIndex < 0){
      paletteIndex = NUM_ACTIVE_PALETTES - 1;
    }
//    currentPalette = *ActivePaletteList[paletteIndex];      
  }
  s+= paletteIndex;
  
  return s;
}


/* 
 * Advance to the next color palette in the list  
 */
void Section::chooseNextColorPalette( CRGBPalette16& pal){
  const byte numberOfPalettes = 15;
  static byte whichPalette = -1; 
  whichPalette = addmod8( whichPalette, 1, numberOfPalettes);

//  pal = *(&ActivePaletteList[whichPalette]);
}


/*
 * 
 */
void Section::update_gHue(){
  if (true){  // TODO: add Section variable fot varying/random gHue, or manually set gHue speed
    EVERY_N_MILLISECONDS_I(sp, 100){ 
      sp.setPeriod(hueSpeed);
      gHue++; 
    }
  }
//  } else {
//     EVERY_N_MILLISECONDS_I(sp, 100){ 
//      sp.setPeriod(random8(Sect[sect].minHueSpeed,Sect[sect].maxHueSpeed);
//      Sect[sect].gHue++; 
//    }
//  }
}


/*
 * This function fills the palette with totally random colors. 
 */
void Section::SetupTotallyRandomPalette(){
  for( int i = 0; i < 16; ++i) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}


// which is more interesting than just a gradient of different hues.
void Section::SetupRandomPalette(){
  currentPalette = CRGBPalette16( 
                      CHSV( random8(), 255, 32), 
                      CHSV( random8(), 255, 255), 
                      CHSV( random8(), 128, 255), 
                      CHSV( random8(), 255, 255)); 
}

//##################################################################################################################################
//##################################################################################################################################
//##################################################################################################################################
