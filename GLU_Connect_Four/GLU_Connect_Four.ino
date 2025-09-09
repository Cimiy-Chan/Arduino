/*
   Connect Four Game (Demo) for the amomii Glow UNO
   Author: Shaun Armstrong

   Description:
   This Arduino sketch demonstrates a version of the Connect Four game on the Glow UNO 8x8 LED matrix.
   Players take turns dropping colored pieces into a grid, aiming to connect four of their own
   pieces vertically, horizontally, or diagonally before their opponent.

   Hardware:
   - amomii Glow UNO (WS2812B LED matrix) connected to an Arduino UNO or compatible microcontroller
   - Ensure correct wiring and pin configuration.

   Libraries:
   - FastLED: Library for controlling WS2812B LEDs.

   Usage:
   - Use the built-in buttons SW1 and SW3 for moving, and SW2 for selecting to drop pieces into columns.
   - Follow on-screen instructions or LED animations to indicate game state and player turns.

   Note:
   - This is a free demo version for demonstration purposes only.
   - Ensure the FastLED library is installed in your Arduino IDE.
   - This sketch assumes an 8x8 LED matrix; adjust `ROWS` and `COLS` for different configurations.

   For specific hardware details:
   - Visit amomii.com for more information on compatible products.

   Last Updated: 2024/07/12

   Version 1.0
*/

#include <FastLED.h>
#include "digiats.h"
#define NUM_LEDS 64
#define DATA_PIN 9
#define BRIGHTNESS 20 //change the bringhtness with values between 0 and 255
#define ROTATION 1 //you can rotate the screen by changing this value (0 - 3)

const int ROWS = 8;
const int COLS = 8;


int sw1 = 2;
int sw2 = 3;
int sw3 = 4;


//color codes
long black = 0x000000;
int red = 0;
int blue = 1;
int yellow = 2;
int green = 3;
int purple = 4;
int teal = 5;
int fuchsia = 6;
int lime = 7;

long colors[COLS] = { 0xFF0000, 0x0000FF, 0xFFFF00, 0x008000, 0x800080, 0x008080, 0xFF00FF, 0x00FF00 };
uint8_t hue;  // 0 - 255;
int Speed = 20;

//gameplay

bool SELECT = true;
int bottoms[COLS];
const int NUM_PLAYERS = 2;
bool player;
long players[NUM_PLAYERS] = { 0xFFFFFF, 0xFFFFFF };
int fallSpeed = 80;
long grid[ROWS][COLS];
bool winGrid[ROWS][COLS];
int WIN_AMOUNT = 4;
int buz = 11;

// Define the array of leds
CRGB leds[NUM_LEDS];

int pixel[ROWS][COLS];

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(sw3, INPUT_PULLUP);

  for (int i = 0; i < COLS; i++) {
    bottoms[i] = ROWS;
  }

  pinMode(buz, OUTPUT);

  glmx_setup(ROTATION);

  colorSelect();
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
    sw1 = 4;
    sw3 = 2;
  }
}

void colorSelect() {

  resetGame();
  int index = 0;
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      if (one[r][c]) {
        leds[pixel[r][c]] = colors[teal];
      } else {
        leds[pixel[r][c]] = black;
      }
    }
  }

  for (int c = 0; c < COLS; c++) {
    leds[pixel[ROWS - 1][c]] = colors[c];
  }
  FastLED.show();
  players[player] = colors[checkInputs(true)];
  colorSelectFX();
  rainbow_stack_up(255, 255, 255, 10);
  delay(500);
  player = !player;

  index = 0;
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      if (two[r][c]) {
        leds[pixel[r][c]] = colors[teal];
      } else {
        leds[pixel[r][c]] = black;
      }
    }
  }

  for (int c = 0; c < COLS; c++) {
    leds[pixel[ROWS - 1][c]] = colors[c];
  }
  FastLED.show();
  players[player] = colors[checkInputs(true)];
  colorSelectFX();
  rainbow_stack_down(255, 255, 255, 10);
  delay(500);
  player = !player;

  FastLED.clear();
  FastLED.show();
  SELECT = false;
}



int checkInputs(bool colorSelect) {


  bool flag = true;
  bool longPress = false;
  int button;
  bool select = false;

  while (flag) {
    if (digitalRead(sw1) == false && longPress == false) {
      longPress = true;
      if (button < COLS - 1) {
        button++;
      }
    }

    if (digitalRead(sw3) == false && longPress == false) {
      longPress = true;
      if (button > 0) {
        button--;
      }
    }

    if (digitalRead(sw2) == false && longPress == false) {
      select = true;
    }

    if (digitalRead(sw3) == digitalRead(sw2) == digitalRead(sw1) == true) {
      longPress = false;
      if (select) {
        flag = false;
      }
    }

    if (colorSelect) {

      leds[pixel[ROWS - 2][button]] = CRGB::White;
      FastLED.show();
      leds[pixel[ROWS - 2][button]] = black;
    }else{
      leds[pixel[0][button]] = players[player];
      FastLED.show();
      showGrid();
    }
  }
  return (button);
}

void changePlayer() {
  player = !player;
}

void showGrid() {
  int index = 0;
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      leds[pixel[r][c]] = grid[r][c];

    }
  }
  //FastLED.show();
}

void fallInPlace(int col) {
  int chipClick = 0;
  for (int i = 0; i < bottoms[col]; i++) {
    grid[i][col] = players[player];
    showGrid();
    FastLED.show();
    if (i != bottoms[col] - 1) {
      chipClick++;
      grid[i][col] = black;
      int gravity = 10 * i;
      delay(fallSpeed - gravity);
    }else{

    }
  }
  fallingChipFX(chipClick);
  bottoms[col]--;
}

