#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_GC9A01A.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

//The following pins out are for Arduino UNO
#define TFT_DC  7
#define TFT_CS 10
#define TFT_BL 9


//Clock face parameters
#define BLUE_1 0xB73D
#define SEC_TEXT_POS_X 98
#define SEC_TEXT_POS_Y 170
#define HM_TEXT_POS_X 45
#define HM_TEXT_POS_Y 120
#define AMPM_TEXT_POS_X 50
#define AMPM_TEXT_POS_Y 75
#define SEC_TEXT_SIZE 4
#define HM_TEXT_SIZE 5
#define AMPM_TEXT_SIZE 4
#define BACKGROUND_COLOR GC9A01A_BLACK

//LED Strip (NeoPixel) parameters
#define LED_DATA_PIN 4
#define NUM_LED 60 //Number of LED
#define BRIGHT_LEVEL 20

//Clock face color parameters (NeoPixel)
#define CLOCK_FACE_COLOR_1 0x002200 //Dark Green
#define CLOCK_FACE_COLOR_2 0x101010 //Dark white
#define HOUR_COLOR 0xff0000 //Bright red
#define MIN_COLOR 0x0000ff //Bright blue
#define SEC_COLOR 0x00ff00 //Bright green

#define STOP Serial.println("Stop for debugging");while (true) {} //For debugging purpose

// Display constructor for primary hardware SPI connection -- the specific
// pins used for writing to the display are unique to each board and are not
// negotiable. "Soft" SPI (using any pins) is an option but performance is
// reduced; it's rarely used, see header file for syntax if needed.
Adafruit_GC9A01A tft(TFT_CS, TFT_DC); 

// LED strip constructor
Adafruit_NeoPixel rgb_led = Adafruit_NeoPixel(NUM_LED, LED_DATA_PIN, NEO_GRB + NEO_KHZ800);

uint8_t sec_point_x[60]={120,130,140,150,160,169,178,186,194,200,206,211,215,217,219,
                       220,219,217,215,211,206,200,194,186,178,170,160,150,140,130,
                       120,109,99,89,79,70,61,53,45,39,33,28,24,22,20,
                       20,20,22,24,28,33,39,45,53,61,69,79,89,99,109};
uint8_t sec_point_y[60]={20,20,22,24,28,33,39,45,53,61,69,79,89,99,109,
                       120,130,140,150,160,169,178,186,194,200,206,211,215,217,219,
                       220,219,217,215,211,206,200,194,186,178,170,160,150,140,130,
                       120,109,99,89,79,70,61,53,45,39,33,28,24,22,20};
uint8_t neopixel_clock_face_1[4]={0,15,30,45};
uint8_t neopixel_clock_face_2[12]={0,5,10,15,20,25,30,35,40,45,50,55};
char sec_display[2]="00", hm_display[5]="00:00";


void clock_display (uint8_t hour_val, uint8_t min_val, uint8_t sec_val) {
    //Clear LED color before refreshing
    rgb_led.clear();
    
    //Build or refresh clock face at LED strip
    for (uint8_t i=0; i<=11; i++)
      rgb_led.setPixelColor(neopixel_clock_face_2[i], CLOCK_FACE_COLOR_2);
    rgb_led.show();
    
    //Show hour/min/sec at LED strip
    uint8_t hour_pos = hour_val*5 + int((min_val * 60 + sec_val)/720);
    rgb_led.setPixelColor(hour_pos, HOUR_COLOR | rgb_led.getPixelColor(hour_pos));
    rgb_led.setPixelColor(min_val, MIN_COLOR | rgb_led.getPixelColor(min_val));
    rgb_led.setPixelColor(sec_val, SEC_COLOR | rgb_led.getPixelColor(sec_val));
    rgb_led.show();


    //Show seconds value and dots around clock at OLED
    if (sec_val == 0) {
      for (uint8_t i=0; i<=59; i++) {
        tft.fillCircle(sec_point_x[i], sec_point_y[i], 2, BACKGROUND_COLOR);  //Refresh 
      }
    }
      
    tft.setTextSize(SEC_TEXT_SIZE);
    tft.setCursor(SEC_TEXT_POS_X, SEC_TEXT_POS_Y);
    tft.fillCircle(sec_point_x[sec_val], sec_point_y[sec_val], 2, BLUE_1);
    tft.setTextColor(GC9A01A_RED, BACKGROUND_COLOR);
    sprintf(sec_display, "%02d", sec_val);
    tft.print(sec_display);

    //Show minute and hour values in HH:MM format at OLED
    tft.setTextSize(HM_TEXT_SIZE);
    tft.setCursor(HM_TEXT_POS_X, HM_TEXT_POS_Y);
    tft.setTextColor(GC9A01A_GREEN, BACKGROUND_COLOR);
    sprintf (hm_display, "%02d:%02d", hour_val, min_val);
    tft.print(hm_display);

    //Show AMPM at OLED
    tft.setTextSize(AMPM_TEXT_SIZE);
    tft.setCursor(AMPM_TEXT_POS_X, AMPM_TEXT_POS_Y);
    tft.setTextColor(GC9A01A_MAGENTA, BACKGROUND_COLOR);
    hour_val >= 12 ? tft.print("PM") : tft.print("AM");
}

void setup() {
  Serial.begin(9600);
  tft.begin();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH); // Backlight on. It should be always ON.
  tft.fillScreen(BACKGROUND_COLOR); //Fill background color
  rgb_led.begin();
  rgb_led.setBrightness(BRIGHT_LEVEL);
  rgb_led.clear();
  rgb_led.show();

  //Clock simulation   
  for (uint8_t hour = 0; hour <= 11; hour++)
    for (uint8_t min = 0; min<=59; min++)
      for (uint8_t sec = 0; sec <=59; sec++) {
          clock_display(hour, min, sec);
          delay (100);
      }
          


  while (true); //Not use the Arduino stupid loop
}

void loop(void) {
}
