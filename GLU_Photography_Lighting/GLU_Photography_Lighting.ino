/*
   GLU Photography Lighting Control
   Author: Shaun Armstrong
   Date: 2024/07/12

   Description:
   This Arduino sketch enables precise control over lighting parameters (Hue, Saturation, Vibrancy) 
   using the amomii Glow UNO LED matrix. It features two distinct modes:
   - Regular Mode: Allows real-time adjustment of parameters using sw1 (increase), sw2 (mode switch), 
     and sw3 (decrease). The LED matrix visually indicates the active parameter by illuminating a number 
     of LEDs corresponding to the current value.
   - Scroll Mode: Activated by pressing sw1 and sw3 simultaneously, this mode smoothly scrolls through 
     all hues. The scrolling speed is adjusted using sw1 and sw3, with the number of illuminated LEDs 
     representing the current speed setting. 
        - 8 pixels = Hue
        - 16 pixels = Saturation
        - 24 pixels = Vibrancy

   Hardware:
   - amomii Glow UNO (WS2812B LED matrix) connected to an Arduino UNO or compatible microcontroller.
   - Proper wiring and pin configuration are essential.

   Libraries:
   - FastLED: Library for controlling WS2812B LEDs.

   Usage:
   - Regular Mode:
     - Adjust parameters (Hue, Saturation, Vibrancy) using sw1 (increase) and sw3 (decrease).
     - Toggle between parameter modes (Hue, Saturation, Vibrancy) using sw2.
     - LED matrix visually indicates the active parameter by illuminating LEDs corresponding to the current value.
   - Scroll Mode:
     - Activate by pressing sw1 and sw3 simultaneously (and to change back to regular mode).
     - Use sw1 to increase and sw3 to decrease the scrolling speed.
     - Press sw2 to exit Scroll Mode.
     - The number of illuminated LEDs represents the current scrolling speed setting.

   Notes:
   - Ensure the FastLED library is installed in your Arduino IDE.
   - Designed for single 8x8 LED matrix configurations (amomii Glow UNO).
   - Visit amomii.com for hardware details and compatibility.

   Last Updated: 2024/07/12
   Version: 1.0
*/

#include <FastLED.h>  // Include the FastLED library

#define DATA_PIN 9                          // Data pin connected to the LED matrix
#define ROWS 8                              // Number of rows in the LED matrix
#define COLS 8                              // Number of columns in the LED matrix
#define PANELS 1                            // Number of LED panels (can be extended)
#define ROTATION 1                          // Rotation of LED matrix (0 to 3)
#define BRIGHTNESS 150                      // Initial brightness (0-255)
#define NUM_LEDS_PANEL (COLS * ROWS)        // LEDs per panel
#define NUM_LEDS (NUM_LEDS_PANEL * PANELS)  // Total LEDs
#define TOTAL_DIGITS 10                     // Total digits for display
#define TOTAL_COLS (COLS * PANELS)          // Total columns

CRGB leds[NUM_LEDS];  // Define the array of LEDs

int pixel[ROWS][COLS];  // Pixel mapping for LED matrix

int delayTime = 30;  // Delay time between adjustments

byte hue = 0;           // Hue value (0-255)
byte saturation = 255;  // Saturation value (0-255)
byte vibrancy = 255;    // Vibrancy value (0-255)

int digit = 0;  // Current digit displayed

int sw1 = 2;  // Button for incrementing values
int sw2 = 3;  // Button for changing modes
int sw3 = 4;  // Button for decrementing values

bool longPress1 = false;  // Flag for long press on sw1
bool longPress2 = false;  // Flag for long press on sw2
bool longPress3 = false;  // Flag for long press on sw3
bool longPress13 = false;

int mode = 1;  // Current mode: 1 for hue, 2 for saturation, 3 for vibrancy

bool scrollMode = false;  //Press sw1 and sw3 to change the mode


unsigned long timer;
byte scrollTimer = 20;

int TEMP_NUM_LEDS = NUM_LEDS;


