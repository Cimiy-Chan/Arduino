#include "Arduino.h"
#include "HardwareSerial.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

class RgbLamp {
  public:
    int pwm_frequency;
    float brightness_level;

    Adafruit_PWMServoDriver pwm_RGB = Adafruit_PWMServoDriver(0x5F);
    //Constructor setup
    RgbLamp(int freq, int brightness) {
      pwm_frequency = freq;
      brightness_level =brightness/10.0;
      Serial.begin(115200);
      pwm_RGB.begin();
      pwm_RGB.setPWMFreq(pwm_frequency);
      pwm_RGB.begin();
      delay(100);
      Serial.println("RGB object created");

    }

    void color(int index,float R,float G,float B){
      R = int(map(R, 0,255, 0, 4095*brightness_level));
      G = int(map(G, 0,255, 0, 4095*brightness_level));
      B = int(map(B, 0,255, 0, 4095*brightness_level));

      if (index == 1){
        pwm_RGB.setPWM(0, 0, 4095-R); // (CH0, 0, PWM value)
        pwm_RGB.setPWM(1, 0, 4095-G); // (CH1, 0, PWM value)
        pwm_RGB.setPWM(2, 0, 4095-B); // (CH2, 0, PWM value)
      }
      else if (index == 2){
        pwm_RGB.setPWM(3, 0, 4095-R);
        pwm_RGB.setPWM(4, 0, 4095-G);
        pwm_RGB.setPWM(5, 0, 4095-B);
      }
    }
};


