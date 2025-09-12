#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <string.h>

//The following pin assignment is for Pico-W with 1.14 LCD (240 x 135) ST7789VW
#define LCD_DC 8
#define LCD_CS 9
#define LCD_RST 12
#define LCD_CLK 10
#define LCD_DIN 11
#define User_Key_A 15
#define User_Key_B 17
#define Joystick_Up 2
#define Joystick_Down 18
#define Joystick_Left 16
#define Joystick_Right 20
#define Joystick_Ctrl 3
Adafruit_ST7789 tft = Adafruit_ST7789(LCD_CS, LCD_DC, LCD_DIN, LCD_CLK, LCD_RST);

// color definitions
const uint16_t  Display_Color_Black        = 0x0000;
const uint16_t  Display_Color_Blue         = 0x001F;
const uint16_t  Display_Color_Red          = 0xF800;
const uint16_t  Display_Color_Green        = 0x07E0;
const uint16_t  Display_Color_Cyan         = 0x07FF;
const uint16_t  Display_Color_Magenta      = 0xF81F;
const uint16_t  Display_Color_Yellow       = 0xFFE0;
const uint16_t  Display_Color_White        = 0xFFFF;

// The colors we actually want to use
uint16_t        Display_Text_Color         = Display_Color_Green;
uint16_t        Display_Backround_Color    = Display_Color_Black;

float p = 3.1415926;
const int BI_LED = 32; //This is the default port number of Pico-W build-in LED
int pos_x = 0;
int pos_y = 0;

const size_t Max=16;
char string_val[Max]={0};
int counter = 0;
char display_str[32]={0};
char temp_str[30]={0};

void setup(void) {
  delay (5000); //Delay 5 sec allow user to manually switch Serial Monitor to debug
  Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Test"));
  pinMode(BI_LED, OUTPUT);
  pinMode(User_Key_A, INPUT_PULLUP);
  pinMode(User_Key_B, INPUT);
  pinMode(Joystick_Up, INPUT);
  pinMode(Joystick_Down, INPUT);
  pinMode(Joystick_Left, INPUT);
  pinMode(Joystick_Right, INPUT);
  digitalWrite(BI_LED, HIGH);

  //Use this initializer (uncomment) if using a 1.14" 240x135 TFT:
  tft.init(135, 240);           // Init ST7789 240x135
  

  Serial.println(F("Initialized"));
  tft.fillScreen(ST77XX_BLACK); //Clean up the screen
  delay(100);
  tft.setRotation(3); //Value: 0-3 (0=0, 1=90, 2=180, 3=270), set 3 to display it in landscape view.
  tft.setTextColor(Display_Text_Color);

  sprintf(display_str, "CNT: %.3u", counter);
  print_text(0, 0, 3, Display_Text_Color, Display_Backround_Color, display_str);
  for (counter = 0; counter <=100; counter++) {
    sprintf(display_str, "%.3u", counter);
    print_text(5, 0, 3, Display_Text_Color, Display_Backround_Color, display_str);
    delay (50);
  }
  print_text(0, 1, 3, Display_Text_Color, Display_Backround_Color, "End count");
  //
  get_key_code();
} 

void loop() {
}

int get_key_code (void) {
  while (true) {
    delay (500);
    int code1=digitalRead(User_Key_A);
    Serial.print("Code: " + String(code1));
  }
}

int print_text (uint16_t x, uint16_t y, uint8_t size, uint16_t text_color, uint16_t bg_color,  char* text) {

  //Set x_offset based on text size
  int x_offset = 0;
  int y_offset = 0;
  int return_code = 0;
  switch (size) {
    case 2: x_offset = 15;
            y_offset =18;
            break;
    case 3: x_offset = 20;
            y_offset = 22;
            break;
    default: x_offset =10;
             y_offset = 10;
  }
  int str_len=strlen(text);
  if (str_len !=0){
    for (int i=0; i<str_len; i++) {
      tft.drawChar(x_offset * (x + i), y_offset * y, text[i], text_color, bg_color, size);
    }
    return_code = 0;
  }
  else {
    return_code = 99;
  }
  return return_code;
}

void testlines(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, 0, tft.width()-1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, 0, 0, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, tft.height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
    delay(0);
  }
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t y=0; y < tft.height(); y+=5) {
    tft.drawFastHLine(0, y, tft.width(), color1);
  }
  for (int16_t x=0; x < tft.width(); x+=5) {
    tft.drawFastVLine(x, 0, tft.height(), color2);
  }
}

void testdrawrects(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color);
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=tft.width()-1; x > 6; x-=6) {
    tft.fillRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color1);
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color2);
  }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=radius; x < tft.width(); x+=radius*2) {
    for (int16_t y=radius; y < tft.height(); y+=radius*2) {
      tft.fillCircle(x, y, radius, color);
    }
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < tft.width()+radius; x+=radius*2) {
    for (int16_t y=0; y < tft.height()+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
}

void testtriangles() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 0xF800;
  int t;
  int w = tft.width()/2;
  int x = tft.height()-1;
  int y = 0;
  int z = tft.width();
  for(t = 0 ; t <= 15; t++) {
    tft.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0;
    int y = 0;
    int w = tft.width()-2;
    int h = tft.height()-2;
    for(i = 0 ; i <= 16; i+=1) {
      tft.drawRoundRect(x, y, w, h, 5, color);
      x+=2;
      y+=3;
      w-=4;
      h-=6;
      color+=1100;
    }
    color+=100;
  }
}

void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(3);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(4);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_GREEN);
  tft.print(p, 6);
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(ST77XX_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(" seconds.");
}

void mediabuttons() {
  // play
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRoundRect(25, 10, 78, 60, 8, ST77XX_WHITE);
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_RED);
  delay(500);
  // pause
  tft.fillRoundRect(25, 90, 78, 60, 8, ST77XX_WHITE);
  tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_GREEN);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_GREEN);
  delay(500);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_BLUE);
  delay(50);
  // pause color
  tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_RED);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_RED);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_GREEN);
}
