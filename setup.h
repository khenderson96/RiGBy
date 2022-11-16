#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <TelnetStream.h>
#include <FastLED.h>
#include "Section.h"
#include "secrets.h"

WiFiClient espClient;
PubSubClient client(espClient);
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

// change DEBUG to 0 to stop printing to serial/reduce code size for deployment.
#define DEBUG 1
#if DEBUG == 1
  #define p(x) Serial.print(x)
  #define pln(x) Serial.println(x)
#endif

/* ##########################################################################
 * ###                         BOARD DEFINITIONS                          ###
 * ##########################################################################
 * 
 * Uncomment the controller/board name/location of the target device.
 * 
 * When adding new hardware, NUM_SECTIONS should always be declared 
 * with one additional section to encompass the entire strip.  
 * For example, if there are 3 sections, define NUM_SECTIONS as 4.
 * Therefore there should never be less than 2+1 sections.
 */

#define BOARD1_MATRIX_TEST_3_SECTIONS
// #define SMALL_TEST
// #define SMALL_TEST_3_SECTIONS
// #define BIG_TEST
// #define DESK
// #define HEADBOARD
// #define LIVINGROOM

#ifdef BOARD1_MATRIX_TEST_3_SECTIONS
  // root topic is used to differentiate among different controllers
  #define ROOT_TOPIC "T1/"
  #define BUILTIN_LED 2
  #define LED_PIN 4
  #define NUM_LEDS 24
  #define COLOR_ORDER GRB
  #define LED_TYPE WS2812B
  #define NUM_SECTIONS 3+1 //  3 individual sections + 1 as the whole strip
  // define where each section starts/stops.
  #define SECTION_0_MAX 7   // leds 0 to 7
  #define SECTION_1_MAX 15  // leds 8 to 15
  // SECTION_2_MAX=NUM_LEDS // leds 16 to 23
#endif

#ifdef SMALL_TEST_3_SECTIONS
  #define ROOT_TOPIC "T1/"
  #define LED_PIN 5
  #define NUM_LEDS 10
  #define COLOR_ORDER GRB
  #define LED_TYPE WS2812B
  #define NUM_SECTIONS 3+1  // 3 individual sections + 1 as the whole strip
  #define SECTION_0_MAX 2   // leds 0 to 2   
  #define SECTION_1_MAX 6   // leds 3 to 6
#endif

#ifdef BIG_TEST
  #define LED_PIN 12
  #define NUM_LEDS 48
  #define COLOR_ORDER RGB
  #define LED_TYPE WS2811
  #define DHT_PIN 11
  #define DHT_TYPE DHT22
  #define ROOT_TOPIC "lights2/"
#endif

#ifdef DESK
  #define LED_PIN 17
  #define NUM_LEDS 235 // 160 (bottom, top front) + 76 (top back)
  #define NUM_SECTIONS 3+1 //  3 individual + 1 whole strip
  #define SECTION_0_MAX 80
  #define SECTION_1_MAX 155
  #define BOTTOM 80
  #define DHT_TYPE DHT22
  #define DHT_PIN 32   
  #define ROOT_TOPIC "desk/"
#endif

#ifdef HEADBOARD
  #define LED_PIN 10
  #define DHT_PIN 11
  #define DHT_TYPE DHT22
  #define NUM_LEDS 10
  #define ROOT_TOPIC "lights1/"
#endif

#ifdef LIVINGROOM
  #define LED_PIN 10
  #define DHT_PIN 11
  #define DHT_TYPE DHT22
  #define NUM_LEDS 10
  #define ROOT_TOPIC "lights2/"
#endif

/* ##########################################################################
 * ###                           LED GLOBALS                              ###
 * ##########################################################################
 *
 * declare led array now that NUM_LEDS is defined for the specific hardware configuration
 */
CRGB leds[NUM_LEDS];
byte heat[NUM_LEDS];            // Array of temperature readings at each simulation cell for fire2012()

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

//##########################################################################
//###                             SECTIONS                               ###
//##########################################################################
/*
 * declare individual sections now that the entire led array has been declared
 */
#if NUM_SECTIONS == 5
  Section sections[] = {
    Section(*leds, *heat, 0, SECTION_0_MAX),
    Section(*leds, *heat, SECTION_0_MAX+1, SECTION_1_MAX),
    Section(*leds, *heat, SECTION_1_MAX+1, SECTION_2_MAX),
    Section(*leds, *heat, SECTION_1_MAX+1, SECTION_2_MAX),
    Section(*leds, *heat, 0, NUM_LEDS-1)
  };
#elif NUM_SECTIONS == 4
  Section sections[] = {
    Section(*leds, *heat, 0, SECTION_0_MAX),
    Section(*leds, *heat, SECTION_0_MAX+1, SECTION_1_MAX),
    Section(*leds, *heat, SECTION_1_MAX+1, NUM_LEDS-1),
    Section(*leds, *heat, 0, NUM_LEDS-1)
  };
#elif NUM_SECTIONS == 3
  Section sections[] = {
    Section(leds, heat, 0, SECTION_0_MAX),
    Section(leds, heat, SECTION_0_MAX+1, SECTION_1_MAX),
    Section(leds, heat, 0, NUM_LEDS-1)
  };
#endif

//##########################################################################
//###                         TEMPERATURE                                ###
//##########################################################################
/*
 * initalize variables for DHT temperature sensor readings
 */
#ifdef DHT_PIN 
  #include "DHT.h"
  DHT dht(DHT_PIN, DHT_TYPE);
  float hum;  
  float temp;
  unsigned long lastTempMsg;
  unsigned long lastHumMsg;

  void readDHT(){
    temp = dht.readTemperature()*1.8+32;
    char tempBuf[9];
    dtostrf(temp, 8, 3, tempBuf);  
    hum = dht.readHumidity();
    char humBuf[9];
    dtostrf(hum, 8, 3, humBuf);   
    lastTempMsg = publishEveryNsec(withRoot("temp"), tempBuf, 10, now, lastTempMsg);
    lastHumMsg = publishEveryNsec(withRoot("hum"), humBuf, 10, now, lastHumMsg);
  }
#endif

#ifdef OTHER_TEMP_SENSOR_PIN 
  #include "actual library name.h"
  float hum;  
  float temp;
  unsigned long lastTempMsg;
  unsigned long lastHumMsg;
#endif


//##########################################################################
//###                              TOPICS                                ###
//##########################################################################
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
      //"gradientDirection"
};

// topics that will be subscibed to once per controller
const byte NUM_SINGLE_TOPICS = 3;             
char* singleTopicSect[NUM_SINGLE_TOPICS] = { 
      "handshake",      // bool     .
      "power",          
      "brightness",                                     
};

//##########################################################################
//###                         EXPERIMENTAL                               ###
//##########################################################################

#ifndef PHOTORESITOR_PIN
  byte sensor_brightness;
  //sensor_brightness = analogRead(PHOTORESISTOR_PIN);
  //sensor_brightness = map(...
  //void controlBrightness(){ if (sensor_brightness > threshold){...
#endif

//// Background color for 'unlit' pixels in TwinkleFox
//CRGB gBackgroundColor = CRGB::Black; //CRGB(CRGB::FairyLight).nscale8_video(16);
