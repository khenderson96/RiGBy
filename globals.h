#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <TelnetStream.h>
#include <FastLED.h>
#include "Section.h"
#include "hardware.h"
#include "secrets.h"

const byte BUILTIN_LED = 2;

WiFiClient espClient;
PubSubClient client(espClient);

// change DEBUG to 0 to stop printing to serial/reduce code size for deployment.
#define DEBUG 1
#if DEBUG == 1
#define p(x) Serial.print(x)
#define pln(x) Serial.println(x)
#endif

#define COLOR_ORDER GRB
#define LED_TYPE WS2812B
#define FRAMES_PER_SECOND 60    // Suggested range of 30-100fps --> interframe delay of ~10-35ms.
#define maxRipples 6            // Min is 2 and value has to be divisible by two because each ripple has a left and right component. This cuts down on bouncing code.
byte secondsPerPalette = 30;    // How often to change color palettes.
byte power = 0;                 // must be enabled to control all strips.
byte masterMode = 1;            // when mastermode is 1, the entire strip is treated as a single section.
byte brightness = 10;           // brightness: master brightness control, used by all sections
static int16_t dist;            // A random number for our noise generator.
boolean rotate_palette = true;  // fire2012
 
// When autoSelectBG is set to 1, for any palette where the first two colors match, 
// a dimmed version of that color will automatically be used as the background color.
byte autoSelectBG = 1;

// If COOL_LIKE_INCANDESCENT is set to 1, colors will  fade out slightly 'reddened' to emulate dim incandesence.
byte coolLikeIncandescent = 1;




#ifdef DHT_PIN 
  #include "DHT.h"
  float hum;  
  float temp;
  unsigned long lastTempMsg;
  unsigned long lastHumMsg;
#endif

// topics that will be subscribed to once PER section
const byte NUM_MULTI_TOPICS = 11;
char* multiTopicSect[NUM_MULTI_TOPICS] = { 
      "color",          
      "pattern",        
      "nthLed",         
      "speed",          
      "deltaHue",       
      "chance",         
      "decay",          
      "palette",        
      "paletteButton",  
      "animation",
      "animationButton"  
      //"gradient",
      //"gradientDirection",
      //                               
};

// topics that will be subscibed to once per controller
const byte NUM_SINGLE_TOPICS = 3;             
char* singleTopicSect[NUM_SINGLE_TOPICS] = { 
      "handshake",      // bool     .
      "power",          
      "brightness",                                     
};

#ifdef DHT_PIN 
  DHT dht(DHT_PIN, DHT_TYPE);
#endif

//##########################################################################
//###                         EXPERIMENTAL                               ###
//##########################################################################

//// Background color for 'unlit' pixels in TwinkleFox
//CRGB gBackgroundColor = CRGB::Black; //CRGB(CRGB::FairyLight).nscale8_video(16);
