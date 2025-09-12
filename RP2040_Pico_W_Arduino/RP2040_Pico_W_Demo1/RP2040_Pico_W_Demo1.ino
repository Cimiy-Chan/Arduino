#include <DHT.h>
/*
* The demo bread board is configured with the following
1. Push buttone switch
    - GP11 - GP13
2. LED Output
    - GP2 - GP4
3. DHT11 Sensor
    - Out: GP14
4. Rotary encoder
    - CLK (A): GP18
    - DT (B): GP19
    - Push button SW: GP20
5. UART0: (TX=GP0)/(RX=GP1) are looped together for debug test

Notes: 
- For Pico-W, the "port number" of Build-in LED is 32
- Due to the IDE bug, DO NOT use the serial monitor inside the IDE apps. Othewise
  it will crash the system. Please use other serial monitor (for example PUTTY).

*/

#define DHT11_PIN 14
#define LED2 2
#define LED3 3
#define LED4 4

const int BI_LED = 32; //This is the default port number of Pico-W build-in LED
DHT dht11(DHT11_PIN, DHT11);

int counter = 20;
int byte_read = 0;
int led_pos = 3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //It doesn't work on IDE's serial monitor
  Serial1.begin (9600); // Init UART0 (pin 1, 2 of rpi pico-W)
  dht11.begin();
  pinMode(BI_LED, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  digitalWrite(BI_LED, LOW);
  Serial.println("Start application...");

}

void loop() {
  // put your main code here, to run repeatedly:
  //float temperature_reading = dht11.readTemperature();
  //float humidity_reading = dht11.readHumidity();
  digitalWrite(led_pos, HIGH);
  delay(100);
  digitalWrite(led_pos, LOW);
  delay(500);
  Serial.println("Looping...");
  int temp = Serial.available();
  Serial.println("Serial Available" + temp);
  if (Serial.available() > 0) {
    byte_read = Serial.read();

    Serial.print("Byte received: ");
    Serial.println(byte_read, DEC);
    if (byte_read == '3') {
      led_pos = 3;
    }
    else if (byte_read == '4') {
      led_pos = 4;
    }
    else {
      led_pos = 2;
    }
    if (byte_read == 3) { //Data 3 mean ctrl-C
      Serial.print("Halt processing");
      for (;;) {
        if (Serial.available() >0) {
          byte_read = Serial.read();
          if (byte_read == 's')
          break;
        }

      }
    }

  }
}
