#include <Arduino.h>

class TrackingSensor {

  public:
    int left_port;
    int mid_port;
    int right_port;

    TrackingSensor (int x, int y, int z) {
      left_port = x;
      mid_port = y;
      right_port = z;

      pinMode(left_port, INPUT);
      pinMode(mid_port, INPUT);
      pinMode(right_port, INPUT);
    }

    int trackValue (void) {
      return digitalRead(left_port) << 2 | digitalRead(mid_port) << 1 | digitalRead(right_port); 
    }
};