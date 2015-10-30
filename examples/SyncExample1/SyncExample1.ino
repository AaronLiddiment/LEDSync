/*
LEDSync class example

This example is for a 720x24 led matrix using three Teensy 3.1's with OctoWS2811 adapters, 1 pin per 720 leds.
Each Teensy runs the same software but shifts its text window & sprite position based on the Board ID returned by the cLEDSync class.

Written & tested on a Teensy 3.1 using Arduino V1.6.3 & teensyduino V1.22
*/

#define USE_OCTOWS2811
#include <OctoWS2811.h>
#include <FastLED.h>

#include <LEDMatrix.h>
#include <LEDText.h>
#include <LEDSprites.h>
#include <LEDSync.h>

#include <ComicSansP24.h>

// Change the next 3 defines to match your matrix type and size
#define MATRIX_WIDTH   720
#define MATRIX_HEIGHT  8
#define MATRIX_TYPE    HORIZONTAL_MATRIX

cLEDMatrix<-MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;

cLEDText ScrollingMsg;

const unsigned char TxtDemo[] = { EFFECT_FRAME_RATE "\x00"
                                  EFFECT_SCROLL_LEFT "                                                                                          "
                                  " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}\x7e\x7f      "
                                  "The Quick Brown Fox Jumps Over The Lazy Dog Again and Again!"
                                  "                                             "
                                  };

#define READY_PIN   			 22
#define WAIT_PIN    			 23
#define BOARD_ID_LOW_PIN   0
#define BOARD_ID_HIGH_PIN  1

cLEDSync<READY_PIN, WAIT_PIN, BOARD_ID_LOW_PIN, BOARD_ID_HIGH_PIN> BoardSync;

uint8_t BoardNumber;


const uint8_t Minion1EyeData[] = 
{
  B8_3BIT(00011111),B8_3BIT(10000000),
  B8_3BIT(00111221),B8_3BIT(11000000),
  B8_3BIT(01112332),B8_3BIT(11100000),
  B8_3BIT(04423443),B8_3BIT(24400000),
  B8_3BIT(04423443),B8_3BIT(24400000),
  B8_3BIT(01112332),B8_3BIT(11100000),
  B8_3BIT(01111221),B8_3BIT(11100000),
  B8_3BIT(01141111),B8_3BIT(41100000),
  B8_3BIT(01114444),B8_3BIT(11100000),
  B8_3BIT(01111111),B8_3BIT(11100000),
  B8_3BIT(05111111),B8_3BIT(11500000),
  B8_3BIT(01555555),B8_3BIT(55100000),
  B8_3BIT(01155555),B8_3BIT(51100000),
  B8_3BIT(01155555),B8_3BIT(51100000),
  B8_3BIT(01555555),B8_3BIT(55100000),
  B8_3BIT(44555555),B8_3BIT(55440000),
  B8_3BIT(04555555),B8_3BIT(55400000),
  B8_3BIT(00044004),B8_3BIT(40000000),
  B8_3BIT(00444004),B8_3BIT(44000000),
};
const uint8_t Minion1EyeMask[] = 
{
  B8_1BIT(00011111),B8_1BIT(10000000),
  B8_1BIT(00111111),B8_1BIT(11000000),
  B8_1BIT(01111111),B8_1BIT(11100000),
  B8_1BIT(01111111),B8_1BIT(11100000),
  B8_1BIT(01111111),B8_1BIT(11100000),
  B8_1BIT(01111111),B8_1BIT(11100000),
  B8_1BIT(01111111),B8_1BIT(11100000),
  B8_1BIT(01111111),B8_1BIT(11100000),
  B8_1BIT(01111111),B8_1BIT(11100000),
  B8_1BIT(01111111),B8_1BIT(11100000),
  B8_1BIT(01111111),B8_1BIT(11100000),
  B8_1BIT(01111111),B8_1BIT(11100000),
  B8_1BIT(01111111),B8_1BIT(11100000),
  B8_1BIT(01111111),B8_1BIT(11100000),
  B8_1BIT(01111111),B8_1BIT(11100000),
  B8_1BIT(11111111),B8_1BIT(11110000),
  B8_1BIT(01111111),B8_1BIT(11100000),
  B8_1BIT(00011001),B8_1BIT(10000000),
  B8_1BIT(00111001),B8_1BIT(11000000),
};
const struct CRGB Minion1EyeColTab[] =  {  CRGB(255, 255, 0), CRGB(128, 128, 128), CRGB(255, 255, 255), CRGB(64, 64, 64), CRGB(0, 0, 255), CRGB(255, 0, 0), CRGB(255, 0, 0)  };

cLEDSprites Sprites(&leds);

cSprite SprMinion1Eye(12, 19, Minion1EyeData, 1, _3BIT, Minion1EyeColTab, Minion1EyeMask);


void setup()
{
  BoardNumber = BoardSync.BoardNo();

  FastLED.addLeds<OCTOWS2811>(leds[0], (MATRIX_HEIGHT / 8) * MATRIX_WIDTH);
  FastLED.setBrightness(64);
  FastLED.clear(true);
  delay(500);
  FastLED.showColor(CRGB::Red);
  delay(1000);
  FastLED.showColor(CRGB::Lime);
  delay(1000);
  FastLED.showColor(CRGB::Blue);
  delay(1000);
  FastLED.showColor(CRGB::White);
  delay(1000);
  FastLED.show();

  ScrollingMsg.SetFont(ComicSansP24Data);
  ScrollingMsg.Init(&leds, leds.Width(), (MATRIX_HEIGHT * 3) + 1, 0, -((BoardNumber * MATRIX_HEIGHT) + 0) );
  ScrollingMsg.SetText((unsigned char *)TxtDemo, sizeof(TxtDemo) - 1);
  ScrollingMsg.SetTextColrOptions(COLR_HSV | COLR_GRAD_AV, 0x40, 0xff, 0xff, 0xe0, 0xff, 0xff);
  ScrollingMsg.SetOptionsChangeMode(INSTANT_OPTIONS_MODE);
  
  SprMinion1Eye.SetPositionFrameMotionOptions(600/*X*/, (-(BoardNumber * MATRIX_HEIGHT)) + 3/*Y*/, 0/*Frame*/, 0/*FrameRate*/, +1/*XChange*/, 2/*XRate*/, 0/*YChange*/, 0/*YRate*/);
  Sprites.AddSprite(&SprMinion1Eye);
}


void loop()
{
  if (ScrollingMsg.UpdateText() == -1)
  {
    ScrollingMsg.SetText((unsigned char *)TxtDemo, sizeof(TxtDemo) - 1);
    ScrollingMsg.UpdateText();
  }
  Sprites.UpdateSprites();
  Sprites.RenderSprites();
  if (SprMinion1Eye.m_X >= 720)
    SprMinion1Eye.m_X = 600;

  BoardSync.Wait();

  FastLED.show();

  BoardSync.NotReady();
}
