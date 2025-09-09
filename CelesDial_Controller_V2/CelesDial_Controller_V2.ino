/*
CelesDial Controller Version 2

Year end project for CelesDial controller. It includes the following hardware attached.

Hardware connection:-
1. Arduino UNO R3 or above
2. 1.28" round OLED with GC9A01A controller (DC=7, CS=9, CLK=13 (Together with Buildin LED), DOUT=12, DIN=11)
3. NeoPixel RGB LED strip with 60 LEDs
4. RTC clock module DS1302
5. Push button (Active)

Student name: Ho Chi Him Samuel
Student number: 230688185
*/

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_GC9A01A.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "Ds1302.h"
#include <Wire.h>

//The following pins out are for Arduino UNO

//OLED around clock face hardware pin connection
#define TFT_DC  7
#define TFT_CS 10
#define TFT_BL 9
#define TFT_CLK 13
#define TFT_DOUT 12
#define TFT_DIN 11

//RTC hardware pin connection
#define RTC_CLK       4
#define RTC_DAT       3
#define RTC_RST       2

//LED Strip (NeoPixel) parameters
#define LED_DATA_PIN 5
#define NUM_LED 60 //Number of LED
#define BRIGHT_LEVEL 50

//Misc hardware pin connection
#define BUTTON_PIN 1

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

//Clock face color parameters (NeoPixel)
#define CLOCK_FACE_COLOR_1 0x002200 //Dark Green
#define CLOCK_FACE_COLOR_2 0x505050 //Dark white
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

//RTC clock constructor
Ds1302 rtc(RTC_RST, RTC_CLK, RTC_DAT);

uint8_t sec_point_x[60]={120,130,140,150,160,169,178,186,194,200,206,211,215,217,219,
                       220,219,217,215,211,206,200,194,186,178,170,160,150,140,130,
                       120,109,99,89,79,70,61,53,45,39,33,28,24,22,20,
                       20,20,22,24,28,33,39,45,53,61,69,79,89,99,109};
uint8_t sec_point_y[60]={20,20,22,24,28,33,39,45,53,61,69,79,89,99,109,
                       120,130,140,150,160,169,178,186,194,200,206,211,215,217,219,
                       220,219,217,215,211,206,200,194,186,178,170,160,150,140,130,
                       120,109,99,89,79,70,61,53,45,39,33,28,24,22,20};

uint8_t led_index[60]={44,45,46,47,48,49,50,51,52,53,
                       54,55,56,57,58,59,0,1,2,3,
                       4,5,6,7,8,9,10,11,12,13,
                       14,15,16,17,18,19,20,21,22,23,
                       24,25,26,27,28,29,30,31,32,33,
                       34,35,36,37,38,39,40,41,42,43};
uint8_t neopixel_clock_face_1[4]={0,15,30,45};
uint8_t neopixel_clock_face_2[12]={4,9,14,19,24,29,34,39,44,49,54,59};
char sec_display[2]="00", hm_display[5]="00:00";


void clock_display (uint8_t hour_val, uint8_t min_val, uint8_t sec_val, bool power_status) {
    //Clear LED color before refreshing
    uint8_t hour_display;

    rgb_led.clear();
    if (hour_val > 12)
      hour_display = hour_val - 12;

    
    //Build or refresh clock face at LED strip
    if (power_status){
      digitalWrite(TFT_BL, HIGH); //OLED back light control.
      for (uint8_t i=0; i<=11; i++)
        rgb_led.setPixelColor(neopixel_clock_face_2[i], CLOCK_FACE_COLOR_2);
      rgb_led.show();
      
      //Show hour/min/sec at LED strip
      uint8_t hour_pos = hour_display*5 + int((min_val * 60 + sec_val)/720);
      rgb_led.setPixelColor(led_index[hour_pos], HOUR_COLOR | rgb_led.getPixelColor(led_index[hour_pos]));
      rgb_led.setPixelColor(led_index[min_val], MIN_COLOR | rgb_led.getPixelColor(led_index[min_val]));
      rgb_led.setPixelColor(led_index[sec_val], SEC_COLOR | rgb_led.getPixelColor(led_index[sec_val]));
    }
    else {
      digitalWrite(TFT_BL, LOW);
    }
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
    sprintf (hm_display, "%02d:%02d", hour_display, min_val);
    tft.print(hm_display);

    //Show AMPM at OLED
    tft.setTextSize(AMPM_TEXT_SIZE);
    tft.setCursor(AMPM_TEXT_POS_X, AMPM_TEXT_POS_Y);
    tft.setTextColor(GC9A01A_MAGENTA, BACKGROUND_COLOR);
    hour_val >= 12 ? tft.print("PM") : tft.print("AM");
}

void setup() {

  //Define variable
  bool is_set_clock = false;
  bool power_on = true;
  bool button_state = false;
  bool led_on_off = false;
  uint8_t last_second = 0;
  uint8_t now_year, now_month, now_day, now_hour, now_minute, now_second, now_week_day;

  //Serial.begin(9600);
  Wire.begin(); //For DS1302 RTC
  rtc.init();
  tft.begin();
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH); // Backlight on. It should be always ON.
  tft.fillScreen(BACKGROUND_COLOR); //Fill background color
  rgb_led.begin();
  rgb_led.setBrightness(BRIGHT_LEVEL);
  rgb_led.clear();
  rgb_led.show();

  //Set date and time
  if (is_set_clock) {
    Ds1302::DateTime dt = {
      .year = 25,
      .month = Ds1302::MONTH_JUL,
      .day = 14,
      .hour = 9,
      .minute = 7,
      .second = 0,
      .dow = Ds1302::DOW_MON
    };
    rtc.setDateTime(&dt);
    is_set_clock = false;
  }

  while (true){
    if (digitalRead(BUTTON_PIN) == LOW){
      if (button_state == false) {
        button_state = true;
        power_on = !power_on;
      }
    }
    else button_state = false;

    Ds1302::DateTime now;
    rtc.getDateTime (&now);

    //Get date and time
    now_hour = now.hour;
    now_minute = now.minute;
    now_second = now.second;

    if (last_second != now_second) {
      last_second = now_second;
    clock_display(now_hour, now_minute, now_second, power_on);
    }
    
    delay (100);
  }
}

void loop(void) {
}
