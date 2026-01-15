/*
Stepper motor + Rainbow LED Button demo

Hardware configuration:-
Stepper motor:
IN1 = D4
IN2 = D5
IN3 = D6
IN4 = D7

Rainbow LED Button
SW1 = D8
SW2 = D9
SW3 = D10
SW4 = D11
LED_IN = D12
*/

#include <Adafruit_NeoPixel.h>

#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

#define BUTTON_1_PIN 8
#define BUTTON_2_PIN 9
#define BUTTON_3_PIN 10
#define BUTTON_4_PIN 11

#define NeoPixel_PIN 12
#define BRIGHT_LEVEL 80

#define CLOCKWISE true
#define ANTICLOCKWISE false
#define LONG_PRESS_REPEAT true
#define LONG_PRESS_NOT_REPEAT false

//Full step sequence for clockwise rotation
const byte fullStepSequence[4][4] = {
  { HIGH, LOW, LOW, LOW },
  { LOW, HIGH, LOW, LOW },
  { LOW, LOW, HIGH, LOW },
  { LOW, LOW, LOW, HIGH }
};
const uint8_t button_pin_map[4] = {BUTTON_1_PIN, BUTTON_2_PIN, BUTTON_3_PIN, BUTTON_4_PIN};


Adafruit_NeoPixel rgb_led = Adafruit_NeoPixel(4, NeoPixel_PIN, NEO_GRB + NEO_KHZ800);


/*
stepper_motor_move
speed = speed of motor (1-10). 1=slowest, 10=fastest
step = number of step to be move. One rotation of output shaft = 512 step
direction: 0=Clockwise, 1=Anticlockwise
*/
void stepper_motor_move(uint8_t speed, uint16_t step, bool direction) {

  if (speed > 0 && speed < 11) {
    if (direction == true) {
      for (uint16_t i = 0; i < step; i++) {
        for (uint8_t j = 0; j < 4; j++) {
          digitalWrite(IN1, fullStepSequence[j][0]);
          digitalWrite(IN2, fullStepSequence[j][1]);
          digitalWrite(IN3, fullStepSequence[j][2]);
          digitalWrite(IN4, fullStepSequence[j][3]);
          delay(12 - speed);  //2ms is the shortest time
        }
      }
    } else {
      for (uint16_t i = 0; i < step; i++) {
        for (uint8_t j = 0; j < 4; j++) {
          digitalWrite(IN1, fullStepSequence[j][0]);
          digitalWrite(IN2, fullStepSequence[j][3]);
          digitalWrite(IN3, fullStepSequence[j][2]);
          digitalWrite(IN4, fullStepSequence[j][1]);
          delay(12 - speed);  //2ms is the shortest time
        }
      }
    }
  }
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 0);
}

/*
- Button status routine which can detect key is either short press or long press
- duration_1 (ms) = range in 10ms to 500ms. 10ms per step. Duration for short press (typical value: 20ms)
- duration_2 (ms) = range in 110ms to 1500ms. 10ms per step. Duration for long press (typical value: 1000ms)
- duration_1 < duration 2
- is_repeat. 0 = Not repeat and exit. 1 = repeat to execute the function defined at func_to_be_exec
- Return value contain the information of 'short' or 'long' press with button id. Return 0 if key press time less than duration_1 or invalid duration_1/_2 setting. 
- Upper nibble (byte7-byte4): Button ID. Lower nibble (byte3-byte0): 1=short press, 2=long press, 3=long press and repeatly (if button is holding on) execut function 
  defined at func_to_be_exec
- For example: return value 0x31 means that BUTTON 3 is 'short' press. 
*/
uint8_t button_status(uint16_t duration_1, uint16_t duration_2, bool is_repeat, void(*func_to_be_exec)(const char *) = NULL, char *func_arg = NULL) {
  uint8_t button_counter = 0;

  uint8_t button_id = get_button_id();
  if ((duration_1 >= 10 && duration_1 <= 500) && (duration_2 >= 110 && duration_2 <= 1500) && (duration_1 < duration_2)) {
    uint8_t step_1 = (int)(duration_1 / 10.0);
    uint8_t step_2 = (int)(duration_2 / 10.0);
    while (true) { //looping until key is release
      if (digitalRead(button_pin_map[button_id - 1]) == LOW) { //button press and hold
        button_counter++;
        //Serial.println(button_counter);
        delay(10);
        if (button_counter > step_2 && is_repeat == true) {  //Press and hold until time > step_2
          func_to_be_exec(func_arg);
          delay (100);
        }
        if (button_counter > step_2 && is_repeat == false)
          return 2 + button_id * 16;
      } 
      else {  //button release
        if (button_counter > step_1 && button_counter <= step_2) 
          return 1 + button_id * 16;
        if (button_counter > step_2 && is_repeat == true)
          return 3 + button_id * 16;        
      }
    }
  }
  else
    return 0;
}

