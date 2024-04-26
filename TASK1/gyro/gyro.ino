//IN BRANCH 2604


#include <cstdio>
#include <cstdlib>
#include <Arduino_BHY2.h>
#include "Nicla_System.h"
  SensorXYZ accel(SENSOR_ID_ACC_PASS);
  SensorXYZ gyro(SENSOR_ID_GYRO_PASS);

  #define ACC_ANGGRG 4096/9.81
  #define CALIBRATION 100
  #define MEASUREMENTS 100
  #define DISCARD 10


  long _gyroX, _gyroY, _gyroZ;
  long _calibrationGyroX, _calibrationGyroY, _calibrationGyroZ;
  long _avgGyroX = 0, _avgGyroY = 0, _avgGyroZ = 0; 
  int runMeasure = 0;
  bool isCalibrated = false;
void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    BHY2.begin();
    accel.begin();
    gyro.begin();
  //DISCARD MEASUREMENTS
    for(int i = 0; i < DISCARD; i++)
    {
      BHY2.update();
      delay(100);
    }
    Serial.println("Some values discarded");
}

void loop() {
  // put your main code here, to run repeatedly:
  //_gyroX = gyro.x();
  static auto lastCheck = millis();
  
  if(runMeasure < CALIBRATION && isCalibrated == false) {
      if( millis() - lastCheck >= 100){
        lastCheck = millis();
        BHY2.update();
        _calibrationGyroX += gyro.x();
        _calibrationGyroY += gyro.y();
        _calibrationGyroZ += gyro.z();
        //Serial.println("RAW READINGS  X: " + String(_gyroX) + "\t Y: " + String(_gyroY) + "\t Z: " + String(_gyroZ));
        runMeasure++;
      }
  } else if (isCalibrated == false)
  {
    _calibrationGyroX /= CALIBRATION;
    _calibrationGyroY /= MEASUREMENTS;
    _calibrationGyroZ /= MEASUREMENTS;
    runMeasure = 0;
    isCalibrated = true;
    Serial.println("CALIBRATION COMPLETE  X: " + String(_calibrationGyroX) + "\t Y: " + String(_calibrationGyroY) + "\t Z: " + String(_calibrationGyroZ)) + "\n";
  } else if(runMeasure < MEASUREMENTS)
  {

      if( millis() - lastCheck >= 100){
        lastCheck = millis();
        BHY2.update();
        _gyroX = gyro.x();
        _gyroY = gyro.y();
        _gyroZ = gyro.z();

        _avgGyroX += _gyroX;
        _avgGyroY += _gyroY;
        _avgGyroZ += _gyroZ;
        Serial.println("RAW READINGS  X: " + String(_gyroX) + "\t Y: " + String(_gyroY) + "\t Z: " + String(_gyroZ));
        runMeasure++;
      }
  } else
  {
    _avgGyroX = (_avgGyroX / MEASUREMENTS) - _calibrationGyroX;
    _avgGyroY = (_avgGyroY / MEASUREMENTS) - _calibrationGyroY;
    _avgGyroZ = (_avgGyroZ / MEASUREMENTS) - _calibrationGyroZ;
    Serial.println("Averaged calibrated readings  X: " + String(_avgGyroX) + "\t Y: " + String(_avgGyroY) + "\t Z: " + String(_avgGyroZ) + "\n");
    delay(5000);
    runMeasure = 0;
  }
}
