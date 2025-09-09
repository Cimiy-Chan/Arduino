//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include "C:/Cimiy20181027/Home Projects/Arduino_IDE/libraries/LiquidCrystal_I2C/LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//For Pico-w: GP4(SDA), GP5(SCL)

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello, world!");
  lcd.setCursor(0,1);
  lcd.print("I am Cimiy...");
}


void loop()
{
}