void clearWinGrid() {
  for (int c = 0; c < COLS; c++) {
    for (int r = 0; r < ROWS; r++) {
      winGrid[r][c] = 0;
    }
  }
}

void resetGame() {
  for (int c = 0; c < COLS; c++) {
    for (int r = 0; r < ROWS; r++) {
      winGrid[r][c] = 0;
      grid[r][c] = 0;
    }
    bottoms[c] = 8;
  }
}

bool checkGame() {

  if (checkVerticle()) {
    Serial.println("vert");
    return (true);
  } else if (checkHorizontal()) {
    Serial.println("hor");
    return (true);
  } else if (checkDiagDown()) {
    Serial.println("diagDown");
    return (true);
  } else if (checkDiagUp()) {
    Serial.println("diagUp");
    return (true);
  } else {
    return (false);
  }
}

bool checkVerticle() {
  int winCount = 0;
  bool winner = false;
  //chech verticle
  for (int c = 0; c < COLS; c++) {
    for (int r = 0; r < ROWS; r++) {
      if (grid[r][c] == players[player]) {
        winCount++;
        winGrid[r][c] = 1;
        Serial.println(winCount);
      } else {
        winCount = 0;
        clearWinGrid();
      }
      if (winCount == 4) {
        winner = true;
        break;
      }
    }
    winCount = 0;
    if (winner) {
      break;
    } else {
      clearWinGrid();
    }
  }
  if (!winner) {
    clearWinGrid();
  }
  return (winner);
}

bool checkHorizontal() {
  int winCount = 0;
  bool winner = false;
  //chech verticle
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      if (grid[r][c] == players[player]) {
        winCount++;
        winGrid[r][c] = 1;
        Serial.println(winCount);
      } else {
        winCount = 0;
        clearWinGrid();
      }
      if (winCount == 4) {
        winner = true;
        break;
      }
    }
    winCount = 0;
    if (winner) {
      break;
    } else {
      clearWinGrid();
    }
  }
  if (!winner) {
    clearWinGrid();
  }
  return (winner);
}



bool checkDiagDown() {
  int winCount = 0;
  bool winner = false;
  //chech verticle
  for (int c = 0; c < 5; c++) {
    for (int r = 0; r < 5; r++) {
      if (grid[r][c] == players[player] && grid[r + 1][c + 1] == players[player] && grid[r + 2][c + 2] == players[player] && grid[r + 3][c + 3] == players[player]) {
        winGrid[r][c] = 1;
        winGrid[r + 1][c + 1] = 1;
        winGrid[r + 2][c + 2] = 1;
        winGrid[r + 3][c + 3] = 1;
        winner = true;
        break;
      } else {
        winCount = 0;
        clearWinGrid();
      }
    }  //end of r for

    if (winner) {
      break;
    } else {
      clearWinGrid();
    }
  }  //end of c for

  if (!winner) {
    clearWinGrid();
  }
  return (winner);
}


bool checkDiagUp() {
  int winCount = 0;
  bool winner = false;
  int rowStart = 3;
  //chech verticle
  for (int c = 0; c < 5; c++) {
    for (int r = rowStart; r < rowStart + 5; r++) {
      if (grid[r][c] == players[player] && grid[r - 1][c + 1] == players[player] && grid[r - 2][c + 2] == players[player] && grid[r - 3][c + 3] == players[player]) {
        winGrid[r][c] = 1;
        winGrid[r - 1][c + 1] = 1;
        winGrid[r - 2][c + 2] = 1;
        winGrid[r - 3][c + 3] = 1;
        winner = true;
        break;
      } else {
        winCount = 0;
        clearWinGrid();
      }
    }  //end of r for

    if (winner) {
      break;
    } else {
      clearWinGrid();
    }
  }  //end of c for

  if (!winner) {
    clearWinGrid();
  }
  return (winner);
}

void showWinner() {
  int winFX = 600;
  int index = 0;
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      if (winGrid[r][c]) {
        leds[pixel[r][c]] = 0xFFFFFF;
      } else {
        leds[pixel[r][c]] = players[player];
      }
      FastLED.show();
      tone(buz, winFX);
      winFX += 10;
      delay(20);
    }
  }
  delay(400);
  noTone(buz);

}

void fallingChipFX(int a) {

  randomSeed(millis());
  int hit = 650 + (a * 30);
  for (int i = 0; i < 30; i++) {
    tone(buz, hit);
    hit -= 2;
    delay(1);
  }
  noTone(buz);
}

void colorSelectFX() {
  int baseNote = 2000;  // starting frequency
  int duration = 1;     // duration of each tone
  int numNotes = 80;    // number of tones in the sequence

  for (int i = 0; i < numNotes; i++) {
    int note = baseNote + (i * 100);  // increment frequency for each note
    tone(buz, note, duration);        // play the tone
    delay(duration);                  // wait for the tone to finish
  }

  noTone(buz);  // turn off the buzzer
}

void loop() {
  int action;
  while(1){
    action = checkInputs(false);
    if(bottoms[action] != 0){
      break;
    }
  }
  fallInPlace(action);

  if (checkGame()) {
    Serial.println("yeah boi");
    showWinner();
    delay(1000);
    showGrid();
    FastLED.show();
    delay(3000);
    cross_hoop(255,255,255,177,255,255,40);
    colorSelect();
    return;
  }else{
    changePlayer();
  }

}



void spin_up(byte a, byte b, byte c, int d) {

  for (int r = ROWS - 1; r >= 0; r--) {
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

  for (int r = ROWS - 1; r >= 0; r--) {
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
