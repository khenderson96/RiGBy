#include "Section.h"

/*
 * Called by the callback function in network.ino 
 * Assigns incoming message value to appropriate variable
 */
void setSectionVariables(byte i, char* varName, int val){
  String varStr = (String)varName;
  
  if (varStr == String("power")) {                
    setPower(val);
  }
  else if (varStr == String("brightness")){
    brightness = val;   
  } 
  else if (varStr == String("masterMode")){
    masterMode = val;
  }
  else if (varStr == String("hsv")){
    setHSV(varName, val);
  }
  else if (varStr.indexOf(withRoot("animation")) >= 0){ // covers both animationButton and animationDropDown varNames{
    setSectionAnimation(i, varName, val);
  }
  else if (varStr.indexOf("paletteButton") >= 0) {
    setSectionPalette(i, val);
  }
  else if (varStr.indexOf("nthled") >= 0){
    sections[i].nth = val;
  } 
  else if (varStr.indexOf("deltaHue") >= 0){
    sections[i].deltaHue = val;
  } 
  else if (varStr.indexOf("chance") >= 0){
    sections[i].chance = val;
  } 
  else if (varStr.indexOf("hueSpeed") >= 0){
    sections[i].hueSpeed = val;
  } 
  else if (varStr.indexOf("decay") >= 0){
    sections[i].decay = val;
  } 
  else if (varStr.indexOf("x") >= 0){
    sections[i].noiseVal = val;
  } 
  else if (varStr.indexOf("scale") >= 0){                  //
    sections[i].scaleN = val;
  } 
  else if (varStr.indexOf("speed") >= 0){                  //
    sections[i].speedT = val;
  } 
  else if (varStr.indexOf("topHue") >= 0){                 
    sections[i].topHue = val;
  } 
  else if (varStr.indexOf("bottomHue") >= 0){              
    sections[i].bottomHue = val;
  }
}


/*
 * Called by setSectionVariables
 * Sets section palette and prints debug message returned by Section object.
 */
void setSectionPalette(byte i, byte val){
  printlnAll(sections[i].setPalette(val));
}

/*
 * Called by setSectionVariables
 * Sets section animation, prints debug message returned by Section object, and publishes current animation index for test purposes.
 */
void setSectionAnimation(byte i, char* varName, byte val){
  printlnAll(sections[i].setAnimation(varName, val));
  // publish current animation index so we can keep track in the nodeRed dashboard (until TODO is implemented)
  // TODO #1: use function node to init idx to 0, then increment or decrement (fwd payload = 1, back = -1, ...
  // ... rand mapped with JS rand num generator???)
  char idxBuf[3];
  dtostrf(sections[i].currentAnimation, 2, 0, idxBuf); 
  publishAll(withRoot("animationIndex"), idxBuf);
}

/*
 * Called by setSectionVariables
 * Sets power for entire strip and prints status.
 */
void setPower (byte val){
  printAll("Turning lights ");
  if(val == 1){
    printlnAll("on.");
    power = 1;
  } else if(val == 0){
    printlnAll("off.");
    power = 0;
  }
}

/*
 * 
 */
void setHSV(char* var, int value){
  char i = var[strlen(var)-3];          //index of color to change EX root/1/h. note h/s/v is always the last char.
  char c = var[strlen(var)-1];   
  
  if (c == 'h'){
    sections[i].h = value;
  } else if (c == 's'){
    sections[i].s = value;
  } else if (c == 'v'){
    sections[i].v = value;
  }

  printHSVarray();
}


// unimplemented ####################################################################
// ##################################################################################
// ##################################################################################

/*
 * 
 */
void setStartConditions(byte i, byte currentAnimation){
//  if (animation == 1){
//    sections[i].h == 0;
//  }
}


/*
 * 
 */
int hash (char *str) {
  int hash = 5381;
  int c;
  while (c = *str++)
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash;
}


/*
 * 
 */
char* withRoot(char* topic){
  static char buf[64];
  strcpy(buf, ROOT_TOPIC);
  strcat(buf, topic);
  return buf;
}