void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // Initialize FastLED with LED configuration
  delay(500);
  FastLED.setBrightness(BRIGHTNESS);  // Set initial brightness
  glmx_setup(ROTATION);               // Setup LED matrix
  FastLED.show();                     // Display initial LED state

  pinMode(sw1, INPUT_PULLUP);  // Initialize sw1 as input with pull-up resistor
  pinMode(sw2, INPUT_PULLUP);  // Initialize sw2 as input with pull-up resistor
  pinMode(sw3, INPUT_PULLUP);  // Initialize sw3 as input with pull-up resistor
  timer = millis();
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

void adjustParameter(byte& param, byte minVal, byte maxVal, int increment) {
  int newVal = param + increment;
  if (newVal < minVal) {
    param = maxVal;
  } else if (newVal > maxVal) {
    param = minVal;
  } else {
    param = newVal;
  }
}

void loop() {

  if (!digitalRead(sw1) && !digitalRead(sw3) && !longPress13) {
    longPress13 = true;
    scrollMode = !scrollMode;
  }
  if (digitalRead(sw1) && digitalRead(sw3)) {
    longPress13 = false;
  }
  if (!scrollMode) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = 0x00000000;
    }
    switch (mode) {
      case 1:
        TEMP_NUM_LEDS = 8;
        break;
      case 2:
        TEMP_NUM_LEDS = 16;
        break;
      case 3:
        TEMP_NUM_LEDS = 24;
        break;
      case 4:
        TEMP_NUM_LEDS = NUM_LEDS;
        break;
    }
    if (!digitalRead(sw2) && !longPress2) {
      longPress2 = true;
      mode++;
      if (mode > 4) {
        mode = 1;
      }
    }
    if (digitalRead(sw2)) {
      longPress2 = false;
    }

    if (!digitalRead(sw1) && !longPress1) {
      longPress1 = true;
      switch (mode) {
        case 1:
          TEMP_NUM_LEDS = 8;
          adjustParameter(hue, 0, 255, 10);  // Adjust Hue (H)
          break;
        case 2:
          TEMP_NUM_LEDS = 16;
          adjustParameter(saturation, 0, 255, 10);  // Adjust Saturation (S)
          break;
        case 3:
          TEMP_NUM_LEDS = 24;
          adjustParameter(vibrancy, 0, 255, 10);  // Adjust Vibrancy (V)
          break;
      }
    }
    if (digitalRead(sw1)) {
      longPress1 = false;
    }

    if (!digitalRead(sw3) && !longPress3) {
      longPress3 = true;
      switch (mode) {
        case 1:
          TEMP_NUM_LEDS = 8;
          adjustParameter(hue, 0, 255, -10);  // Adjust Hue (H)
          break;
        case 2:
          TEMP_NUM_LEDS = 16;
          adjustParameter(saturation, 0, 255, -10);  // Adjust Saturation (S)
          break;
        case 3:
          TEMP_NUM_LEDS = 24;
          adjustParameter(vibrancy, 0, 255, -10);  // Adjust Vibrancy (V)
          break;
      }
    }
    if (digitalRead(sw3)) {
      longPress3 = false;
    }

    // Update LEDs with current parameters
    for (int i = 0; i < TEMP_NUM_LEDS; i++) {
      leds[i] = CHSV(hue, saturation, vibrancy);
    }
    FastLED.show();    // Show updated LED state
    delay(delayTime);  // Delay between updates
  } else {
    Serial.println(scrollTimer);
    if (!digitalRead(sw1)) {
      scrollTimer++;
      delay(40);
    }

    if (!digitalRead(sw3)) {
      scrollTimer--;
      delay(40);
    }

    if (!digitalRead(sw3) || !digitalRead(sw1)) {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = 0x00000000;
      }
      TEMP_NUM_LEDS = map(scrollTimer, 255, 0, 0, NUM_LEDS);
    } else {
      TEMP_NUM_LEDS = NUM_LEDS;
    }

    if (millis() - timer > scrollTimer) {
      timer = millis();
      hue++;
      for (int i = 0; i < TEMP_NUM_LEDS; i++) {
        leds[i] = CHSV(hue, saturation, vibrancy);
      }
      FastLED.show();  // Show updated LED state
    }
  }
}
