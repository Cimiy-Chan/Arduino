/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

uint64_t receive_code[2]={0xFF6897, 0xFF30CF}; //Code for 0 and 1
char ASCII_code[2]={'0', '1'};

char get_code (unsigned long input_val) {
    for (int i = 0; i <=1;  i++) {
      if (input_val == receive_code[i]) {
        return ASCII_code[i];
      }
    }
    return '?';  
}



void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("IR remote demo...");
}

void loop() {
  //Serial.println(results.value, HEX);
  if (irrecv.decode(&results)) {
    char code = get_code(results.value);
    if (code != '?') {
      Serial.print("Code: ");
      Serial.println(code);
      //Light control codes.... Noepixel 
    }
    irrecv.resume(); // Receive the next value
  }

  delay(200);
}
