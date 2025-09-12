#include <Arduino.h>

#define BUZZER_FREQUENCY 2000           //Define the resonant frequency of the buzzer 
class Buzzer {

  public:
    int pinBuzzer;
  
    Buzzer (int x) {
      pinBuzzer = x;
      pinMode(pinBuzzer, OUTPUT);
      delay (100);
    }

    void alert(int beat, int rebeat){
      beat = constrain(beat, 1, 9);
      rebeat = constrain(rebeat, 1, 255);
      for (int j = 0; j < rebeat; j++){
        for (int i = 0; i < beat; i++){
          tone(pinBuzzer, BUZZER_FREQUENCY);
          delay(100);
          noTone(pinBuzzer); //stop vocalizing
          delay(100);
        }
        delay(500);
      }
    }
};