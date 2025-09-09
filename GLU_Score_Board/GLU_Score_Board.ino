/*
   GLU Score Board
   Author: Shaun Armstrong - amomii

   Description:
   This Arduino sketch controls a single-panel LED matrix using FastLED library to display digits.
   It includes functionality to increment/decrement the displayed digit using push buttons, and 
   randomly change the color hue and saturation of the displayed digit.

   Hardware:
   - amomii Glow UNO or similar WS2812B LED matrix, connected to Arduino UNO or compatible board.
   - Ensure correct wiring: DATA_PIN connected to Arduino pin 9, and appropriate power supply.

   Libraries:
   - FastLED: Library for controlling WS2812B LEDs.

   Features:
   - Displays a single digit (0-9) on the LED matrix.
   - Button sw1 increments the displayed digit.
   - Button sw3 decrements the displayed digit.
   - Button sw2 randomly changes the hue and saturation of the displayed digit.
   - Adjust brightness with BRIGHTNESS constant (0-255).

   Note:
   - Ensure the FastLED library is installed in your Arduino IDE.
   - Adjust ROWS, COLS, PANELS, and ROTATION constants for different LED matrix configurations.

   For more details:
   - Visit amomii.com for hardware specifications and compatibility.

   Last Updated: 2024/07/12
*/


#include "digits.h"
#include <FastLED.h>  //include the library
#define DATA_PIN 9    // connected to arduino pin 10
#define ROWS 8
#define COLS 8
#define PANELS 1
#define ROTATION 1
#define BRIGHTNESS 30
#define NUM_LEDS_PANEL COLS* ROWS
#define NUM_LEDS COLS* ROWS* PANELS
#define TOTAL_DIGITS 10;
#define TOTAL_COLS COLS* PANELS
// Define the array of leds
CRGB leds[NUM_LEDS];

int pixel[ROWS][COLS];

int delayTime = 30;

byte hue = 0;  //0-255

byte hue_spread = 20;

byte HUE = 255;
byte SAT = 255;
byte VIB = 255;

int digit = 0;

int sw1 = 2;
int sw2 = 3;
int sw3 = 4;

bool longPress1 = false;

bool longPress2 = false;

bool longPress3 = false;



void setup() {

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  //like the on button
  delay(500);
  FastLED.setBrightness(BRIGHTNESS);  // how bright from 0 - 255
  //Serial.begin(9600);
  glmx_setup(ROTATION);
  FastLED.show();

  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(sw3, INPUT_PULLUP);
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

int firstDigitHSV(byte d, byte h, byte s, byte v) {



  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      bool bit = bitRead(DIGITS[d][r], (COLS - 1) - c);
      leds[pixel[r][c]] = 0x00000000;
      if (bit) {
        leds[pixel[r][c]] = CHSV(h, s, v);
      }
    }
  }

  FastLED.show();
}

// int secondDigitHSV(byte d, byte h, byte s, byte v) {

//   int digit_offset = NUM_LEDS_PANEL * 1;


//   for (int r = 0; r < ROWS; r++) {
//     for (int c = 0; c < COLS; c++) {
//       bool bit = bitRead(DIGITS[d][r], (COLS - 1) - c);
//       leds[pixel[r][c] + digit_offset] = 0x000000;
//       if (bit) {
//         leds[pixel[r][c] + digit_offset] = CHSV(h, s, v);
//       }
//     }
//   }

//   FastLED.show();
// }

// int thirdDigitHSV(byte d, byte h, byte s, byte v) {

//   int digit_offset = NUM_LEDS_PANEL * 2;


//   for (int r = 0; r < ROWS; r++) {
//     for (int c = 0; c < COLS; c++) {
//       bool bit = bitRead(DIGITS[d][r], (COLS - 1) - c);
//       leds[pixel[r][c] + digit_offset] = 0x000000;
//       if (bit) {
//         leds[pixel[r][c] + digit_offset] = CHSV(h, s, v);
//       }
//     }
//   }

//   FastLED.show();
// }

// int forthDigitHSV(byte d, byte h, byte s, byte v) {

//   int digit_offset = NUM_LEDS_PANEL * 3;


//   for (int r = 0; r < ROWS; r++) {
//     for (int c = 0; c < COLS; c++) {
//       bool bit = bitRead(DIGITS[d][r], (COLS - 1) - c);
//       leds[pixel[r][c] + digit_offset] = 0x000000;
//       if (bit) {
//         leds[pixel[r][c] + digit_offset] = CHSV(h, s, v);
//       }
//     }
//   }

//   FastLED.show();
// }

void loop() {

  if (!digitalRead(sw1) && !longPress1) {
    longPress1 = true;
    digit++;
  }
  if (digitalRead(sw1)) {
    longPress1 = false;
  }

  if (!digitalRead(sw3) && !longPress3) {
    longPress3 = true;
    digit--;
  }
  if (digitalRead(sw3)) {
    longPress3 = false;
  }

  if (!digitalRead(sw2) && !longPress2) {
    randomSeed(millis());
    longPress2 = true;
    HUE = random(0, 255);
    SAT = random(150, 255);
  }
  if (digitalRead(sw2)) {
    longPress2 = false;
  }


  if (digit < 0) {
    digit = 9;
  }
  if (digit > 9) {
    digit = 0;
  }
  firstDigitHSV(digit, HUE, SAT, VIB);
  // secondDigitHSV(r, 100, 255, 255);
  // thirdDigitHSV(r, 200, 150, 255);
  // forthDigitHSV(r, 100, 150, 255);
}