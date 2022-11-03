/* 
 * Uncomment the controller/board name/location of the target device
 */
// #define SMALL_TEST
#define SMALL_TEST_3_SECTIONS
// #define BIG_TEST
// #define DESK
// #define HEADBOARD
// #define LIVINGROOM

#ifdef SMALL_TEST_3_SECTIONS
  #define ROOT_TOPIC "T1/"
  #define LED_PIN 5
  #define NUM_LEDS 10
  CRGB leds[NUM_LEDS];
  byte heat[NUM_LEDS]; // fire2012: Array of temperature readings at each simulation cell

  #define NUM_SECTIONS 3+1 //  3 individual sections + 1 as the whole strip
  Section sections[] = {
    Section(leds, heat, 0, 2),
    Section(leds, heat, 3, 6),
    Section(leds, heat, 7, 9),
    Section(leds, heat, 0, 9)
  };
#endif

#ifdef BIG_TEST
  #define LED_PIN 12
  #define NUM_LEDS 48 
  #define DHT_PIN 11
  #define DHT_TYPE DHT22
  #define ROOT_TOPIC "lights2/"
#endif

#ifdef DESK
  #define LED_PIN 17
  #define NUM_LEDS 235 //160 (bottom, top front) + 76 (top back
  #define NUM_SECTIONS 4 //  3 individual strips, + 1 as the whole strip
  #define SECT_1_END 80
  #define SECT_2_END 155
  #define BOTTOM 80
  #define DHT_TYPE DHT22
  #define DHT_PIN 32   
  #define ROOT_TOPIC "desk/"
#endif

#ifdef HEADBOARD
  #define LED_PIN 10
  #define DHT_PIN 11
  #define DHT_TYPE DHT22
  //#define DHT_PIN 32    //desk
  #define NUM_LEDS 10
  #define ROOT_TOPIC "lights2/"
#endif

#ifdef LIVINGROOM
  #define LED_PIN 10
  #define DHT_PIN 11
  #define DHT_TYPE DHT22
  //#define DHT_PIN 32    //desk
  #define NUM_LEDS 10
  #define ROOT_TOPIC "lights2/"
#endif
