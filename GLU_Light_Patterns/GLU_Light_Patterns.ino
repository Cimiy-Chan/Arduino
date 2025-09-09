/*
   Light Patterns and Functions for the amomii Glow UNO
   Author: Shaun Armstrong

   Description:
   This Arduino sketch demonstrates various LED animation effects using the FastLED library
   on the Glow UNO 8x8 LED matrix.

   Hardware:
   - amomii Glow UNO (WS2812B LED matrix) connected to an Arduino UNO as a shield, or wired to other microcontrollers
   - Ensure correct wiring and pin configuration.

   Libraries:
   - FastLED: Library for controlling WS2812B LEDs.

   Note:
   - Ensure the FastLED library is installed in your Arduino IDE.
   - This sketch assumes an 8x8 LED matrix; adjust `ROWS` and `COLS` for different configurations.

   For specific hardware details:
   - Visit amomii.com for more information on compatible products.

   Last Updated: 2024/07/12

   Version 1.1
*/

#include <FastLED.h>  //include the library
#define DATA_PIN 9    // connected to arduino pin 10
#define ROWS 8
#define COLS 8
#define NUM_LEDS COLS*ROWS
#define BRIGHTNESS 50
// Define the array of leds
CRGB leds[NUM_LEDS];

int pixel[ROWS][COLS];

int delayTime = 30;

byte hue = 0;  //0-255

byte hue_spread = 20;


void setup() {

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  //like the on button
  delay(500);
  FastLED.setBrightness(BRIGHTNESS);  // how bright from 0 - 255
  Serial.begin(9600);
  glmx_setup(2);
  FastLED.show();
}

void glmx_setup(int rot) {

  int index = 0;
  if (rot == 0) {
    for (int c = 0; c < COLS; c++) {
      for (int r = 0; r < ROWS; r++) {
        pixel[c][r] = index;
        index++;
      }
    }
  } else if (rot == 1) {
    for (int c = COLS - 1; c >= 0; c--) {
      for (int r = 0; r < ROWS; r++) {
        pixel[r][c] = index;
        index++;
      }
    }
  } else if (rot == 2) {
    for (int c = COLS - 1; c >= 0; c--) {
      for (int r = ROWS - 1; r >= 0; r--) {
        pixel[c][r] = index;
        index++;
      }
    }
  } else if (rot == 3) {
    for (int c = 0; c < COLS; c++) {
      for (int r = ROWS - 1; r >= 0; r--) {
        pixel[r][c] = index;
        index++;
      }
    }
  }
}

//a = hue, b = saturation, c = brightness, d = delay time
void spin_up(byte a, byte b, byte c, int d) {

  for (int r = ROWS - 1; r >= 0; r--) {
    Serial.println("test");
    for (int i = 0; i < COLS; i++) {
      leds[pixel[r][i]] = CHSV(a, b, c);
      FastLED.show();
      delay(d);
    }
  }
}

//a = hue, b = saturation, c = brightness, d = delay time
void spin_down(byte a, byte b, byte c, int d) {

  for (int r = 0; r < ROWS; r++) {
    for (int i = 0; i < COLS; i++) {
      leds[pixel[r][i]] = CHSV(a, b, c);
      delay(d);
      FastLED.show();
    }
  }
}

void hoop_down(byte a, byte b, byte c, int d) {

  for (int r = 0; r < ROWS; r++) {
    for (int i = 0; i < COLS; i++) {
      leds[pixel[r][i]] = CHSV(a, b, c);
    }
    delay(d);
    FastLED.show();
  }
}

void hoop_up(byte a, byte b, byte c, int d) {

  for (int r = 0; r < ROWS; r++) {
    for (int i = 0; i < COLS; i++) {
      leds[pixel[ROWS - 1 - r][i]] = CHSV(a, b, c);
    }

    delay(d);
    FastLED.show();
  }
}

void single_hoop_up(byte a, byte b, byte c, int d) {

  for (int r = ROWS-1; r >= 0; r--) {
    for (int i = 0; i < COLS; i++) {
      leds[pixel[r][i]] = CHSV(a, b, c);
    }
    delay(d);
    FastLED.show();
    for (int clear = 0; clear < NUM_LEDS; clear++) {
      leds[clear] = 0x000000;
    }
  }
}

void single_hoop_down(byte a, byte b, byte c, int d) {

  for (int r = 0; r < ROWS; r++) {
    for (int i = 0; i < COLS; i++) {
      leds[pixel[r][i]] = CHSV(a, b, c);
    }
    delay(d);
    FastLED.show();
    for (int clear = 0; clear < NUM_LEDS; clear++) {
      leds[clear] = 0x000000;
    }
  }
}

void cross_hoop(byte ad, byte bd, byte cd, byte au, byte bu, byte cu, int d) {

  for (int r = 0; r < ROWS; r++) {
    for (int i = 0; i < COLS; i++) {
      leds[pixel[ROWS - 1 - r][i]] = CHSV(au, bu, cu);
      leds[pixel[r][i]] = CHSV(ad, bd, cd);
    }
    delay(d);
    FastLED.show();
    for (int clear = 0; clear < COLS; clear++) {
      leds[pixel[ROWS - 1 - r][clear]] = 0x000000;
      leds[pixel[r][clear]] = 0x000000;
    }
  }
}

void rainbow_stack_down(byte a, byte b, byte c, int d) {
  int index = ROWS;
  for (int s = 0; s < index; s++) {
    for (int r = 0; r < ROWS - s; r++) {
      for (int i = 0; i < COLS; i++) {
        leds[pixel[r][i]] = CHSV(a, b, c);
      }
      delay(d);
      FastLED.show();
      if (r != (ROWS - s) - 1) {
        for (int clear = 0; clear < COLS; clear++) {
          leds[pixel[r][clear]] = 0x000000;
        }
      }
    }
    a += 16;
  }
}

void rainbow_stack_up(byte a, byte b, byte c, int d) {
  int index = ROWS;
  for (int s = 0; s < index; s++) {
    for (int r = 0; r < ROWS - s; r++) {
      for (int i = 0; i < COLS; i++) {
        leds[pixel[ROWS - 1 - r][i]] = CHSV(a, b, c);
      }
      delay(d);
      FastLED.show();
      if (r != (ROWS - s) - 1) {
        for (int clear = 0; clear < COLS; clear++) {
          leds[pixel[ROWS - 1 - r][clear]] = 0x000000;
        }
      }
    }
    a += 16;
  }
}


void loop() {

  spin_up(hue, 255, 255, delayTime);
  spin_down(hue+127, 255, 255,delayTime);

  hue+=hue_spread;

  single_hoop_up(hue, 255, 255, delayTime);
  single_hoop_down(hue+127, 255, 255, delayTime);

  hue+=hue_spread;

  hoop_up(hue, 255, 255, delayTime);
  hoop_down(hue + 127, 255, 255, delayTime);

  hue += hue_spread;

  cross_hoop(hue, 255, 255, hue+127, 255, 255, delayTime);

  hue+=hue_spread;

  cross_hoop(hue, 255, 255, hue+127, 255, 255, delayTime);

  hue+=hue_spread;

  cross_hoop(hue, 255, 255, hue+127, 255, 255, delayTime);

  hue+=hue_spread;

  cross_hoop(hue, 255, 255, hue+127, 255, 255, delayTime);

  hue+=hue_spread;

  rainbow_stack_up(hue, 255, 255, delayTime);
  rainbow_stack_down(hue+127, 255, 255, delayTime);

  hue+=hue_spread;
}