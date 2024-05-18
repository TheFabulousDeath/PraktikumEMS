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


  long _AccX, _AccY, _gyroZ;
  long _calibrationAccX, _calibrationAccY, _calibrationGyroZ;
  long _avgAccX = 0, _avgAccY = 0, _avgGyroZ = 0; 
  int runMeasure = 0;
  long milliseconds = 0;
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
  static auto lastCheck = millis();
  
  if(runMeasure < CALIBRATION && isCalibrated == false) {
      if( millis() - lastCheck >= 100){
        lastCheck = millis();
        BHY2.update();

        _AccX = accel.x();
        _AccY = accel.y();
        _gyroZ = gyro.z();

        _calibrationAccX += accel.x();
        _calibrationAccY += accel.y();
        _calibrationGyroZ += gyro.z();
        //Serial.println("RAW READINGS  X: " + String(_AccX) + "\t Y: " + String(_AccY) + "\t Z: " + String(_gyroZ));
        runMeasure++;
      }
  } else if (isCalibrated == false)
  {
    _calibrationAccX /= CALIBRATION;
    _calibrationAccY /= CALIBRATION;
    _calibrationGyroZ /= CALIBRATION;
    runMeasure = 0;
    isCalibrated = true;
    Serial.println("CALIBRATION COMPLETE  X: " + String(_calibrationAccX) + "\t Y: " + String(_calibrationAccY) + "\t Z: " + String(_calibrationGyroZ)) + "\n";
  } else if(runMeasure < MEASUREMENTS)
  {

      if((milliseconds = millis()) - lastCheck >= 100){
        lastCheck = millis();
        BHY2.update();
        _AccX = accel.x();
        _AccY = accel.y();
        _gyroZ = gyro.z();

        _avgAccX += _AccX;
        _avgAccY += _AccY;
        _avgGyroZ += _gyroZ;
        Serial.println(String(milliseconds) + "," + String(_AccX) + "," + String(_AccY) + "," + String(_gyroZ));
        runMeasure++;
      }
  } else
  {
    _avgAccX = (_avgAccX / MEASUREMENTS) - _calibrationAccX;
    _avgAccY = (_avgAccY / MEASUREMENTS) - _calibrationAccY;
    _avgGyroZ = (_avgGyroZ / MEASUREMENTS) - _calibrationGyroZ;
    Serial.println("Averaged calibrated readings  X: " + String(_avgAccX) + "\t Y: " + String(_avgAccY) + "\t Z: " + String(_avgGyroZ) + "\n");
    delay(5000);
    runMeasure = 0;
  }
}