void print_func (char *input_str){
  char cat_str[50];
  sprintf(cat_str, "Call by print_func: %s", input_str);
  Serial.println(cat_str);
}

/*
Get button ID
- Return ID = 1 to 4
*/
uint8_t get_button_id (void) {
  bool key_pressed = false;

  while (true) {
    if (digitalRead(BUTTON_1_PIN) == LOW)
      return 1;
    if (digitalRead(BUTTON_2_PIN) == LOW)
      return 2;
    if (digitalRead(BUTTON_3_PIN) == LOW)
      return 3;
    if (digitalRead(BUTTON_4_PIN) == LOW)
      return 4;   
  }
}



  void setup() {
  Serial.begin(9600);
  rgb_led.begin();
  rgb_led.setBrightness(BRIGHT_LEVEL);

  pinMode(BUTTON_1_PIN, INPUT);
  pinMode(BUTTON_2_PIN, INPUT);
  pinMode(BUTTON_3_PIN, INPUT);
  pinMode(BUTTON_4_PIN, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  /*
  rgb_led.setPixelColor(0, 0x00ff00);
  rgb_led.setPixelColor(1, 0xffff00);
  rgb_led.setPixelColor(2, 0xff0000);
  rgb_led.setPixelColor(3, 0x00ffff);
  rgb_led.show();

 
  stepper_motor_move(5, 128, CLOCKWISE);
  delay (1000);
  stepper_motor_move(5, 128, ANTICLOCKWISE);
  */

  rgb_led.setPixelColor(0, 0x00ff00);
  rgb_led.setPixelColor(1, 0xffff00);
  rgb_led.setPixelColor(2, 0xff0000);
  rgb_led.setPixelColor(3, 0x00ffff);
  rgb_led.show();
  delay (1000);
  rgb_led.clear();
  rgb_led.show();
  Serial.println("Stepper Motor Demo...");  


  while (true) {
    uint8_t button_action = button_status(10, 1000, LONG_PRESS_NOT_REPEAT);
    if (button_action > 0) {
      switch (button_action) {
        case 0x11:
          rgb_led.setPixelColor(0, 0xff0000);
          rgb_led.show();
          Serial.println("Motor move 1 step clockwise");
          stepper_motor_move(10, 1, CLOCKWISE);
          delay(10);
          Serial.println("Motor stop...");
          rgb_led.clear();
          rgb_led.show();
          break;
        case 0x21:
          rgb_led.setPixelColor(1, 0xffff00);
          rgb_led.show();
          Serial.println("Motor move 1 step anti-clockwise");
          stepper_motor_move(10, 1, ANTICLOCKWISE);
          delay(10);
          Serial.println("Motor stop...");
          rgb_led.clear();
          rgb_led.show();
          break;
        case 0x12:
          rgb_led.setPixelColor(0, 0xff0000);
          rgb_led.show();
          Serial.println("Motor move 90Deg clockwise");
          stepper_motor_move(10, 128, CLOCKWISE);
          Serial.println("Motor stop...");
          rgb_led.clear();
          rgb_led.show();
          break;
        case 0x22:
          rgb_led.setPixelColor(1, 0xffff00);
          rgb_led.show();
          Serial.println("Motor move 90Deg anti-clockwise");
          stepper_motor_move(10, 128, ANTICLOCKWISE);
          Serial.println("Motor stop...");
          rgb_led.clear();
          rgb_led.show();
          break;
        case 0x42:
          break;
      }
    }
    if (button_action == 0x42) {
      rgb_led.setPixelColor(0, 0x00ff00);
      rgb_led.setPixelColor(1, 0xffff00);
      rgb_led.setPixelColor(2, 0xff0000);
      rgb_led.setPixelColor(3, 0x00ffff);
      rgb_led.show();
      delay (500);
      rgb_led.clear();
      rgb_led.show();
      break;
    }
  }
  Serial.println("End....");

  //Use this infinite loop. Not use the Arduino stupid loop
  while (true) {
  }
}

void loop() { //Not use this loop
}
