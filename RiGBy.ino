#include "setup.h"

long now;
int value = 0;
unsigned long lastTestMsg = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  pln("\n\n\n\n\n\n\n\nhello Serial!\n\n\n\n\n\n\n\n\n\n");
  
  setupWifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  TelnetStream.begin();
  TelnetStream.print("\n\n\n\n\n\n\n\nhello Telnet!\n\n\n\n\n\n\n\n\n\n");

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(64);
}

void loop() {
  // connect to broker, loop
  client.loop();
  if (!client.connected()) {
    reconnect();
  }  

  // control lights
  if (power == 1){
    FastLED.setBrightness(brightness);
    // preform animation routine
    // master mode will control the entire strip rather than individual sections
    if (masterMode == true){
      sections[NUM_SECTIONS-1].update_gHue();
      sections[NUM_SECTIONS-1].runAnimation();
    } else {
      // iterate through sections and run respective animation routine
      for (int i = 0; i < NUM_SECTIONS-1; i++){
        sections[i].update_gHue();
        sections[i].runAnimation();
      }
    }
  } else {
    FastLED.clear();
  }
  FastLED.show();

  // publish something, print it to telnet/serial
  now = millis();
  lastTestMsg = publishEveryNsec("testout",  "hello world", 5, now, lastTestMsg);

  // publish sensor readings every N seconds
  #ifdef DHT_PIN
    readDHT();
  #endif
}
