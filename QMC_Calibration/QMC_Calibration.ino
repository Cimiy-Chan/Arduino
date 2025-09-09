/**************************************************************************************************

Author: Vegetable_SYC

Address: https://github.com/Vegetable-SYC/IMU_Fusion_SYC

Version: v1.1.6

QMC5883L calibration steps:
Step 1: Start by running the following code directly and viewing serial monitor
step 2: You need to keep turning QMC5883L until the progress bar is full
Step 3: Enter the value obtained in step1 into imu.QMC5883L_SetOffsets and imu.QMC5883L_SetScales
Step 4: Annotation imu.QMC5883L_Calibration();
Step 5: Uncomment the function imu.Calculate and Serial.println(imu.getHeading())
Step 6: Upload the modified program to get the correct Angle value

**************************************************************************************************/
#include "IMU_Fusion_SYC.h"

IMU imu(Wire);

uint16_t loop_counter = 0;
uint16_t LOOP_END = 10;
bool is_calibrate = false;

void setup() {
  Serial.begin(9600);
  Serial.print("\n");
  Wire.begin();

  //Set up
  imu.begin(CHOOSE_MPU6050); //Setup for enabling bypass I2C for QMC5883L

  // After calibration set the QMC5883L calibration value
  imu.QMC5883L_SetOffsets(338.5, 516.5, 0);

  imu.QMC5883L_SetScales(1.00, 1.04, 0);
  imu.begin(CHOOSE_QMC5883L); // Select QMC5883L
}

void loop() {
  if (is_calibrate) {
    LOOP_END = 150;
    imu.QMC5883L_Calibration(); //Do calibration
    delay(1000);
  }
  else {
    imu.Calculate();
    Serial.print("Heading: ");
    Serial.println(imu.getHeading());
    delay(1000);
  }
  loop_counter++;
  if (loop_counter > LOOP_END) {
    Serial.print("End...");
    while(true); //End with dead loop
  }
}