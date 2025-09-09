/*
Serial port demonstration with GPIO4 blink
To show the effective of serial functional need to 
connect port to PC as the following:-

|-------|                      |---------|
|       |                      |         |
|       |      |--------------||         |
| ESP32 |<---->| UART <-> USB ||   PC    |
|       |      |--------------||         |
|       |                      |         |
|-------|                      |---------|


*/

#define TXD2 17
#define RXD2 16

const int GPIO4=4;
String myData;

void setup() {
	Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(GPIO4, OUTPUT);
}

void loop() {
	digitalWrite(GPIO4, HIGH);
  delay(200);
  digitalWrite(GPIO4, LOW);
  Serial2.println("Serial port from ESP32 DevKit3_V4");
  Serial2.print("Wait for input: ");

  //Wait for user input
  while (Serial2.available()==0) {}
    myData = Serial2.readStringUntil('\r\t');   
    Serial2.println("Data read: " + myData);
  

  delay (2000); //Delay 2 seconds
  
}
