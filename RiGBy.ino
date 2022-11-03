#include "globals.h"

long now;
int value = 0;
unsigned long lastTestMsg = 0;


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  
  setupWifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  TelnetStream.begin();

  LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(64);
}

void loop() {
    client.loop();
  // connect to broker, loop
  if (!client.connected()) {
    reconnect();
  }  
  
  // preform animation routine
  controlLights();

  // publish something and print it to telnet/serial
  now = millis();
  lastTestMsg = publishEveryNsec(withRoot("testout"),  "hello world", 5, now, lastTestMsg);

  // publish sensor readings every N seconds
  #ifdef DHT_PIN
  readDHT();
  #endif
}

void controlLights(){
  if (power == 1){
    FastLED.setBrightness(brightness);
    // master mode will control the entire strip rather than individual sections
    if (masterMode == true){
      sections[NUM_SECTIONS-1].update_gHue();
      sections[NUM_SECTIONS-1].runAnimation();
    } else {
      for (int i = 0; i < NUM_SECTIONS-1; i++){
        sections[i].update_gHue();
        sections[i].runAnimation();
      }
    }
  } else {
    FastLED.clear();
  }
  FastLED.show();
}

#ifdef DHT_PIN
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

unsigned long publishEveryNsec(char* topic, char* message, int rate, unsigned long now, unsigned long prev){
  rate *= 1000;
  if (now - prev > rate) {
    prev = now;
    ++value;
    publishAll(topic, message);
  }
  return prev;
}
