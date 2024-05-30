#include <cstdio>
#include <cstdlib>

#include "Nicla_System.h"
#include <Arduino_BHY2.h>

#define CALIBRATION 100
#define MEASUREMENTS 1000

long _calX, _calY, _calZ;
SensorXYZ accel(SENSOR_ID_ACC_PASS);
long _AccX, _AccY, _AccZ;
int buffer;
int input;

void measure() {

  static auto lastCheck = millis();

  for(int i = 0; i < MEASUREMENTS;){
    if(millis()-lastCheck >= 100){
    lastCheck = millis();
    BHY2.update();

    _AccX = accel.x()-_calX;
    _AccY = accel.y()-_calY;
    _AccZ = accel.z()-_calZ;

    Serial.println(String(_AccX) + "," + String(_AccY) + "," + String(_AccZ));
    i++;
    }
  }
}

void printCalibration() {
  Serial.println(String(_calX) + "," + String(_calY) + "," + String(_calZ));
}

void accCalibrate() {
  static auto lastCheck = millis();
  _calX = 0;
  _calY = 0;
  _calZ = 0;

  for(int i = 0; i < CALIBRATION;){
    if(millis()-lastCheck >= 10){
      lastCheck = millis();
      BHY2.update();

      _calX += accel.x();
      _calY += accel.y();
      _calZ += accel.z();
      i++;
    }
  }
  _calX /= CALIBRATION;
  _calY /= CALIBRATION;
  _calZ /= CALIBRATION;
}

void setup() {
  Serial.begin(115200);
  BHY2.begin();
  accel.begin();
  Serial.println("ENSURE THAT THE SERIAL MONITOR DOES NOT SEND NEWLINE");
  accCalibrate();
  printCalibration();
}

void loop() {
  while ((buffer = Serial.available()) == 0) {
  }
  input = Serial.parseInt();
  Serial.println(input);
  switch(input) {
    case 0:
      break;
    case 1:
      accCalibrate();
      printCalibration();
      break;
    case 2:
      measure();
      break;
    default:
      break;
  }
}