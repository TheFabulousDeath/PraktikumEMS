#include <cstdio>
#include <cstdlib>
#include <Arduino_BHY2.h>
#include "Nicla_System.h"
  SensorXYZ accel(SENSOR_ID_ACC_PASS);
  SensorXYZ gyro(SENSOR_ID_GYRO_PASS);

  #define ACC_ANGGRG 4096/9.81 //m/s²
  #define GYRO_ANGGRG 1/16.4 //Grad/Sekunde
  #define CALIBRATION 50
  #define MEASUREMENTS 200
  #define DISCARD 10


  long _AccX, _AccY, _AccZ, _gyroZ;
  long milliseconds;
void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    BHY2.begin();
    accel.begin();
    gyro.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  static auto lastCheck = millis();
      if((milliseconds = millis()) - lastCheck >= 20){
        lastCheck = millis();
        BHY2.update();
        _AccX = accel.x();
        _AccY = accel.y();
        _AccZ = accel.z();
        _gyroZ = gyro.z();

        Serial.println(String(milliseconds) + "," + String(_AccX) + "," + String(_AccY) + "," + String(_AccZ) + "," + String(_gyroZ));
      }
}
  
