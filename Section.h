#ifndef Section_h
#define Section_h

#include <Arduino.h>
#include <FastLED.h>

#define NUM_ANIMATIONS 20
const int NUM_ACTIVE_PALETTES = 15;

class Section {

  private:

    void fadeIn();                        //
    void fire2012wPal();                  //
    void drawTwinkles(byte twinkleSpeed, byte twinkleDensity, byte autoSelectBG, byte coolLikeIncandescent);
    CRGB computeOneTwinkle( uint32_t ms, byte salt, byte twinkleSpeed, byte twinkleDensity, byte coolLikeIncandescent);
    byte attackDecayWave8(byte i);
    void coolPixel(CRGB& c, byte phase);
    void pacifica_one_layer(CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, byte bri, uint16_t ioff);
    void pacifica_add_whitecaps();
    void pacifica_deepen_colors();
    void chooseNextColorPalette( CRGBPalette16& pal);
    void SetupRandomPalette();
    void SetupTotallyRandomPalette();

  public:

    CRGB *leds;

    byte f;
    byte l;
    byte len;

    byte currentAnimation = 0;

    byte nth = 1;

    byte h = 100;
    byte s = 100;
    byte v = 100;

    byte gHue     = 0;
    byte deltaHue = 7;

    byte whiteSparkles = 1;
    byte chance        = 1;
    byte decay         = 20;
    byte delay         = 0;
    uint16_t  speedT   = 64;

    byte twinkleSpeed = 4;
    byte twinkleDensity = 5;

    uint16_t  noiseVal  = 10;
    uint16_t  scaleN    = 20;
    uint16_t  dist      = 123;

    uint16_t  topHue    = 0;
    uint16_t  bottomHue = 0;

    uint16_t  xScale = 30;
    uint16_t  yScale = 30;
    byte      maxChanges = 4;

    boolean gradient = false;
    boolean gradientDirection = false;
    byte    hueSpeed = 10;

    byte *heat;

//    TProgmemRGBPalette16 *ActivePaletteList;
    byte paletteIndex = 0;
    CRGBPalette16 currentPalette;
    CRGBPalette16 targetPalette;
    TBlendType    currentBlending;
    boolean       rotatePalette = true;

    Section(CRGB *L, byte *heat, byte first, byte last);
    void runAnimation();                  //
    String setAnimation(char* topic, byte message);   //
    String setPalette(byte message);      //
    void update_gHue();                   //
    void fillSolid();                     //
    void addGlitter(fract8 chanceOfGlitter, byte whiteSparkles);
    void rainbow();                       //
    void confetti();                      //
    void noise();                         //
    void BPM();                           //
    void juggle();                        //
    void sinelon();                       //
    void rainbowBeat();                   //
    void pride();                         //
    void fire2012wPalLoop(byte FPS);      //
    void noise8();                        //
    void INoise8Pal();                    //
    void rainbowMarch();                  //
    void RGBflip();                       //
    void colorWaves(CRGBPalette16 &palette); //
    void fadeInLoop();                    //
    void sparkles(byte duration, byte amount, byte spread);
    void twinkleFox(byte secondsPerPalette, byte autoSelectBG, byte coolLikeIncandescent);
    void pacifica();
};

#endif
