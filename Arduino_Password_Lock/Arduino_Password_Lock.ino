#include <Wire.h>
#include <Keypad.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <Buzzer.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define BUZZER_PIN 12
#define BEEP 1
#define NO_BEEP 0
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Buzzer buzzer(12);


const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 7, 6, 5, 4 };
byte colPins[COLS] = { 11, 10, 9, 8 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String input = "";           // 儲存輸入字符
String password = "123456";  // 預設密碼
String maskedInput = "";

enum SystemState {
 STATE_0,
 STATE_1,
 STATE_2,
 //STATE_4,
};

SystemState currentState = STATE_1;
unsigned long stateStartTime = 0;

void lock(void) {
  display.setRotation(2);
  display.setTextSize(2);
  display.clearDisplay();
  display.setCursor(30, 0);
  display.print("ENTER    PASASWORD");
  display.display();
}

void unlock(void) {
  display.setRotation(2);
  display.setTextSize(3);
  display.clearDisplay();
  display.setCursor(0, 5);
  display.print("Welcome");
  digitalWrite(BUZZER_PIN, BEEP);
  delay (100);
  digitalWrite(BUZZER_PIN, NO_BEEP);
  display.display();
  //delay(1000);
}

unsigned long alertStartTime = 0;
bool alertActive = false;

void alert(void) {
  //buzzer.begin(0);

  display.setRotation(2);
  display.setTextSize(2);
  display.clearDisplay();
  display.setCursor(30, 0);
  display.print("WRONG    PASSWORD");
  digitalWrite(BUZZER_PIN, BEEP);
  delay (500);
  digitalWrite(BUZZER_PIN, NO_BEEP);
  display.display();

  //buzzer.sound(1, 1);
  //buzzer.distortion(NOTE_C5, NOTE_C3);

  //buzzer.end(1000);
  //delay(2000);
}

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT); // Pin for buzzer
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  //Buzzer(OUTPUT);
  //Buzzer(LOW);
  display.setRotation(2);
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.display();
  lock();
}

void loop() {
  char key = keypad.getKey();
  unsigned long currentTime = millis();

  display.setRotation(2);
  display.clearDisplay();
  display.setCursor(10,1);

  if (key) {
      if (key == 'C') {  // "" 清空輸入
      //Serial.println(key);
      input = "";
      maskedInput = "";
      //buzzer.end(0);
      currentState = STATE_0;
      lock();
      alertActive = false;
      //return;
    } 
    if (key && input.length() < 6 && key != 'C'){ //Input only for valid password char
      input += key;  // 限制最大 6 位數字
      maskedInput += '*';
    }
    //display.print("ENTER PASSWORD");
    for (int i = 0; i < input.length(); i++) {
      display.setTextSize(3);
      //display.setCursor(0, 10);
      display.print('*');
    }
    display.display();
  }


  if (input.length() == 6){
      delay(500);
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 10);
      
      if (input == password) {
        currentState = STATE_1; //Correct password
      } else {
        currentState = STATE_2; //Wrong password
        alertStartTime = millis();
        alertActive = true;
        alert();
      }

  Serial.print ("Input length: ");
  Serial.print(input.length());
  Serial.print ("  State: ");
  Serial.println(currentState);
  switch (currentState) {
    case STATE_0:
    lock();
    break;

    case STATE_1:
      unlock();
      delay(3000);
      currentState = STATE_0;
      input = "";
      lock();
      break;

    case STATE_2: //Wrong password
      if (alertActive && ((millis() - alertStartTime) >= 2000)){
        Serial.print("Time elasped: ");
        Serial.println(millis() - alertStartTime);
        //buzzer.end(0);
        alertActive = false;
        currentState = STATE_0;
        input = "";
        lock();
      }
      alertActive = false; //Copied from if true state
      currentState = STATE_0;
      input = "";
      lock();
      break;
    }
  }
}

