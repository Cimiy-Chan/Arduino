/*
Final year project for CelesDial controller. It includes the following hardware attached

1. RTC clock module DS1302 (CLK=Pin4, DAT=Pin3, RST=Pin2)
2. Push button (Input=Pin7). ACtive LOW.
3. RGB LED strip with WS2812 (Data-in=Pin10)
4. LCD Module (16 x 2) (SDA=A4, SCL=A5, I2C address=0x27)

Student Name: Ho Chi Him Samuel
Student Number: 230688185

*/

#include <Arduino.h>
//#include <Ds1302.h>
#include "uRTCLib.h"
#include <Adafruit_NeoPixel.h>
//#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Define hardware parameter constant
#define BUTTON_PIN    7
#define LED_DATA_PIN  10
#define RTC_CLK       4
#define RTC_DAT       3
#define RTC_RST       2
#define NeoPixel_PIN  10
#define NUM_LED       48 //LED strip has 48 LEDs but we used them just certain number of LED at specific position
#define BRIGHT_LEVEL  50 //Original 80
#define HIGH          1
#define LOW           0
#define LCD_I2C       0x27
#define RTC_DS1307_I2C  0x68
#define STOP          Serial.println("Stop for debugging");while (true) {}


//Common variables
int button_val = 0;
const static char* week_days[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
const static uint8_t led_pos[12] = {25, 22, 17, 13, 9, 4, 0, 47, 43, 38, 34, 29};
const static uint32_t led_dim[4] = {0x0000cc, 0x000099, 0x000066, 0x000033};

//Ds1302 rtc(RTC_RST, RTC_CLK, RTC_DAT);
uRTCLib rtc(RTC_DS1307_I2C);
Adafruit_NeoPixel rgb_led = Adafruit_NeoPixel(NUM_LED, NeoPixel_PIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(LCD_I2C, 16, 2);

/*
Clock arm: Hour = Red, Min = Green, Sec = Blue
*/
void clock_arm (uint8_t t_hour, uint8_t t_min, uint8_t t_sec) {

  uint8_t t_5_min = 0, t_5_sub_min = 0, t_5_sec = 0, t_5_sub_sec = 0;

  //t_hour *= 5; //Special handle of hour time.
  //t_5_hour = t_hour / 5; // This will not use at hour arm
  //t_5_sub_hour = t_hour % 5; //Thiw will not use at hour arm
  t_5_min = t_min / 5;//As t_5_min is integer, so if t_min 0,1,2,3,4,5,6,7,8,9,10,11...., t_5_min will be 0,0,0,0,0,1,1,1,1,1,2,2...
  t_5_sub_min = t_min % 5; // if t_min 0,1,2,3,4,5,6,7,8,9,10,11...., t_5_sub_min will be 0,1,2,3,4,0,1,2,3,4,0,1...
  t_5_sec = t_sec / 5;
  t_5_sub_sec = t_sec % 5;

  //Hour Arm showing: GREEN
  if (t_hour == 12)
    rgb_led.setPixelColor(led_pos[0], 0x00ff00 | rgb_led.getPixelColor(led_pos[0])); //t_hour always 1, 2...12
  else
    rgb_led.setPixelColor(led_pos[t_hour], 0x00ff00 | rgb_led.getPixelColor(led_pos[t_hour])); //t_hour always 1, 2...12;
  

  /*
  if (t_5_sub_hour == 0) {
    rgb_led.setPixelColor(led_pos[t_5_hour], 0x00ff00 | rgb_led.getPixelColor(led_pos[t_5_hour]));
  }
  else { //This case never happen fpr hour arm, can be deleted this
    if (t_5_hour == 11) { //Special case of 12 -> 1
      rgb_led.setPixelColor(led_pos[11], led_dim[t_5_sub_hour - 1] <<8 |rgb_led.getPixelColor(led_pos[11])); 
      rgb_led.setPixelColor(led_pos[0], led_dim[4 - t_5_sub_hour] <<8 | rgb_led.getPixelColor(led_pos[0])); 

    }
    else { //This will not used, can be deleted it.
      rgb_led.setPixelColor(led_pos[t_5_hour], led_dim[t_5_sub_hour - 1] <<8 | rgb_led.getPixelColor(led_pos[t_5_hour])); 
      rgb_led.setPixelColor(led_pos[t_5_hour + 1], led_dim[4 - t_5_sub_hour] <<8 | rgb_led.getPixelColor(led_pos[t_5_hour + 1])); 

    }
  }
*/
  //Min Arm showing: RED
  if (t_5_sub_min == 0) {
    rgb_led.setPixelColor(led_pos[t_5_min], 0xff0000 | rgb_led.getPixelColor(led_pos[t_5_min]));
  }
  else {
    if (t_5_min == 11) {
      rgb_led.setPixelColor(led_pos[11], led_dim[t_5_sub_min - 1] <<16 |rgb_led.getPixelColor(led_pos[11]));//Gradually dim out
      rgb_led.setPixelColor(led_pos[0], led_dim[4 - t_5_sub_min] <<16 | rgb_led.getPixelColor(led_pos[0]));//Gradually light up

    }
    else {
      rgb_led.setPixelColor(led_pos[t_5_min], led_dim[t_5_sub_min - 1] <<16 | rgb_led.getPixelColor(led_pos[t_5_min]));//Gradually dim out
      rgb_led.setPixelColor(led_pos[t_5_min + 1], led_dim[4 - t_5_sub_min] <<16 | rgb_led.getPixelColor(led_pos[t_5_min + 1]));//Gradually light up

    }
  }

  //Sec Arm showing: BLUE
  if (t_5_sub_sec == 0) {
    rgb_led.setPixelColor(led_pos[t_5_sec], 0x0000ff | rgb_led.getPixelColor(led_pos[t_5_sec]));
  }
  else {
    if (t_5_sec == 11) {
      rgb_led.setPixelColor(led_pos[11], led_dim[t_5_sub_sec - 1] | rgb_led.getPixelColor(led_pos[11]));
      rgb_led.setPixelColor(led_pos[0], led_dim[4 - t_5_sub_sec] | rgb_led.getPixelColor(led_pos[0]));

    }
    else {
      rgb_led.setPixelColor(led_pos[t_5_sec], led_dim[t_5_sub_sec - 1] | rgb_led.getPixelColor(led_pos[t_5_sec]));
      rgb_led.setPixelColor(led_pos[t_5_sec + 1], led_dim[4 - t_5_sub_sec] | rgb_led.getPixelColor(led_pos[t_5_sec + 1]));
    }
  }


  rgb_led.show();

}


void setup() {
  Wire.begin(); //For DS1307 RTC
  static uint8_t last_second = 0;
  uint8_t display_hour = 0;
  bool power_on = true;
  bool button_state = false;
  bool is_set_clock = true; //If you want to set clock, set it to true, set dat/time value, then upload the program
  bool led_on_off = false;

  uint8_t now_year, now_month, now_day, now_hour, now_minute, now_second, now_week_day;

  
  Serial.begin(9600); //Serial port just for debug only.
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //rtc.init();
  rgb_led.begin();
  rgb_led.setBrightness(BRIGHT_LEVEL);

  //Init LCD
  lcd.init();
  lcd.backlight();

  //Clear all LED first
  rgb_led.clear();
  rgb_led.show();

  //Set date and time here
  if (is_set_clock) {
    // rtc.set(second, minute, hour, dayOfWeek, dayOfMonth, month, year)
    rtc.set(25, 59, 12, 2, 18, 6, 25);
    is_set_clock = false;
  }

  //LCD Testing
  lcd.setCursor(1, 0);
  lcd.print(" CelesDial...");

  //User define loop. Will not use Arduino assinged loop()
  while (true) {
    
    if (digitalRead(BUTTON_PIN) == LOW){
      if (button_state == false) {
        button_state = true;
        power_on = !power_on;
      }
    }
    else button_state = false;

    //Ds1302::DateTime now;
    //rtc.getDateTime (&now);

    //Get Date Time
    rtc.refresh();
    now_year = rtc.year();
    now_month = rtc.month();
    now_day = rtc.day();
    now_hour = rtc.hour();
    now_minute = rtc.minute();
    now_second = rtc.second();
    now_week_day = rtc.dayOfWeek();


    if (last_second !=now_second){
      last_second = now_second;
      if (now_hour >12)
        display_hour = now_hour - 12;
      else
        display_hour = now_hour;

      lcd.setCursor(1, 1);
      Serial.print("Time and Date: ");
      lcd.print("Time: ");
      Serial.print("20");
      Serial.print(now_year);    // 00-99
      Serial.print('-');
      if (now_month < 10) Serial.print('0');
      Serial.print(now_month);   // 01-12
      Serial.print('-');
      if (now_day < 10) Serial.print('0');
      Serial.print(now_day);     // 01-31
      Serial.print(' ');
      Serial.print(week_days[now_week_day - 1]); // 1-7
      Serial.print(' ');
      if (now_hour < 10) {
        Serial.print('0');
        lcd.print('0');
      }
      Serial.print(now_hour);
      lcd.print(display_hour);
      lcd.print(':');    // 00-23
      Serial.print(':');
      if (now_minute < 10) {
      Serial.print('0');
      lcd.print('0');
      }
      Serial.print(now_minute);  // 00-59
      lcd.print(now_minute);
      Serial.print(':');
      lcd.print(':');
      if (now_second < 10) {
        Serial.print('0');
        lcd.print('0');
      }
      Serial.print(now_second);  // 00-59
      lcd.print(now_second);
      Serial.println();    
      }
      if (power_on) {
      clock_arm(display_hour, now_minute, now_second);
      led_on_off ?  digitalWrite(LED_BUILTIN, HIGH) : digitalWrite(LED_BUILTIN, LOW);
      led_on_off = !led_on_off;
      delay (100); //Time is ms
      rgb_led.clear();
      }
      else {
        rgb_led.clear();
        rgb_led.show();
        digitalWrite(LED_BUILTIN, LOW);
      }


                                                                                                                                                         
    }
   
}