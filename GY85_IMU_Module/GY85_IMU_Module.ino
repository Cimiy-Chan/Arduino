/*
  Modified on Mar 16, 2021
  Modified by MehranMaleki from Arduino Examples
  https://electro
  peak.com/learn/
*/

#include "I2Cdev.h"
#include "QMC5883LCompass.h"
#include "MPU6050.h"
#include <Adafruit_BMP085.h>


MPU6050 accelgyro;
Adafruit_BMP085 bmp;
QMC5883LCompass Compass;


int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

#define LED_PIN 13
bool blinkState = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // initialize devices
  Serial.println("Initializing I2C devices...");

  // initialize bmp085
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

  // initialize mpu6050
  accelgyro.reset();
  delay (100);
  accelgyro.resetAccelerometerPath();
  accelgyro.resetGyroscopePath();
  accelgyro.resetTemperaturePath();
  delay(100);
  accelgyro.setI2CMasterModeEnabled(false);
  accelgyro.setI2CBypassEnabled(true); // set bypass mode for gateway to hmc5883L
  accelgyro.initialize();
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  // Init compass QMC5883L
  Compass.init();

  //Checking IIC address
  int nDevices = 0;
    for (byte address = 1; address < 127; ++address) {
    // The i2c_scanner uses the return value of
    // the Wire.endTransmission to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");

      ++nDevices;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
  Serial.println("-----------------------------------------------");
  Serial.println();


  // configure Arduino LED for checking activity
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");
  
  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  Serial.print("Altitude = ");
  Serial.print(bmp.readAltitude());
  Serial.println(" meters");
  Serial.print("Pressure at sealevel (calculated) = ");
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");
  Serial.print("Real altitude = ");
  Serial.print(bmp.readAltitude(101500));
  Serial.println(" meters");

  
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  // display tab-separated accel/gyro x/y/z values
  Serial.print("a/g:\t");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);

  //Get compass
  Compass.read();
  byte AzimuthAngle = Compass.getAzimuth();
  char direction_array[3];
  Compass.getDirection(direction_array, AzimuthAngle);

  Serial.print("X: \t");
  Serial.print(direction_array[0]);
  Serial.print("  ");
  Serial.print("Y: \t");
  Serial.print(direction_array[1]);
  Serial.print("  ");
  Serial.print("Z: \t");
  Serial.print(direction_array[2]);
  Serial.print("AzimuthAngle: \t");
  Serial.print(AzimuthAngle);

  Serial.println();


  // blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);

  delay(1000);
}
