#include "HardwareSerial.h"
#include <Arduino.h>
class UltraSonic {

  public:
    int trigPin;
    int echoPin;
    float maxDistance = 200;          // define the range(cm) for ultrasonic ranging module, Maximum sensor distance is rated at 400-500cm.
    float soundVelocity = 340;        // Sound velocity = 340 m/s
    float rangingTimeOut = 2 * maxDistance / 100 / soundVelocity * 1000000; // define the timeout(ms) for ultrasonic ranging module
    UltraSonic (int x, int y) {
      trigPin = x;
      echoPin = y;
      Serial.begin(115200);
      pinMode(trigPin, OUTPUT);
      pinMode(echoPin, INPUT);
      Serial.println("UltraSonic object created");
    }

    float getDistance() {
      unsigned long pingTime; // save the high level time returned by ultrasonic ranging module
      float distance;         // save the distance away from obstacle
      // set the trigPin output 10us high level to make the ultrasonic ranging module start to measure
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      // get the high level time returned by ultrasonic ranging module
      pingTime = pulseIn(echoPin, HIGH, rangingTimeOut);
      if (pingTime != 0) {  // if the measure is not overtime
        distance = pingTime * soundVelocity / 2 / 10000;  // calculate the obstacle distance(cm) according to the time of high level returned
        return distance;    // return distance(cm)
      }
      else                  // if the measure is overtime
        return maxDistance; // returns the maximum distance(cm)
    }
};